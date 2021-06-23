
#ifndef _TIMER_TCC0_CPP_
#define _TIMER_TCC0_CPP_


#include "Arduino.h"
#include "TimerTCC0.h"

#ifdef __SAMD21__

TimerTCC0 TimerTcc0;


void TCC0_Handler()
{
    Tcc* TC = (Tcc*) TCC0; // get timer struct

    if(TC->INTFLAG.bit.MC0 == 1) // A compare to cc0 caused the interrupt
    {
        TimerTcc0.isrCallback();
        TC->INTFLAG.bit.MC0 = 1; // writing a one clears the flag ovf flag
    }
}

void TimerTCC0::initialize(long microseconds)
{
    // Enable clock for TC 
    REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC0_TCC1) ;
    while(GCLK->STATUS.bit.SYNCBUSY == 1); // wait for sync 

    Tcc* TC = (Tcc*) TCC0;

    TC->CTRLA.reg &= ~TCC_CTRLA_ENABLE; // Disable TC
    while(TC->SYNCBUSY.bit.ENABLE == 1); // wait for sync 

    // Use match mode so that the timer counter resets when the count matches the compare register
    //TC->CTRLA.reg |= TCC_WAVE_WAVEGEN_MFRQ;
    TC->CTRLA.reg |= TCC_WAVE_WAVEGEN_NFRQ;
    while(TC->SYNCBUSY.bit.WAVE == 1);

    setPeriod(microseconds);
}

void TimerTCC0::setPeriod(long microseconds)
{
    Tcc* TC = (Tcc*) TCC0;
    
    uint32_t cycles = (CPU_HZ / 1000000) * microseconds;
    uint32_t prescalerConfigBits;
    
    if(cycles < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV1;
    else if((cycles >>= 1) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV2;
    else if((cycles >>= 1) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV4;
    else if((cycles >>= 1) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV8;
    else if((cycles >>= 1) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV16;
    else if((cycles >>= 2) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV64;
    else if((cycles >>= 2) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV256;
    else if((cycles >>= 2) < RESOLUTION) prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV1024;
    else cycles = RESOLUTION - 1, prescalerConfigBits = TCC_CTRLA_PRESCALER_DIV1024;
    /*
    SerialUSB.print("cycles is ");
    SerialUSB.println(cycles);
    SerialUSB.print("prescalerConfigBits is ");
    SerialUSB.println(prescalerConfigBits);
    */
    // Set prescaler
    TC->CTRLA.reg |= prescalerConfigBits;
    while(TC->SYNCBUSY.bit.ENABLE == 1);
    
    //TC->CC[0].reg = cycles;
    //while(TC->SYNCBUSY.bit.CC0 == 1);
    TC->PER.reg = cycles;              // Set counter Top using the PER register  
    while (TC->SYNCBUSY.bit.PER == 1); // wait for sync 
    
}

void TimerTCC0::attachInterrupt(void (*isr)())
{
    isrCallback = isr;
    
    start();
}

void TimerTCC0::detachInterrupt()
{
    // Disable InterruptVector
    NVIC_DisableIRQ(TCC0_IRQn);
    NVIC_ClearPendingIRQ(TCC0_IRQn);
    
    stop();
}

void TimerTCC0::start()
{
    Tcc* TC = (Tcc*) TCC0; // get timer struct
    
    // Enable the compare interrupt
    TC->INTENSET.reg = 0;
    TC->INTENSET.bit.MC0 = 1;
    
    // Enable InterruptVector
    NVIC_EnableIRQ(TCC0_IRQn);

    // Enable TC
    TC->CTRLA.reg |= TCC_CTRLA_ENABLE;
    while(TC->SYNCBUSY.bit.ENABLE == 1);
    
    TC->CTRLBSET.reg |= TCC_CTRLBSET_CMD_RETRIGGER; //  Start    
}

void TimerTCC0::restart()
{
   Tcc* TC = (Tcc*) TC3; // get timer struct
   TC->CTRLBSET.reg |= TCC_CTRLBCLR_CMD_RETRIGGER; // restart		
}

void TimerTCC0::stop()
{
    Tcc* TC = (Tcc*) TCC0; // get timer struct
    TC->CTRLBSET.reg |= TCC_CTRLBSET_CMD_STOP; // Stop counter    
}
#else
#error TCC0 is only on SAMD21 MCU! Use TC3 instead!

#endif

#endif