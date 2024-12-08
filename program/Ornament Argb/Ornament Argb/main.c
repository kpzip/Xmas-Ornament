/*
 * Ornament Argb.c
 *
 * Created: 11/21/2024 11:37:21 PM
 * Author : kpzip
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define nop asm("nop");
#define sbi(x,b) (x) |= 1<<(b)
#define cbi(x,b) (x) &= ~(1<<(b))

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

typedef struct Color {
	unsigned char r, g, b;
};

void show(Color* colors, int colors_len) {
	cli
	nop
	sei
}

inline void writeOne() {
	// Might want to use hardware timers instead of nops in the future
	// T1H = 0.6 +/- 0.15 uS -> 10 clock cycles
	// T1L = 0.6 +/- 0.15 uS -> 10 clock cycles
	
	// Set high - Low is default
	sbi(DDRB, 5)
	
	cli
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	nop
	nop
	
	// Set low
	cbi(DDRB, 5)
	
	//nop
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	nop
	sei
}

