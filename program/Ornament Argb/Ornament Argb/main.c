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
#include <stdbool.h>

// For setting individual bits
#define sbi(x,b) (x) |= 1<<(b)
#define cbi(x,b) (x) &= ~(1<<(b))

#define LED_PIN 6
#define SWITCH_PIN PCINT5
#define STRIP_LEN 10
#define NUM_PROGRAMS 2

// Order is important! colors are sent as g, r, b
typedef struct Color {
	unsigned char g;
	unsigned char r;
	unsigned char b;
} Color;

const Color red = { .r = 128, .g = 0, .b = 0 };
const Color white = { .r = 128, .g = 128, .b = 128 };
const Color gold = { .r = 255, .g = 255, .b = 0 };
const Color green = { .r = 0, .g = 255, .b = 0 };

static Color strip[STRIP_LEN];
static int program = 0;
static bool press_detected = false;

extern void output_grb(uint8_t * ptr, uint16_t count);
extern void reset();

inline void setup() {
	// Set LED Pin as output
	sbi(DDRB, LED_PIN);
	// Set program change switch pin as input and engage pull-up resistor
	cbi(DDRB, SWITCH_PIN);
	// Enable pull-up resistor for input
	sbi(PORTB, SWITCH_PIN);
	
	//sbi(PCMSK0, SWITCH_PIN);
	//sbi(PCIFR, 0);
	//sbi(PCICR, 0);
	
	
	
	sei();
	
}

inline void show(Color * colors, int colors_len) {
	output_grb((uint8_t*) colors, colors_len * sizeof(Color));
	reset();
}

inline void set_solid(Color c, Color * colors, int colors_len) {
	for (int i = 0; i < colors_len; i++) {
		colors[i] = c;
	}
}

void candy_cane() {
	strip[STRIP_LEN - 1] = gold;
	set_solid(red, strip, STRIP_LEN - 1);
	show(strip, STRIP_LEN);
	_delay_ms(1000);
	set_solid(white, strip, STRIP_LEN - 1);
	show(strip, STRIP_LEN);
	_delay_ms(1000);
}

void solid_green() {
	strip[STRIP_LEN - 1] = gold;
	set_solid(green, strip, STRIP_LEN - 1);
	show(strip, STRIP_LEN);
	_delay_ms(1000);
}

void solid_white() {
	strip[STRIP_LEN - 1] = gold;
	set_solid(white, strip, STRIP_LEN - 1);
	show(strip, STRIP_LEN);
	_delay_ms(1000);
}

int main(void) {
	
	// Initialize IO
	setup();
	
	// Set colors to all zero
	memset(strip, 0, sizeof(Color) * STRIP_LEN);
	
	while (1) {
		if ((PINB & (1 << (SWITCH_PIN))) == 0) {
			if (!press_detected) {
				press_detected = true;
				program++;
				program %= NUM_PROGRAMS;
			}
		}
		else {
			press_detected = false;
		}
		
		switch (program) {
			case 0:
				candy_cane();	
				break;
			case 1:
				solid_green();
				break;
			default:
				solid_white();
				break;
		}
	}
}