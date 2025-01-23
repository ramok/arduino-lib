// This file is part of the PinChangeInt library for the Arduino.  This library will work on any ATmega328-based
// or ATmega2560-based Arduino, as well as the Sanguino or Mioduino.

// Most of the pins of an Arduino Uno use Pin Change Interrupts, and because of the way the ATmega interrupt
// system is designed it is difficult to trigger an Interrupt Service Request off of any single pin, and on
// any change of state (either rising, or falling, or both).  The goal of this library is to make it easy for
// the programmer to attach an ISR so it will trigger on any change of state on any Pin Change Interrupt pin.

// (NOTE TO SELF: Update the PCINT_VERSION define, below) -----------------
#define PCINT_VERSION 2402
/*
Copyright 2008 Chris J. Kiick
Copyright 2009-2011 Lex Talionis
Copyright 2010-2014 Michael Schwager (aka, "GreyGnome")

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * QUICKSTART
 *
 * For the beginners/lazy/busy/wreckless:
 * To attach an interrupt to your Arduino Pin, calling your function "userFunc", and acting on
 * the "mode", which is a change in the pin's state; either RISING or FALLING or CHANGE:
 *		attachPinChangeInterrupt(pin,userFunc,mode)
 * Your function must not return any values and it cannot take any arguments (that is, its definition
 * has to look like this:
 *		void userFunc() {
 *		  ...your code here...
 *		}
 *
 * That's it. Everything else are details.
 *
 * If you need to exchange information to/from the interrupt, you can use global volatile variables.
 * See the example for more information.
 *
 *	You probably will not need to do this, but later in your sketch you can detach the interrupt:
 *		detachPinChangeInterrupt(pin)
 *
 *	If you want to see what the *last* pin that triggered an interrupt was, you can get it this way:
 *		getInterruptedPin()
 *	Note: If you have multiple pins that are triggering interrupts and they are sufficiently fast,
 *	you will not be able to find all the pins that interrupted.
*/

//
// For the beginners
//
#define	detachPinChangeInterrupt(pin)				PCintPort::detachInterrupt(pin)
#define	attachPinChangeInterrupt(pin,userFunc,mode)	PCintPort::attachInterrupt(pin, &userFunc,mode)
#define getInterruptedPin()							PCintPort::getArduinoPin()

// We use 4-character tabstops, so IN VIM:  <esc>:set ts=4 sw=4 sts=4
// ...that's: ESCAPE key, colon key, then
//		"s-e-t SPACE key t-s = 4 SPACE key s-w = 4 SPACE key s-t-s = 4"

/*
 * 	This is the PinChangeInt library for the Arduino.
	This library provides an extension to the interrupt support for arduino by adding pin change
	interrupts, giving a way for users to have interrupts drive off of any pin (ATmega328-based
	Arduinos) and by the Port B, J, and K pins on the Arduino Mega and its ilk (see the README file).

	See the README for license, acknowledgments, and other details (especially concerning the Arduino MEGA).

	See google code project for latest, bugs and info http://code.google.com/p/arduino-pinchangeint/
	See github for the bleeding edge code: https://github.com/GreyGnome/PinChangeInt
	For more information Refer to avr-gcc header files, arduino source and atmega datasheet.

	This library was inspired by and derived from Chris J. Kiick's PCInt Arduino Playground
	example here: http://www.arduino.cc/playground/Main/PcInt
	Nice job, Chris!
*/

//-------- define these in your sketch, if applicable ----------------------------------------------------------
//-------- These must go in your sketch ahead of the #include <PinChangeInt.h> statement -----------------------
// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
// #define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// --- Mega support ---
// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTJ_PINCHANGES // to indicate that port j will not be used for pin change interrupts
// #define NO_PORTK_PINCHANGES // to indicate that port k will not be used for pin change interrupts
// In the Mega, there is no Port C, no Port D.  Instead, you get Port J and Port K.  Port B remains.
// Port J, however, is practically useless because there is only 1 pin available for interrupts.  Most
// of the Port J pins are not even connected to a header connection.  // </end> "Mega Support" notes
// --- Sanguino, Mioduino support ---
// #define NO_PORTA_PINCHANGES // to indicate that port a will not be used for pin change interrupts

// You can reduce the code size by 20-50 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variables PCintPort::pinState and/or
// PCintPort::arduinoPin are set and made available to your interrupt routine.
// #define NO_PIN_STATE        // to indicate that you don't need the pinState
// #define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
// #define DISABLE_PCINT_MULTI_SERVICE // to limit the handler to servicing a single interrupt per invocation.
// #define GET_PCINT_VERSION   // to enable the uint16_t getPCIintVersion () function.
// The following is intended for testing purposes.  If defined, then a whole host of static variables can be read
// in your interrupt subroutine.  It is not defined by default, and you DO NOT want to define this in
// Production code!:
// #define PINMODE
//-------- define the above in your sketch, if applicable ------------------------------------------------------

/*
	VERSIONS found in moved to RELEASE_NOTES.

	See the README file for the License and more details.
*/

#ifndef PinChangeInt_h
#define	PinChangeInt_h

#include "stddef.h"

// Maurice Beelen, nms277, Akesson Karlpetter, and Orly Andico
// sent in fixes to work with Arduino >= version 1.0
#include <arduino.h>
#include <new.h>
#include <pins_arduino.h>
#include <wiring_private.h> // cbi and sbi defined here

