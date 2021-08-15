# include "iodefine.h"
#include "sw.h"

void swInit(void){
	PORTD.DDR.BYTE = 0;
	PORTD.ICR.BYTE = 0xff;
	PORTD.PCR.BYTE = 0;
}

unsigned char swGet(void){
	unsigned char valc;

	valc = PORTD.PORT.BYTE;
	return ~valc;
}