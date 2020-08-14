/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_WIO_TERMINAL
#define _VARIANT_WIO_TERMINAL

// The definitions here needs a SAMD core >=1.6.10
#define ARDUINO_SAMD_VARIANT_COMPLIANCE 10610

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC (32768ul)

/** Master clock frequency */
#define VARIANT_MCK (F_CPU)

#define VARIANT_GCLK0_FREQ (F_CPU)
#define VARIANT_GCLK1_FREQ (48000000UL)
#define VARIANT_GCLK2_FREQ (100000000UL)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
#include "SERCOM.h"
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT (sizeof(g_APinDescription) / sizeof(g_APinDescription[0]))
#define NUM_DIGITAL_PINS (23u)
#define NUM_ANALOG_INPUTS (14u)
#define NUM_ANALOG_OUTPUTS (2u)
#define analogInputToDigitalPin(p) (p)

#define digitalPinToPort(P) (&(PORT->Group[g_APinDescription[P].ulPort]))
#define digitalPinToBitMask(P) (1 << g_APinDescription[P].ulPin)
//#define analogInPinToBit(P)        ( )
#define portOutputRegister(port) (&(port->OUT.reg))
#define portInputRegister(port) (&(port->IN.reg))
#define portModeRegister(port) (&(port->DIR.reg))
#define digitalPinHasPWM(P) (g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER)

/*
 * digitalPinToTimer(..) is AVR-specific and is not defined for SAMD
 * architecture. If you need to check if a pin supports PWM you must
 * use digitalPinHasPWM(..).
 *
 * https://github.com/arduino/Arduino/issues/1833
 */
// #define digitalPinToTimer(P)

// LEDs
#define PIN_LED_13 (13u)
#define PIN_LED_RXL (13u)
#define PIN_LED_TXL (13u)
#define PIN_LED PIN_LED_13
#define PIN_LED2 PIN_LED_RXL
#define PIN_LED3 PIN_LED_TXL
#define LED_BUILTIN PIN_LED_13
#define PIN_NEOPIXEL (13u)

//Digital PINs
#define D0 (0ul)
#define D1 (1ul)
#define D2 (2ul)
#define D3 (3ul)
#define D4 (4ul)
#define D5 (5ul)
#define D6 (6ul)
#define D7 (7ul)
#define D8 (8ul)

//Analog PINs

static const uint8_t A0 = D0;
static const uint8_t A1 = D1;
static const uint8_t A2 = D2;
static const uint8_t A3 = D3;
static const uint8_t A4 = D4;
static const uint8_t A5 = D5;
static const uint8_t A6 = D6;
static const uint8_t A7 = D7;
static const uint8_t A8 = D8;

// 3.3V    ||    5V
// BCM2    ||    5V
// BCM3    ||    GND
// BCM4    ||    BCM14
// GND     ||    BCM15
// BCM17   ||    BCM18
// BCM27   ||    GND
// BCM22   ||    BCM23
// GND     ||    BCM24
// BCM10   ||    GND
// BCM9    ||    BCM25
// BCM11   ||    BCM8
// GND     ||    BCM7
// BCM0    ||    BCM1
// BCM5    ||    GND
// BCM6    ||    BCM12
// BCM13   ||    GND
// BCM19   ||    BCM16
// BCM26   ||    BCM20
// GND     ||    BCM21

//PIN DEFINE FOR RPI
#define BCM0 (78ul)
#define BCM1 (77ul)
#define BCM2 (46ul)
#define BCM3 (45ul)
#define BCM4 (36ul)
#define BCM5 (37ul)
#define BCM6 (38ul)
#define BCM7 (10ul)
#define BCM8 (50ul)
#define BCM9 (47ul)
#define BCM10 (48ul)
#define BCM11 (49ul)
#define BCM12 (5ul)
#define BCM13 (6ul)
#define BCM14 (40ul)
#define BCM15 (41ul)
#define BCM16 (7ul)
#define BCM17 (9ul)
#define BCM18 (52ul)
#define BCM19 (15ul)
#define BCM20 (54ul)
#define BCM21 (53ul)
#define BCM22 (1ul)
#define BCM23 (2ul)
#define BCM24 (3ul)
#define BCM25 (4ul)
#define BCM26 (8ul)
#define BCM27 (0ul)

