/*
 * Ornament Argb.c
 *
 * Created: 11/21/2024 11:37:21 PM
 * Author : kpzip
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define nop __asm__ volatile("nop");

// IMPORTANT: These should compile to one instruction for timing purposes. Luckily the avr gcc makers knew what they were doing!
#define sbi(x,b) (x) |= 1<<(b)
#define cbi(x,b) (x) &= ~(1<<(b))

#define LED_PIN 6
#define SWITCH_PIN 5
#define STRIP_LEN 10

typedef struct Color {
	unsigned char r, g, b;
} Color;

static Color strip[STRIP_LEN];

int main(void)
{
	setup();
	memset(strip, 128, sizeof(Color) * STRIP_LEN);
	Color red = { .r = 128, .g = 0, .b = 0 };
	strip[0] = red;
	//writeByte(0);
	show(strip, 1);
    /* Replace with your application code */
    while (1) 
    {
		//sbi(PORTB, LED_PIN);
		//cbi(PORTB, LED_PIN);
		nop
	}
}

extern void output_grb(uint8_t * ptr, uint16_t count);

inline void setup() {
	// Set LED Pin as output
	sbi(DDRB, LED_PIN);
	// Set program change switch pin as input and engage pull-up resistor
	cbi(DDRB, SWITCH_PIN);
	//
	
}

void show(Color *colors, int colors_len) {
	cli();
	for (int i = 0; i < colors_len; i++) {
		writeByte(colors[i].g);
		writeByte(colors[i].r);
		writeByte(colors[i].b);
	}
	sei();
	resetCode();
}

// Interrupts should be disabled before calling!
inline void writeByte(unsigned char b) {
	for (int i = 7; i >= 0; i--) {
		if (b & (1 << (i)) == 0) {
			writeZero();
		}
		else {
			writeOne();
		}
	}
}

// Interrupts should be disabled before calling!
inline void writeOne() {
	// Might want to use hardware timers instead of nops in the future
	// T1H = 0.6 +/- 0.15 uS -> 10 clock cycles
	// T1L = 0.6 +/- 0.15 uS -> 10 clock cycles
	
	// Set high - Low is default
	sbi(PORTB, LED_PIN);
	
	nop
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	//nop
	//nop
	
	// Set low
	cbi(PORTB, LED_PIN);
	
	//nop
	nop
	nop
	//nop
	//nop
	
	//nop
	//nop
	//nop
	//nop
	//nop
	
	// Leave it as low
}

// Interrupts should be disabled before calling!
inline void writeZero() {
	// Might want to use hardware timers instead of nops in the future
	// T0H = 0.3 +/- 0.15 uS -> 5 clock cycles
	// T0L = 0.9 +/- 0.15 uS -> 15 clock cycles
	
	// Set high - Low is default
	sbi(PORTB, LED_PIN);
	
	nop
	nop
	nop
	//nop
	//nop
	
	// Set low
	cbi(PORTB, LED_PIN);
	
	//nop
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	//nop
	//nop
	
	//nop
	//nop
	//nop
	//nop
	//nop
	
	// Leave it as low
}

// Send reset code
// Interrupts *can* be enabled since longer is fine
inline void resetCode() {
	cbi(PORTB, LED_PIN);
	for (int i = 0; i > 1000; i++) {
		nop
	}
}

