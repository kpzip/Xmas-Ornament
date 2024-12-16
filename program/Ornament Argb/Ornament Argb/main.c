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
#define NUM_PROGRAMS 6

// Order is important! colors are sent as g, r, b
typedef struct Color {
	unsigned char g;
	unsigned char r;
	unsigned char b;
} Color;

typedef struct xorshift32_state {
	uint32_t a;
} xorshift32_state;

const Color red = { .r = 128, .g = 0, .b = 0 };
const Color white = { .r = 128, .g = 128, .b = 128 };
const Color gold = { .r = 255, .g = 255, .b = 0 };
const Color green = { .r = 0, .g = 255, .b = 0 };
const Color lush_green = { .r = 50, .g = 168, .b = 82 };
const Color off = { .r = 0, .g = 0, .b = 0 };

static Color strip[STRIP_LEN];
static int program = 0;
static bool press_detected = false;

/* The state must be initialized to non-zero */
static xorshift32_state randomstate = { .a = 0x75AE3D9A };

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

uint32_t xorshift32() {
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = randomstate.a;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return randomstate.a = x;
}

bool randbool() {
	return xorshift32() % 2 == 0;
}

Color hsv2rgb(float H, float S, float V) {
	float r = 0, g = 0, b = 0;
	
	float h = H / 360;
	float s = S / 100;
	float v = V / 100;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}
	
	Color color;
	color.r = r * 255;
	color.g = g * 255;
	color.b = b * 255;
	
	return color;
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
	for (int i = 0; i < STRIP_LEN - 1; i++) {
		strip[i] = randbool() ? red : white;
	}
	show(strip, STRIP_LEN);
	_delay_ms(500);
}

void solid_green() {
	strip[STRIP_LEN - 1] = gold;
	set_solid(green, strip, STRIP_LEN - 1);
	show(strip, STRIP_LEN);
	_delay_ms(250);
}

void solid_white() {
	strip[STRIP_LEN - 1] = gold;
	set_solid(white, strip, STRIP_LEN - 1);
	show(strip, STRIP_LEN);
	_delay_ms(250);
}

static int white_wave_state = 0;

#define STAR_NUM_BLINKS 3

void white_wave() {
	for (int i = 0; i < STRIP_LEN; i++) {
		if (i == (STRIP_LEN - 1) && white_wave_state >= (STRIP_LEN - 1)) {
			strip[i] = white_wave_state % 2 == 0 ? off : gold;
		}
		else if (i == white_wave_state) {
			strip[i] = white;
		}
		else {
			strip[i] = off;
		}
	}
	show(strip, STRIP_LEN);
	_delay_ms(250);
	white_wave_state++;
	white_wave_state %= (STRIP_LEN + STAR_NUM_BLINKS * 2 - 1);
}

static bool green_red_state = false;

void green_red() {
	strip[STRIP_LEN - 1] = gold;
	for (int i = 0; i < STRIP_LEN - 1; i++) {
		if ((i + green_red_state) % 2 == 0) {
			strip[i] = green;
		}
		else {
			strip[i] = red;
		}
	}
	show(strip, STRIP_LEN);
	_delay_ms(250);
	green_red_state = !green_red_state;
}

static float hue_val = 0.0;

#define SATURATION 100.0f
#define INTENSITY 75.0f

void rgb() {
	strip[STRIP_LEN - 1] = gold;
	Color c = hsv2rgb(hue_val, SATURATION, INTENSITY);
	for (int i = 0; i < STRIP_LEN - 1; i++) {
		strip[i] = c;
	}
	show(strip, STRIP_LEN);
	_delay_ms(25);
	hue_val += 1;
	hue_val = fmod(hue_val, 360);
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
			case 2:
				solid_white();
				break;
			case 3:
				white_wave();
				break;
			case 4:
				green_red();
				break;
			case 5:
				rgb();
				break;
			default:
				solid_white();
				break;
		}
	}
}