/***********************************************************************
  Sample program

   Generated at 2020-09-16 11:54:35

 ***********************************************************************/

/* {{UC3_INCLUDE */
#include "kernel.h"
#include "kernel_id.h"
#include "hw_dep.h"
#include "net_hdr.h"
#include "net_id.h"
#include "http_server.h"
/* }}UC3_INCLUDE */

extern void rx_init_peripheral(void);
extern void init_peripheral(void);

/* {{UC3_CODE */


/******************************************************************
    Default interrupt handler
     Reached when the interrupt is accepted even though it is not the generation of an
     interrupt service routine also define the interrupt handler.
 ******************************************************************/

void int_abort(void)
{
    for(;;);
}

/*******************************
        swReader
 *******************************/
/* {{UC3_CYC(swReader) */
void swReader(VP_INT exinf)
{
}
/* }}UC3_CYC */

/*******************************
        MainTask
 *******************************/
extern ER net_setup(void);
/* {{UC3_TASK(MainTask) */
void MainTask(VP_INT exinf)
{
    ER ercd;

    /* Initialize uNet3 */
    ercd = net_setup();
    if (ercd != E_OK) {
        return;
    }

    for (;;) {
        /* TODO: */
        dly_tsk(1000);
    }
}
/* }}UC3_TASK */

/*******************************
        sci0Task
 *******************************/
/* {{UC3_TASK(sci0Task) */
void sci0Task(VP_INT exinf)
{
}
/* }}UC3_TASK */

/*******************************
        swCtrlTask
 *******************************/
/* {{UC3_TASK(swCtrlTask) */
void swCtrlTask(VP_INT exinf)
{
}
/* }}UC3_TASK */


/* }}UC3_CODE */


/* {{UNET3_CODE */
/*******************************
  CGI Script
 ******************************/
void ledCgi(T_HTTP_SERVER *http)
{
}
/* }}UNET3_CODE */


/*******************************
        Main entry
 *******************************/

int main(void)
{
    ER ret = E_OK;

    /* Initialize hardware */
    init_peripheral();

    /* Generate system and start */
    ret = start_uC3();

    return ret;
}

/* end */
