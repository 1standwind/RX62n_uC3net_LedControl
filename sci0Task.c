/*
 * �V���A���ʐM�^�X�N: sci0Task
 *       void sci0Task(VP_INT exinf);
 * 
 * - �V���A���f�[�^�̓������[�v�[���Ɋi�[����̂ŁA�Œ蒷�������v�[���@
 *   ID_SCI0MPF ���K�v�B ID_SCI0MPF �̃f�[�^�u���b�N�T�C�Y�́A
 *   sizeof(T_SCI0MPF) �BT_SCI0MPF�́Asci0mpf.h�ɂĒ�`�̂��߁A�R���t�B�M��
 *   ���[�V�����t�@�C���u�J�[�l���S�ʁv�́u�w�b�_�t�@�C���v��sci0mpf.h��ǉ�
 *   �̂���
 */
#include "kernel.h"
#include "kernel_id.h"
#include "hw_dep.h"
#include <stdio.h>
#include <string.h>

#include "sci0Mpf.h"

#define MAX_WORDS 5

/* �ʐM�p�����[�^ */
const T_COM_SMOD com_ini = {115200, BLEN8, PAR_NONE, SBIT1, FLW_NONE};

/* �V���A���o�͗p������ */
const VB demo_str[] = "\r\n<<< Serial Comunication Program. >>>\r\n";
const VB ok[] = "ok\n\r";
const VB usage[] = "Usage: LED {number|ALL} {ON|OFF|TOGGLE}.\r\n";
const VB prompt[] = "$ ";

/**********************************************************************
 * static int toupper(int ch);
 * -------------------------------------------------------------------- 
 * �@�\�F�A���t�@�x�b�g��������啶���ɕϊ�����
 * �����Fint ch - �ϊ����镶��
 * �ߒl�Fint    - �ϊ���̕���
 * ���F
 * �⑫�F
 **********************************************************************/
static int toupper(int ch)
{
	if( ch>='a' && ch<='z' ){
		ch = ch-'a'+'A';
	}
	return ch;
}

/**********************************************************************
 * int ledChangeStatus( char * d, int s );
 * -------------------------------------------------------------------- 
 * �@�\�F������̎w���ɏ]����LED�̓_����Ԃ�ύX����
 * �����Fchar * d - �x���̊i�[���ꂽ������ւ̃A�h���X
 *       int s    - ������̒���
 * �ߒl�Fint      - ����������ߌ��ʁi0:����I���A-1:�G���[����)
 * ���F
 * �⑫�F������� '\0' �ŏI���̂Œʏ�͂����ڈ�ɏ������邪�A�{�֐���
 *       s�ŗ^����ꂽ�ꏊ��������̏I�[�Ƃ��ē��삷��B
 **********************************************************************/
int ledChangeStatus( char * d, int s )
{
	char * words[MAX_WORDS];        /* �P��L�^�p�̕�����|�C���^�z�� */
	char delim[] = " \t\n\r";       /* �P��Ԃ̋�؂蕶�� */
	char * p;
	int i;
	unsigned char bit;
	
	/* �A���t�@�x�b�g��啶���ɕϊ� */
	for ( i=0; i<s; i++ ){
		d[i] = (char)toupper( (int)d[i] );
	}

	/* �P��L�^�ꏊ�������� */
	for ( i=0; i<MAX_WORDS; i++ ){
		words[i] = NULL;
	}
	
	/* �P��̐擪�A�h���X���L�^ */
	for ( i=0, p=strtok( d, delim ); i<MAX_WORDS && p!= NULL; p=strtok( NULL, delim ), i++ ){
			words[i] = p;
	}
	
	/* ���͂���� */
	if ( strcmp( words[0], "LED" ) == 0 ){  /* �擪�̒P�ꂪLED */
		/* 2�Ԗڂ̒P�����͂��ď����r�b�g������ */
		if ( strcmp( words[1], "ALL" )==0 ){
			bit = 0xff;
		} else {
			if ( *words[1] <'0' || *words[1] >'7' ){
				return -1;
			}else{
				bit = 0x1 << (*words[1] - '0');
			}
		} 
			
		/* 3�Ԗڂ̒P�����͂��ď��� */
		if ( strcmp( words[2], "ON" ) == 0 ){
			ledSet( bit );
		}else if ( strcmp( words[2], "OFF" ) == 0 ){
			ledClear( bit );
		}else if ( strcmp( words[2], "TOGGLE" ) == 0 ){
			ledToggle( bit );    
		}else {
			return -1;
		}
		return 0;
	}else{
		return -1;
	}
}


/*******************************
        sci0Task
 *******************************/
void sci0Task(VP_INT exinf)
{
	UINT size;
	T_SCI0MPF * cmd, * msg;
	UINT cnt;
  
    ini_com(DID_UART0, &com_ini);
    dly_tsk(1);
    ctr_com(DID_UART0, STA_COM, 0);

    cnt = strlen(demo_str);
    puts_com(DID_UART0, (VB *)demo_str, &cnt, TMO_FEVR);
    ctr_com(DID_UART0,CLN_TXBUF,200);  /* ���M�o�b�t�@�̑��o�҂� */
 
	while(1){
		/* �v�����v�g���o */
		size = 2;
		puts_com(DID_UART0, (VB *)prompt, &size, TMO_FEVR);
		ctr_com(DID_UART0,CLN_TXBUF,200);  
		
		/* �V���A�����͗p�o�b�t�@�̊m�ۂƓ��� */
		get_mpf( ID_SCI0MPF, (VP*)&cmd ); 
		size = MSG_DATA_SIZE;
        gets_com(DID_UART0, (VB *)cmd->data, (UB *)0, '\r', &size, TMO_FEVR );
		putc_com(DID_UART0, '\r', TMO_FEVR);
		putc_com(DID_UART0, '\n', TMO_FEVR);
		
		/* �X�e�[�^�X�o�͗p�o�b�t�@�̊m�� */
		get_mpf( ID_SCI0MPF, (VP*)&msg );
		
		/* ���͕�����������A�󋵂ɉ����ă��b�Z�[�W���o�� */
		if ( ledChangeStatus( cmd->data, size ) == 0 ){
			strncpy( msg->data, ok, strlen(ok));
			size = strlen( ok );
		} else {
			strncpy ( msg->data, usage, strlen( usage) );
			size = strlen( usage );			
		}
		puts_com(DID_UART0, (VB *)msg->data, &size, TMO_FEVR);
		ctr_com(DID_UART0,CLN_TXBUF,200);  
		
		/* �o�b�t�@�̉�� */
		rel_mpf( ID_SCI0MPF, msg );
		rel_mpf( ID_SCI0MPF, cmd );
    }
}

/* }}UC3_TASK */
