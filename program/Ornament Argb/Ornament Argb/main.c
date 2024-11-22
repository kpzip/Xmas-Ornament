/*
 * Ornament Argb.c
 *
 * Created: 11/21/2024 11:37:21 PM
 * Author : kpzip
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


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
	asm("nop")
	sei
}