#undef DEBUG

/*
* Theory: For the IO pins covered by Pin Change Interrupts
* (== all of them on the Atmega168/328, and a subset on the Atmega2560),
* the PCINT corresponding to the pin must be enabled and masked, and
* an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
* must use some logic to actually implement a per-pin interrupt service.
*/

/* Pin to interrupt map, ATmega328:
* D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
* D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
* A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
*/

#undef	INLINE_PCINT
#define INLINE_PCINT
// Thanks to cserveny...@gmail.com for MEGA support!
#if defined __AVR_ATmega2560__ || defined __AVR_ATmega1280__ || defined __AVR_ATmega1281__ || defined __AVR_ATmega2561__ || defined __AVR_ATmega640__
	#define __USE_PORT_JK
	// Mega does not have PORTA, C or D
	#define NO_PORTA_PINCHANGES
	#define NO_PORTC_PINCHANGES
	#define NO_PORTD_PINCHANGES
	#if ((defined(NO_PORTB_PINCHANGES) && defined(NO_PORTJ_PINCHANGES)) || \
			(defined(NO_PORTJ_PINCHANGES) && defined(NO_PORTK_PINCHANGES)) || \
			(defined(NO_PORTK_PINCHANGES) && defined(NO_PORTB_PINCHANGES)))
		#define	INLINE_PCINT inline
	#endif
#else
	#define NO_PORTJ_PINCHANGES
	#define NO_PORTK_PINCHANGES
	#if defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
		#ifndef NO_PORTA_PINCHANGES
			#define __USE_PORT_A
		#endif
	#else
		#define NO_PORTA_PINCHANGES
	#endif
	// if defined only D .OR. only C .OR. only B .OR. only A, then inline it
	#if (   (defined(NO_PORTA_PINCHANGES) && defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES)) || \
			(defined(NO_PORTA_PINCHANGES) && defined(NO_PORTB_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTA_PINCHANGES) && defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) )
		#define	INLINE_PCINT inline
	#endif
#endif

// Provide drop in compatibility with Chris J. Kiick's PCInt project at
// http://www.arduino.cc/playground/Main/PcInt
#define	PCdetachInterrupt(pin)	PCintPort::detachInterrupt(pin)
#define	PCattachInterrupt(pin,userFunc,mode) PCintPort::attachInterrupt(pin, userFunc,mode)
#define PCgetArduinoPin() PCintPort::getArduinoPin()

typedef void (*PCIntvoidFuncPtr)(void);

class PCintPort {
public:
	// portB=PCintPort(2, 1,PCMSK1);
	// index:   portInputReg(*portInputRegister(index)), 
	// pcindex: PCICRbit(1 << pcindex)
	// maskReg: portPCMask(maskReg)
	PCintPort(int index,int pcindex, volatile uint8_t& maskReg) :
	portInputReg(*portInputRegister(index)),
	portPCMask(maskReg),
	PCICRbit(1 << pcindex),
	portRisingPins(0),
	portFallingPins(0),
	firstPin(NULL)
#ifdef PINMODE
	,intrCount(0)
#endif
	{
		#ifdef FLASH
		ledsetup();
		#endif
	}
	volatile	uint8_t&		portInputReg;
	static		int8_t attachInterrupt(uint8_t pin, PCIntvoidFuncPtr userFunc, int mode);
	static		void detachInterrupt(uint8_t pin);
	INLINE_PCINT void PCint();
	static volatile uint8_t curr;
	#ifndef NO_PIN_NUMBER
	static	volatile uint8_t	arduinoPin;
	#endif
	#ifndef NO_PIN_STATE
	static volatile	uint8_t	pinState;
	#endif
	#ifdef PINMODE
	static volatile uint8_t pinmode;
	static volatile uint8_t s_portRisingPins;
	static volatile uint8_t s_portFallingPins;
	static volatile uint8_t s_lastPinView;
	static volatile uint8_t s_pmask;
	static volatile char s_PORT;
	static volatile uint8_t s_changedPins;
	static volatile uint8_t s_portRisingPins_nCurr;
	static volatile uint8_t s_portFallingPins_nNCurr;
	static volatile uint8_t s_currXORlastPinView;
	volatile uint8_t intrCount;
	static volatile uint8_t s_count;
	static volatile uint8_t pcint_multi;
	static volatile uint8_t PCIFRbug;
	#endif
	#ifdef FLASH
	static void ledsetup(void);
	#endif

protected:
	class PCintPin {
	public:
		PCintPin() :
		PCintFunc((PCIntvoidFuncPtr)NULL),
		mode(0) {}
		PCIntvoidFuncPtr PCintFunc;
		uint8_t 	mode;
		uint8_t		mask;
		uint8_t arduinoPin;
		PCintPin* next;
	};
	void 		enable(PCintPin* pin, PCIntvoidFuncPtr userFunc, uint8_t mode);
	int8_t		addPin(uint8_t arduinoPin,PCIntvoidFuncPtr userFunc, uint8_t mode);
	volatile	uint8_t&		portPCMask;
	const		uint8_t			PCICRbit;
	volatile	uint8_t			portRisingPins;
	volatile	uint8_t			portFallingPins;
	volatile uint8_t		lastPinView;
	PCintPin*	firstPin;
};

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);


#endif // #ifndef PinChangeInt_h *******************************************************************
