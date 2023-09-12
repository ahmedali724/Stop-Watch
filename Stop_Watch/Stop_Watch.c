/*
 ============================================================================
 File Name   : Stop_Watch.c
 Project     : Stop Watch (Mini Project 2)
 Author      : Ahmed Ali
 Date        : 12 Sept 2023
 ============================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char seconds;
unsigned char minutes;
unsigned char hours;

void TIMER1_CTC_Init(void) {
	/*
	 * F_CPU = 1Mhz, pre-scaler = 1024
	 * F_Timer = 977, Ttimer = 1.024msec
	 * 1 second = 1000 mile second
	 * for 1 second the compare match value will be 977
	 * and CTC mode
	 * */
	/*  force output compare for unit A (CTC) */
	TCCR1A = (1 << FOC1A);
	/* WGM12 = 1 => using mode 4 in timer1 (CTC) and compare match in OCR1A  */
	/* (CS10 = 1 && CS12 = 1) => pre-scaler 1024 */
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);
	/* start timer from 0 */
	TCNT1 = 0;
	/* set compare match value for 1 second */
	OCR1A = 977;
	/* enable output compare A match interrupt */
	TIMSK |= (1 << OCIE1A);
	/* enable global interrupt */
	SREG |= (1 << 7);
}

void INT0_Init_FalingEdge(void) {
	/* (ISC01 = 1 && ISC00 = 0) => falling edge configuration */
	MCUCR |= (1 << ISC01);
	/* external interrupt request 0 enable */
	GICR |= (1 << INT0);
	/* define PD2 as input */
	DDRD &= ~(1 << PD2);
	/* internal pull-up resistor for PD2 */
	PORTD |= (1 << PD2);
}

void INT1_Init_RisingEdge(void) {
	/* (ISC10 = 1 && ISC11 = 1) => rising edge configuration */
	MCUCR |= (1 << ISC10) | (1 << ISC11);
	/* external interrupt request 1 enable */
	GICR |= (1 << INT1);
	/* define PD3 as input */
	DDRD &= ~(1 << PD3);
}

void INT2_Init_FalingEdge(void) {
	/* (ISC2 = 0) => falling edge configuration */
	MCUCSR &= ~(1 << ISC2);
	/* external interrupt request 2 enable */
	GICR |= (1 << INT2);
	/* define PB2 as input */
	DDRB &= ~(1 << PB2);
	/* internal pull-up resistor for PB2 */
	PORTB |= (1 << PB2);
}

ISR(TIMER1_COMPA_vect) {
	/* every compare match interrupt that mean the the time increase by second */
	seconds++;
	/* 60 seconds equal to 1 minute */
	if (seconds == 60) {
		minutes++;
		seconds = 0;
	}
	/* 60 minutes equal to 1 hour */
	if (minutes == 60) {
		hours++;
		minutes = 0;
	}
	/* if the stop watch reach to 99 hour, it will be reset */
	if (hours == 99) {
		seconds = 0;
		minutes = 0;
		hours = 0;
	}
}

ISR(INT0_vect) {
	/* when external interrupt 0 triggered, the stop watch will be reset */
	TCNT1 = 0;
	seconds = 0;
	minutes = 0;
	hours = 0;
}

ISR(INT1_vect) {
	/* when external interrupt 1 triggered, the stop watch will be paused */
	/* the pause process is to stop the clock source for timer1 */
	TCCR1B &= (~(1 << CS10)) & (~(1 << CS12));
}

ISR(INT2_vect) {
	/* when external interrupt 2 triggered, the stop watch will be resumed */
	/* the resume process is to return the clock source for timer1 */
	TCCR1B |= (1 << CS10) | (1 << CS12);
}

int main(void) {
	/* configurations */
	/* define first 4-pins in PORTC as output for 7447 decoder */
	DDRC |= 0x0F;
	/* put 0 value as initial in first 4-pins in PORTC */
	PORTC &= 0xF0;
	/* define first 6-pins in PORTA as output as the enable/disable pins for the six 7-segments */
	DDRA |= 0x7F;
	/* put 0 value as initial in first 6-pins in PORTA */
	PORTA &= 0x80;
	/* call initialization functions */
	TIMER1_CTC_Init();
	INT0_Init_FalingEdge();
	INT1_Init_RisingEdge();
	INT2_Init_FalingEdge();
	while (1) {
		/* enable first 7 segment for first digit in seconds */
		PORTA = 0x20;
		/* put the value of first digit in seconds in first 4-pins in PORTC */
		PORTC = (PORTC & 0xF0) | ((seconds % 10) & 0x0F);
		/* small delay the show the number (multiplexing method) */
		_delay_ms(3);

		/* enable second 7 segment for second digit in seconds */
		PORTA = 0x10;
		/* put the value of second digit in seconds in first 4-pins in PORTC */
		PORTC = (PORTC & 0xF0) | ((seconds / 10) & 0x0F);
		/* small delay the show the number (multiplexing method) */
		_delay_ms(3);

		/* enable third 7 segment for first digit in minutes */
		PORTA = 0x08;
		/* put the value of first digit in minutes in first 4-pins in PORTC */
		PORTC = (PORTC & 0xF0) | ((minutes % 10) & 0x0F);
		/* small delay the show the number (multiplexing method) */
		_delay_ms(3);

		/* enable fourth 7 segment for second digit in minutes */
		PORTA = 0x04;
		/* put the value of second digit in minutes in first 4-pins in PORTC */
		PORTC = (PORTC & 0xF0) | ((minutes / 10) & 0x0F);
		/* small delay the show the number (multiplexing method) */
		_delay_ms(3);

		/* enable fifth 7 segment for first digit in hours */
		PORTA = 0x02;
		/* put the value of first digit in hours in first 4-pins in PORTC */
		PORTC = (PORTC & 0xF0) | ((hours % 10) & 0x0F);
		/* small delay the show the number (multiplexing method) */
		_delay_ms(3);

		/* enable sixth 7 segment for second digit in hours */
		PORTA = 0x01;
		/* put the value of second digit in hours in first 4-pins in PORTC */
		PORTC = (PORTC & 0xF0) | ((hours / 10) & 0x0F);
		/* small delay the show the number (multiplexing method) */
		_delay_ms(3);
	}
}
