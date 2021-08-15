/***********************************************************************
  Sample program


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

extern TMO LedTmo;

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
        MainTask
 *******************************/
extern ER net_setup(void);
/* {{UC3_TASK(MainTask) */
void MainTask(VP_INT exinf)
{
    ER ercd;
    
    act_tsk(ID_SCI0TASK);
    act_tsk(ID_SWCTASK);
    
    ledInit();
    swInit();
    
    /* Initialize uNet3 */
    ercd = net_setup();
    if (ercd != E_OK) {
        return;
    }
    PORTE.DR.BIT.B0 = 1;
    for (;;) {
	dly_tsk(10);
    }
}
/* }}UC3_TASK */


/* }}UC3_CODE */


/* {{UNET3_CODE */
/*******************************
  CGI Script
 ******************************/
/*
void sample_fnc(T_HTTP_SERVER *http)
{
}
*/

/* }}UNET3_CODE */


/*******************************
        Main entry
 *******************************/

int main(void)
{
    ER ret = E_OK;

    /* Initialize hardware */
    init_peripheral();

    PORTE.DDR.BIT.B0 = 1;           /* LED(LD2) */
    
    PORTA.ICR.BIT.B3 = 1;           /* Ether-In: MDIO */
    PORTB.ICR.BYTE = 0x8f;          /* Ether-In: RMII_RXD1, _RXD0, _REF50CK, _RX_ER, _CRS_DV */
    IOPORT.PFENET.BYTE = 0x80;      /* PHY=RMII */
    
    /* Generate system and start */
    ret = start_uC3();

    return ret;
}

/* end */