// FPC NEW DEFINE
#define FPC1 (17ul)
#define FPC2 (FPC1 + 1)
#define FPC3 (FPC1 + 2)
#define FPC4 (FPC1 + 3)
#define FPC5 (FPC1 + 4)
#define FPC6 (FPC1 + 5)
#define FPC7 (FPC1 + 6)
#define FPC8 (FPC1 + 7)
#define FPC9 (FPC1 + 8)
#define FPC10 (FPC1 + 9)

/*
 * RPI Analog RPIs
 */
#define RPI_A0 (0ul)
#define RPI_A1 (RPI_A0 + 1)
#define RPI_A2 (RPI_A0 + 2)
#define RPI_A3 (RPI_A0 + 3)
#define RPI_A4 (RPI_A0 + 4)
#define RPI_A5 (RPI_A0 + 5)
#define RPI_A6 (RPI_A0 + 6)
#define RPI_A7 (RPI_A0 + 7)
#define RPI_A8 (RPI_A0 + 8)

#define PIN_DAC0 (9ul)
#define PIN_DAC1 (10ul)

static const uint8_t DAC0 = PIN_DAC0;
static const uint8_t DAC1 = PIN_DAC1;

/*
 * FPO Analog RPIs
 */
#define FPC_A7 FPC_D7
#define FPC_A8 FPC_D8
#define FPC_A9 FPC_D9
#define FPC_A11 FPC_D11
#define FPC_A12 FPC_D12
#define FPC_A13 FPC_D13

#define ADC_RESOLUTION 12

/*
 * USB
 */
#define PIN_USB_DM (42ul)
#define PIN_USB_DP (43ul)
#define PIN_USB_HOST_ENABLE (44ul)

/*
 * BUTTON
 */
#define BUTTON_1 (28ul)
#define BUTTON_2 (29ul)
#define BUTTON_3 (30ul)
#define WIO_KEY_A (28ul)
#define WIO_KEY_B (29ul)
#define WIO_KEY_C (30ul)

/*
 * SWITCH
 */
#define SWITCH_X (31ul)
#define SWITCH_Y (32ul)
#define SWITCH_Z (33ul)
#define SWITCH_B (34ul)
#define SWITCH_U (35ul)

#define WIO_5S_UP (31ul)
#define WIO_5S_LEFT (32ul)
#define WIO_5S_RIGHT (33ul)
#define WIO_5S_DOWN (34ul)
#define WIO_5S_PRESS (35ul)
/*
 * IRQ0
 */
#define IRQ0 (11ul)

/*
 * BUZZER_CTR
 */
#define BUZZER_CTR (12ul)
#define WIO_BUZZER (12ul)

/*
 * MIC_INPUT
 */
#define MIC_INPUT (39ul)
#define WIO_MIC (39ul)
/*
 * GCLK
 */
#define GCLK0 (36ul)
#define GCLK1 (37ul)
#define GCLK2 (38ul)

/*
 * Serial interfaces
 */

// Serial2
#define PIN_SERIAL2_RX (83ul)
#define PIN_SERIAL2_TX (82ul)
#define PAD_SERIAL2_RX (SERCOM_RX_PAD_1)
#define PAD_SERIAL2_TX (UART_TX_PAD_0)
#define SERCOM_SERIAL2 sercom1

// UART GPIO (Host)
//  static Uart GpioSerial(&SERCOM_GPIO_SERIAL, PIN_GPIO_SERIAL_RX, PIN_GPIO_SERIAL_TX, PAD_GPIO_SERIAL_RX, PAD_GPIO_SERIAL_TX);
//  INTERRUPT_HANDLER_IMPLEMENT_GPIO_SERIAL(GpioSerial)
#define PIN_GPIO_SERIAL_RX		(BCM15)
#define PIN_GPIO_SERIAL_TX		(BCM14)
#define PAD_GPIO_SERIAL_RX		(SERCOM_RX_PAD_1)
#define PAD_GPIO_SERIAL_TX		(UART_TX_PAD_0)
#define SERCOM_GPIO_SERIAL		sercom2
#define INTERRUPT_HANDLER_IMPLEMENT_GPIO_SERIAL(uart) \
	void SERCOM2_0_Handler() \
	{ \
		(uart).IrqHandler(); \
	} \
	void SERCOM2_1_Handler() \
	{ \
		(uart).IrqHandler(); \
	} \
	void SERCOM2_2_Handler() \
	{ \
		(uart).IrqHandler(); \
	} \
	void SERCOM2_3_Handler() \
	{ \
		(uart).IrqHandler(); \
	}

