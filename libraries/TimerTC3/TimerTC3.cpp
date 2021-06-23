/*

Add SAMD51 TC3 peripheral support

Inspired by Khoi Hoang's package https://github.com/khoih-prog/SAMD_TimerInterrupt

Licensed under MIT license

*/
#ifndef _TIMER_TC3_CPP_
#define _TIMER_TC3_CPP_

#include "Arduino.h"
#include "TimerTC3.h"

TimerTC3 TimerTc3;

#ifdef __SAMD21__

void TC3_Handler()
{
    TcCount16 *TC = (TcCount16 *)TC3; // get timer struct

    if (TC->INTFLAG.bit.MC0 == 1) // A compare to cc0 caused the interrupt
    {
        TimerTc3.isrCallback();
        TC->INTFLAG.bit.MC0 = 1; // writing a one clears the flag ovf flag
    }
}

void TimerTC3::initialize(long microseconds)
{
    REG_GCLK_CLKCTRL = (uint16_t)(GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TCC2_TC3));
    while (GCLK->STATUS.bit.SYNCBUSY == 1)
        ;

    TcCount16 *TC = (TcCount16 *)TC3;

    TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;

    // Use the 16-bit timer
    TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
    while (TC->STATUS.bit.SYNCBUSY == 1)
        ;

    // Use match mode so that the timer counter resets when the count matches the compare register
    TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
    //TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ;
    while (TC->STATUS.bit.SYNCBUSY == 1)
        ;

    setPeriod(microseconds);
}

