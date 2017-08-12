////////////////////////
#define F_CPU 1000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <util/delay.h>               



volatile int  counter = 0;
volatile uint8_t portbhistory = 0xFF; 


static inline void initInterrupt(void)
{
	GIMSK |= (1 << PCIE);   // pin change interrupt enable
	PCMSK |= (1 << PCINT4); // pin change interrupt enabled for PCINT4
	PCMSK |= (1 << PCINT3); // pin change interrupt enabled for PCINT3
	sei();                  // enable interrupts
}



int main(void)
{
	PORTB  =  (1 << PB4) | (1 << PB3); 
	DDRB = 0x07;
	initInterrupt();
	_NOP();
	while(1) {
		_delay_ms(500);
		if(counter > 0) {
			PORTB ^= ( 1<< PORTB1);
			--counter;
		} else if( counter == 0) {
			PORTB = (0 << PB1) ;
		}
	}
	return 0; 
}

ISR(PCINT0_vect)
{
	// This is going to be executed by 
	// all the PCINT interrupts. 
	cli();
	PORTB ^= (1 << PB0) | (1 << PB2);
	uint8_t changedbits;
	changedbits = PINB ^ portbhistory;
	portbhistory = PINB;

	if(changedbits & (1 << PINB3))
	{
		/* PCINT0 changed */
		counter = 10;
	}

	if(changedbits & (1 << PINB4))
	{
		counter = 20;
	}


	_delay_ms(1000);
	sei();
}

////////////////////////