// UART GPIO (Device)
//  Uart GpioSerial(&SERCOM_GPIO_SERIAL_X, PIN_GPIO_SERIAL_X_RX, PIN_GPIO_SERIAL_X_TX, PAD_GPIO_SERIAL_X_RX, PAD_GPIO_SERIAL_X_TX);
//  INTERRUPT_HANDLER_IMPLEMENT_GPIO_SERIAL_X(GpioSerial)

#define PIN_GPIO_SERIAL_X_RX	(93ul)
#define PIN_GPIO_SERIAL_X_TX	(94ul)
#define PAD_GPIO_SERIAL_X_RX	(SERCOM_RX_PAD_1)
#define PAD_GPIO_SERIAL_X_TX	(UART_TX_PAD_0)
#define SERCOM_GPIO_SERIAL_X	sercom4
#define INTERRUPT_HANDLER_IMPLEMENT_GPIO_SERIAL_X(uart) \
	void SERCOM4_0_Handler() \
	{ \
		(uart).IrqHandler(); \
	} \
	void SERCOM4_1_Handler() \
	{ \
		(uart).IrqHandler(); \
	} \
	void SERCOM4_2_Handler() \
	{ \
		(uart).IrqHandler(); \
	} \
	void SERCOM4_3_Handler() \
	{ \
		(uart).IrqHandler(); \
	}

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 2

#define PIN_WIRE_SDA (46ul)
#define PIN_WIRE_SCL (45ul)
#define PERIPH_WIRE sercom3
#define WIRE_IT_HANDLER SERCOM3_Handler

  static const uint8_t SDA = PIN_WIRE_SDA;
  static const uint8_t SCL = PIN_WIRE_SCL;

#define PIN_WIRE1_SDA (78ul)
#define PIN_WIRE1_SCL (77ul)
#define PERIPH_WIRE1 sercom4
#define WIRE1_IT_HANDLER SERCOM4_Handler

  static const uint8_t SDA1 = PIN_WIRE1_SDA;
  static const uint8_t SCL1 = PIN_WIRE1_SCL;

#define PIN_GYROSCOPE_WIRE_SDA PIN_WIRE1_SDA
#define PIN_GYROSCOPE_WIRE_SCL PIN_WIRE1_SCL
#define GYROSCOPE_WIRE Wire1
#define GYROSCOPE_INT1 (79ul)

#define WIO_LIS3DH_SDA PIN_WIRE1_SDA
#define WIO_LIS3DH_SCL PIN_WIRE1_SCL
#define WIO_LIS3DH_WiRE Wire1
#define WIO_LIS3DH_INT (79ul)
/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 4

#define PIN_SPI_MISO (47ul)
#define PIN_SPI_MOSI (48ul)
#define PIN_SPI_SCK (49ul)
#define PIN_SPI_SS (50ul)
#define PERIPH_SPI sercom5
#define PAD_SPI_TX SPI_PAD_0_SCK_1
#define PAD_SPI_RX SERCOM_RX_PAD_2

  static const uint8_t SS = PIN_SPI_SS;
  static const uint8_t MOSI = PIN_SPI_MOSI;
  static const uint8_t MISO = PIN_SPI_MISO;
  static const uint8_t SCK = PIN_SPI_SCK;

#define PIN_SPI1_MISO (84ul)
#define PIN_SPI1_MOSI (85ul)
#define PIN_SPI1_SCK (86ul)
#define PIN_SPI1_SS (87ul)
#define PERIPH_SPI1 sercom0
#define PAD_SPI1_TX SPI_PAD_0_SCK_1
#define PAD_SPI1_RX SERCOM_RX_PAD_2

  static const uint8_t SS1 = PIN_SPI1_SS;
  static const uint8_t MOSI1 = PIN_SPI1_MOSI;
  static const uint8_t MISO1 = PIN_SPI1_MISO;
  static const uint8_t SCK1 = PIN_SPI1_SCK;

#define PIN_SPI2_MISO (61ul)
#define PIN_SPI2_MOSI (62ul)
#define PIN_SPI2_SCK (63ul)
#define PIN_SPI2_SS (64ul)
#define PERIPH_SPI2 sercom6
#define PAD_SPI2_TX SPI_PAD_0_SCK_1
#define PAD_SPI2_RX SERCOM_RX_PAD_2

  static const uint8_t SS2 = PIN_SPI2_SS;
  static const uint8_t MOSI2 = PIN_SPI2_MOSI;
  static const uint8_t MISO2 = PIN_SPI2_MISO;
  static const uint8_t SCK2 = PIN_SPI2_SCK;

