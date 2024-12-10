/*
 * Ornament Argb.c
 *
 * Created: 11/21/2024 11:37:21 PM
 * Author : kpzip
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>
#include <string.h>

// For setting individual bits
#define sbi(x,b) (x) |= 1<<(b)
#define cbi(x,b) (x) &= ~(1<<(b))

#define LED_PIN 6
#define SWITCH_PIN 5
#define STRIP_LEN 10

// Order is important! colors are sent as g, r, b
typedef struct Color {
	unsigned char g;
	unsigned char r;
	unsigned char b;
} Color;

static Color strip[STRIP_LEN];

extern void output_grb(uint8_t * ptr, uint16_t count);
extern void reset();

inline void setup() {
	// Set LED Pin as output
	sbi(DDRB, LED_PIN);
	// Set program change switch pin as input and engage pull-up resistor
	cbi(DDRB, SWITCH_PIN);
	//
	
}

inline void show(Color *colors, int colors_len) {
	output_grb((uint8_t*) colors, colors_len * sizeof(Color));
	reset();
}

void set_solid(Color c) {
	for (int i = 0; i < STRIP_LEN; i++) {
		strip[i] = c;
	}
}

int main(void)
{
	setup();
	
	// Set colors to all zero
	memset(strip, 0, sizeof(Color) * STRIP_LEN);
	
	
	Color red = { .r = 128, .g = 0, .b = 0 };
	Color white = { .r = 128, .g = 128, .b = 128 };
	
	while (1)
	{
		set_solid(red);
		show(strip, STRIP_LEN);
		_delay_ms(1000);
		set_solid(white);
		show(strip, STRIP_LEN);
		_delay_ms(1000);
	}
}