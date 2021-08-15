/***********************************************************************
    Kernel configuration

 ***********************************************************************/

#include "kernel.h"
#include "sci0Mpf.h"

/*******************************
    システム スタック サイズ
 *******************************/

#pragma stacksize su = 0x400      /* Time Event Handler */
#pragma stacksize si = 0x400      /* Isr Service Routine */

extern void _ddr_init(void);

/*******************************
  Inner initialize function
 *******************************/
void _kernel_initial(void)
{
    _ddr_init();
}

/*******************************
  Various management table
 *******************************/

UB const _kernel_atrtbl[] = {
0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x42, 
0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x50, 0x50, 0x50, 
0x50, 0x50, 0x62, 0x62, 0x60, 0x90, 0x90, 0xa0, 0xa0, 0xa0, 
0x32, };

extern void MainTask(VP_INT exinf);
int _kernel_task1_stk[0x200/sizeof(int)];
T_CTSK const _kernel_task1 = {0,
            (FP)MainTask, 0x200, (VP)_kernel_task1_stk, 0, 1};

extern void dev_snd_tsk(VP_INT exinf);
int _kernel_task2_stk[0x200/sizeof(int)];
T_CTSK const _kernel_task2 = {0,
            (FP)dev_snd_tsk, 0x200, (VP)_kernel_task2_stk, 0, 4};

extern void dev_rcv_tsk(VP_INT exinf);
int _kernel_task3_stk[0x300/sizeof(int)];
T_CTSK const _kernel_task3 = {0,
            (FP)dev_rcv_tsk, 0x300, (VP)_kernel_task3_stk, 0, 4};

extern void dev_ctl_tsk(VP_INT exinf);
int _kernel_task4_stk[0x200/sizeof(int)];
T_CTSK const _kernel_task4 = {0,
            (FP)dev_ctl_tsk, 0x200, (VP)_kernel_task4_stk, 0, 4};

extern void net_tim_tsk(VP_INT exinf);
int _kernel_task5_stk[0x200/sizeof(int)];
T_CTSK const _kernel_task5 = {0,
            (FP)net_tim_tsk, 0x200, (VP)_kernel_task5_stk, 0, 4};

extern void sci0Task(VP_INT exinf);
int _kernel_task6_stk[0x100/sizeof(int)];
T_CTSK const _kernel_task6 = {0,
            (FP)sci0Task, 0x100, (VP)_kernel_task6_stk, 0, 4};

extern void swCtrlTask(VP_INT exinf);
int _kernel_task7_stk[0x100/sizeof(int)];
T_CTSK const _kernel_task7 = {0,
            (FP)swCtrlTask, 0x100, (VP)_kernel_task7_stk, 0, 2};

extern void httpd_tsk1(VP_INT exinf);
int _kernel_task8_stk[0x300/sizeof(int)];
T_CTSK const _kernel_task8 = {0,
            (FP)httpd_tsk1, 0x300, (VP)_kernel_task8_stk, 0, 4};

T_CSEM const _kernel_sem1 = {1, 1};

T_CSEM const _kernel_sem2 = {0, 1};

T_CSEM const _kernel_sem3 = {1, 1};

T_CSEM const _kernel_sem4 = {0, 1};

T_CSEM const _kernel_sem5 = {0, 1};

T_CFLG const _kernel_flg1 = {0x0};

T_CFLG const _kernel_flg2 = {0x0};

T_CFLG const _kernel_flg3 = {0x0};

long long _kernel_mpf1_buf[(2*256)/sizeof(long long)];
T_CMPF const _kernel_mpf1 = {2, 256, (VP)_kernel_mpf1_buf};

long long _kernel_mpf2_buf[(5*(sizeof(T_SCI0MPF)))/sizeof(long long)];
T_CMPF const _kernel_mpf2 = {5, (sizeof(T_SCI0MPF)), (VP)_kernel_mpf2_buf};

