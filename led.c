#include "iodefine.h"
#include "led.h"

void ledInit(void)
{
	PORTE.DR.BYTE = 0xff;
	PORTE.DDR.BYTE = 0xff;
}

void ledPut( unsigned char v )
{
	PORTE.DR.BYTE = ~v;
}

unsigned char ledGet( void )
{
	unsigned char v = PORTE.DR.BYTE;
	
	return ~v;
}
