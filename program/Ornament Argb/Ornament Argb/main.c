/*
 * Ornament Argb.c
 *
 * Created: 11/21/2024 11:37:21 PM
 * Author : kpzip
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define nop asm("nop");

// IMPORTANT: These should compile to one instruction for timing purposes. Luckily the avr gcc makers knew what they were doing!
#define sbi(x,b) (x) |= 1<<(b)
#define cbi(x,b) (x) &= ~(1<<(b))

#define LED_PIN 6
#define SWITCH_PIN 5

typedef struct Color {
	unsigned char r, g, b;
} Color;

static Color strip[10];

int main(void)
{
	writeAllZeros();
	writeAllOnes();
	writeAllZeros();
    /* Replace with your application code */
    while (1) 
    {
		nop
    }
}

void writeAllOnes() {
	writeOne();
	writeOne();
	writeOne();
	writeOne();
	writeOne();
	writeOne();
	writeOne();
	writeOne();
}

void writeAllZeros() {
	writeZero();
	writeZero();
	writeZero();
	writeZero();
	writeZero();
	writeZero();
	writeZero();
	writeZero();
}

void show(Color* colors, int colors_len) {
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
	sbi(DDRB, 5);
	
	nop
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
	cbi(DDRB, LED_PIN);
	
	//nop
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	nop
	nop
	
	// Leave it as low
}

// Interrupts should be disabled before calling!
inline void writeZero() {
	// Might want to use hardware timers instead of nops in the future
	// T0H = 0.3 +/- 0.15 uS -> 5 clock cycles
	// T0L = 0.9 +/- 0.15 uS -> 15 clock cycles
	
	// Set high - Low is default
	sbi(DDRB, LED_PIN);
	
	nop
	nop
	nop
	nop
	nop
	
	// Set low
	cbi(DDRB, LED_PIN);
	
	//nop
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	nop
	nop
	
	nop
	nop
	nop
	nop
	nop
	
	// Leave it as low
}

// Send reset code
// Interrupts *can* be enabled since longer is fine
inline void resetCode() {
	cbi(DDRB, LED_PIN);
	for (int i = 0; i > 1000; i++) {
		nop
	}
}

