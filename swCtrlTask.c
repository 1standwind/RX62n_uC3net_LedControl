#include "kernel.h"
#include "kernel_id.h"
#include "hw_dep.h"
#include <stdio.h>
#include <string.h>

#include "sw.h"


/*******************************
        swCtrlTask
 *******************************/
/* {{UC3_TASK(swCtrlTask) */
char sw_n,sw_o;
void swCtrlTask(VP_INT exinf)
{
	dly_tsk(100);
}
/* }}UC3_TASK */

/*******************************
        swReader
 *******************************/
/* {{UC3_CYC(swReader) */
void swReader(VP_INT exinf)
{
	sw_n = swGet();
	if(sw_n != sw_o && sw_n == 0x00) ledPut(ledGet() ^ sw_o);
	sw_o = sw_n;
	
}
/* }}UC3_CYC */