#define PIN_SPI3_MISO (66u)
#define PIN_SPI3_MOSI (67u)
#define PIN_SPI3_SCK (68u)
#define PIN_SPI3_SS (69u)
#define PERIPH_SPI3 sercom7
#define PAD_SPI3_TX SPI_PAD_3_SCK_1
#define PAD_SPI3_RX SERCOM_RX_PAD_2

  static const uint8_t SS3 = PIN_SPI3_SS;
  static const uint8_t MOSI3 = PIN_SPI3_MOSI;
  static const uint8_t MISO3 = PIN_SPI3_MISO;
  static const uint8_t SCK3 = PIN_SPI3_SCK;

// Needed for SD library
#define SDCARD_SPI SPI2
#define SDCARD_MISO_PIN PIN_SPI2_MISO
#define SDCARD_MOSI_PIN PIN_SPI2_MOSI
#define SDCARD_SCK_PIN PIN_SPI2_SCK
#define SDCARD_SS_PIN PIN_SPI2_SS
#define SDCARD_DET_PIN (65ul)

// Needed for LCD library
#define LCD_SPI SPI3
#define LCD_MISO_PIN PIN_SPI3_MISO
#define LCD_MOSI_PIN PIN_SPI3_MOSI
#define LCD_SCK_PIN PIN_SPI3_SCK
#define LCD_SS_PIN PIN_SPI3_SS
#define LCD_DC (70ul)
#define LCD_RESET (71Ul)
#define LCD_BACKLIGHT (72Ul)
#define LCD_XL (73ul)
#define LCD_YU (74ul)
#define LCD_XR (75ul)
#define LCD_YD (76ul)

// Needed for RTL8720D
#define RTL8720D_SPI          SPI1
#define RTL8720D_MISO_PIN     PIN_SPI1_MISO
#define RTL8720D_MOSI_PIN     PIN_SPI1_MOSI
#define RTL8720D_SCK_PIN      PIN_SPI1_SCK
#define RTL8720D_SS_PIN       PIN_SPI1_SS

//QSPI Pins
#define PIN_QSPI_IO0 (55ul)
#define PIN_QSPI_IO1 (56ul)
#define PIN_QSPI_IO2 (57ul)
#define PIN_QSPI_IO3 (58ul)
#define PIN_QSPI_SCK (59ul)
#define PIN_QSPI_CS (60ul)

/*
 * I2S Interfaces
 */
#define I2S_INTERFACES_COUNT 1

#define I2S_DEVICE 0
#define I2S_CLOCK_GENERATOR 3

#define PIN_I2S_FS (51ul)
#define PIN_I2S_SCK (52ul)
#define PIN_I2S_SDO (53ul)
#define PIN_I2S_SDI (54ul)

#define I2S_LRCLK (51ul)
#define I2S_BLCK (52ul)
#define I2S_SDOUT (53ul)
#define I2S_SDIN (54ul)

// /*
//  * RTL8720D Interfaces
//  */
#define RTL8720D_CHIP_PU        (80ul)
#define RTL8720D_GPIO0          (81ul)

/*
 * SWD
 */
#define SWDCLK (88ul)
#define SWDIO (89ul)
#define SWO (90ul)

/*
 * light sensor
 */
#define WIO_LIGHT (27ul)
/*
 * ir sensor
 */
#define WIO_IR  (14ul)
/*
 * 
 */
#define OUTPUT_CTR_5V (91ul)
#define OUTPUT_CTR_3V3 (92ul)

#if !defined(VARIANT_QSPI_BAUD_DEFAULT)
// TODO: meaningful value for this
#define VARIANT_QSPI_BAUD_DEFAULT 5000000
#endif

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
*/
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4;
extern SERCOM sercom5;
extern SERCOM sercom6;
extern SERCOM sercom7;

extern Uart Serial2;

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_USBVIRTUAL Serial
#define SERIAL_PORT_MONITOR Serial
// Serial has no physical pins broken out, so it's not listed as HARDWARE port
#define SERIAL_PORT_HARDWARE
#define SERIAL_PORT_HARDWARE_OPEN
#define RTL8720D Serial2
// Alias Serial to SerialUSB
#define SerialUSB Serial

#endif /* _VARIANT_GREOVE_UI_M4_WIRELESS */