void TimerTC3::setPeriod(long microseconds)
{
    TcCount16 *TC = (TcCount16 *)TC3;

    uint32_t cycles = (CPU_HZ / 1000000) * microseconds;
    uint32_t prescalerConfigBits;

    if (cycles < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV1;
    else if ((cycles >>= 1) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV2;
    else if ((cycles >>= 1) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV4;
    else if ((cycles >>= 1) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV8;
    else if ((cycles >>= 1) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV16;
    else if ((cycles >>= 2) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV64;
    else if ((cycles >>= 2) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV256;
    else if ((cycles >>= 2) < RESOLUTION)
        prescalerConfigBits = TC_CTRLA_PRESCALER_DIV1024;
    else
        cycles = RESOLUTION - 1, prescalerConfigBits = TC_CTRLA_PRESCALER_DIV1024;
    /*
    SerialUSB.print("cycles is ");
    SerialUSB.println(cycles);
    SerialUSB.print("prescalerConfigBits is ");
    SerialUSB.println(prescalerConfigBits);
    */
    // Set prescaler
    TC->CTRLA.reg |= prescalerConfigBits;
    while (TC->STATUS.bit.SYNCBUSY == 1)
        ;

    TC->CC[0].reg = cycles;
    while (TC->STATUS.bit.SYNCBUSY == 1)
        ;

    //TC->PER.reg = cycles;
    //while (TC->STATUS.bit.SYNCBUSY == 1);
}

void TimerTC3::attachInterrupt(void (*isr)())
{
    isrCallback = isr;

    start();
}

void TimerTC3::detachInterrupt()
{
    // Disable InterruptVector
    NVIC_DisableIRQ(TC3_IRQn);
    NVIC_ClearPendingIRQ(TC3_IRQn);

    stop();
}

void TimerTC3::start()
{
    TcCount16 *TC = (TcCount16 *)TC3; // get timer struct

    // Enable the compare interrupt
    TC->INTENSET.reg = 0;
    TC->INTENSET.bit.MC0 = 1;

    // Enable InterruptVector
    NVIC_EnableIRQ(TC3_IRQn);

    // Enable TC
    TC->CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC->STATUS.bit.SYNCBUSY == 1)
        ;

    TC->CTRLBSET.reg |= TC_CTRLBSET_CMD_RETRIGGER; //  Start
}

void TimerTC3::restart()
{
    TcCount16 *TC = (TcCount16 *)TC3;              // get timer struct
    TC->CTRLBSET.reg |= TC_CTRLBCLR_CMD_RETRIGGER; // restart
}

void TimerTC3::stop()
{
    TcCount16 *TC = (TcCount16 *)TC3;         // get timer struct
    TC->CTRLBSET.reg |= TC_CTRLBSET_CMD_STOP; // Stop counter
}
#elif defined __SAMD51__
#define TC3_wait_for_sync() while (TC3->COUNT16.SYNCBUSY.reg != 0)

void TC3_Handler()
{

    if (TC3->COUNT16.INTFLAG.bit.MC0 == 1) // A compare to cc0 caused the interrupt
    {
        TimerTc3.isrCallback();
        TC3->COUNT16.INTFLAG.bit.MC0 = 1; // writing a one clears the flag ovf flag
    }
}

void TimerTC3::initialize(long microseconds)
{
    GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN_GCLK1_Val | (1 << GCLK_PCHCTRL_CHEN_Pos);

    while (GCLK->SYNCBUSY.reg > 0)
        ;

    TC3->COUNT16.CTRLA.bit.ENABLE = 0;

    // Use match mode so that the timer counter resets when the count matches the
    // compare register
    TC3->COUNT16.WAVE.bit.WAVEGEN = TC_WAVE_WAVEGEN_MFRQ;
    TC3_wait_for_sync();

    setPeriod(microseconds);
}

void TimerTC3::setPeriod(long microseconds)
{
    uint32_t TC_CTRLA_PRESCALER_DIVN;
    int _prescaler = 0;
    int _compareValue = 0;
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1024;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV256;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV64;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV16;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV4;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV2;
    TC3_wait_for_sync();
    TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_PRESCALER_DIV1;
    TC3_wait_for_sync();

    if (microseconds > 300000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV1024;
        _prescaler = 1024;
    }
    else if (80000 < microseconds && microseconds <= 300000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV256;
        _prescaler = 256;
    }
    else if (20000 < microseconds && microseconds <= 80000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV64;
        _prescaler = 64;
    }
    else if (10000 < microseconds && microseconds <= 20000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV16;
        _prescaler = 16;
    }
    else if (5000 < microseconds && microseconds <= 10000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV8;
        _prescaler = 8;
    }
    else if (2500 < microseconds && microseconds <= 5000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV4;
        _prescaler = 4;
    }
    else if (1000 < microseconds && microseconds <= 2500)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV2;
        _prescaler = 2;
    }
    else if (microseconds <= 1000)
    {
        TC_CTRLA_PRESCALER_DIVN = TC_CTRLA_PRESCALER_DIV1;
        _prescaler = 1;
    }

    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIVN;
    TC3_wait_for_sync();

    _compareValue = (int)(CPU_HZ / (_prescaler / ((float)microseconds / 1000000))) - 1;

    // Make sure the count is in a proportional position to where it was
    // to prevent any jitter or disconnect when changing the compare value.
    TC3->COUNT16.COUNT.reg = map(TC3->COUNT16.COUNT.reg, 0,
                                 TC3->COUNT16.CC[0].reg, 0, _compareValue);
    TC3->COUNT16.CC[0].reg = _compareValue;
    TC3_wait_for_sync();

    TC3->COUNT16.CTRLA.bit.ENABLE = 1;
    TC3_wait_for_sync();
}

void TimerTC3::attachInterrupt(void (*isr)())
{
    isrCallback = isr;

    start();
}

void TimerTC3::detachInterrupt()
{
    // Disable InterruptVector
    NVIC_DisableIRQ(TC3_IRQn);
    NVIC_ClearPendingIRQ(TC3_IRQn);

    stop();
}

void TimerTC3::start()
{

    // Enable the compare interrupt
    TC3->COUNT16.INTENSET.reg = 0;
    TC3->COUNT16.INTENSET.bit.MC0 = 1;

    // Enable InterruptVector
    NVIC_EnableIRQ(TC3_IRQn);

    // Enable TC
    TC3->COUNT16.CTRLA.bit.ENABLE = 1;
    TC3_wait_for_sync();

    //TC->CTRLBSET.reg |= TC_CTRLBSET_CMD_RETRIGGER; //  Start
}

void TimerTC3::restart()
{
    TC3->COUNT16.CTRLA.bit.ENABLE = 1;
    TC3_wait_for_sync();
}

void TimerTC3::stop()
{
    TC3->COUNT16.CTRLA.bit.ENABLE = 0;
    NVIC_DisableIRQ(TC3_IRQn);
}
#else
#error This library is design only for SAMD51 and SAMD21 MCUs!
#endif

#endif