long long _kernel_mpf3_buf[(8*1576)/sizeof(long long)];
T_CMPF const _kernel_mpf3 = {8, 1576, (VP)_kernel_mpf3_buf};

extern void swReader(VP_INT exinf);
T_CCYC const _kernel_cyc1 = {0, (FP)swReader, 10, 0};

void const * const _kernel_inftbl[] = {
(void const *)&_kernel_task1,
(void const *)&_kernel_task2,
(void const *)&_kernel_task3,
(void const *)&_kernel_task4,
(void const *)&_kernel_task5,
(void const *)&_kernel_task6,
(void const *)&_kernel_task7,
(void const *)&_kernel_task8,
(void const *)&_kernel_sem1,
(void const *)&_kernel_sem2,
(void const *)&_kernel_sem3,
(void const *)&_kernel_sem4,
(void const *)&_kernel_sem5,
(void const *)&_kernel_flg1,
(void const *)&_kernel_flg2,
(void const *)&_kernel_flg3,
0,
0,
(void const *)&_kernel_mpf1,
(void const *)&_kernel_mpf2,
(void const *)&_kernel_mpf3,
(void const *)&_kernel_cyc1,
};

T_TCB _kernel_tcb[8];
UB _kernel_sem[5];
FLGPTN _kernel_flg[3];
T_MBX _kernel_mbx[2];
T_MPF _kernel_mpf[3];
T_CYC _kernel_cyc[1];
T_WTID _kernel_waique[30];

VB const * const _kernel_objname[] = {
(VB const *)0,
(VB const *)0,
(VB const *)0,
(VB const *)0,
(VB const *)0,
(VB const *)0,
(VB const *)0,
(VB const *)0,
(VB const *)0,
"ID_NET_MAIN_TSK",
"ID_ETH_SND_TSK",
"ID_ETH_RCV_TSK",
"ID_ETH_CTL_TSK",
"ID_TCP_TIM_TSK",
"ID_SCI0TASK",
"ID_SWCTASK",
"ID_HTTPD_TSK1",
"ID_TCP_SEM",
"ID_UART0_RESM",
"ID_LEDSEM",
"ID_UART0_TSEM",
"ID_UART0_RSEM",
"ID_ETH_RCV_FLG",
"ID_ETH_SND_FLG",
"ID_SWFLG",
"ID_ETH_SND_MBX",
"ID_ETH_RCV_MBX",
"ID_CONTENTS_MPF",
"ID_SCI0MPF",
"ID_TCP_MPF",
"ID_SWCYC",
};

VP const _kernel_ctrtbl[] = {
(VP)&_kernel_tcb[0],
(VP)&_kernel_tcb[1],
(VP)&_kernel_tcb[2],
(VP)&_kernel_tcb[3],
(VP)&_kernel_tcb[4],
(VP)&_kernel_tcb[5],
(VP)&_kernel_tcb[6],
(VP)&_kernel_tcb[7],
(VP)&_kernel_sem[0],
(VP)&_kernel_sem[1],
(VP)&_kernel_sem[2],
(VP)&_kernel_sem[3],
(VP)&_kernel_sem[4],
(VP)&_kernel_flg[0],
(VP)&_kernel_flg[1],
(VP)&_kernel_flg[2],
(VP)&_kernel_mbx[0],
(VP)&_kernel_mbx[1],
(VP)&_kernel_mpf[0],
(VP)&_kernel_mpf[1],
(VP)&_kernel_mpf[2],
(VP)&_kernel_cyc[0],
};

T_CNSTBL const _kernel_cnstbl = {
_kernel_atrtbl,
_kernel_inftbl-9,
0,
0,
_kernel_waique,
_kernel_ctrtbl-9,
_kernel_objname,
TKERNEL_PRID,
TKERNEL_PRVER,
1,
8,
30,
15         /* Kernel Level */
};

/* end */
