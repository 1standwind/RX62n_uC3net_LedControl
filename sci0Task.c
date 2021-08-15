/*
 * シリアル通信タスク: sci0Task
 *       void sci0Task(VP_INT exinf);
 * 
 * - シリアルデータはメモリープールに格納するので、固定長メモリプール　
 *   ID_SCI0MPF が必要。 ID_SCI0MPF のデータブロックサイズは、
 *   sizeof(T_SCI0MPF) 。T_SCI0MPFは、sci0mpf.hにて定義のため、コンフィギュ
 *   レーションファイル「カーネル全般」の「ヘッダファイル」にsci0mpf.hを追加
 *   のこと
 */
#include "kernel.h"
#include "kernel_id.h"
#include "hw_dep.h"
#include <stdio.h>
#include <string.h>

#include "sci0Mpf.h"

#define MAX_WORDS 5

/* 通信パラメータ */
const T_COM_SMOD com_ini = {115200, BLEN8, PAR_NONE, SBIT1, FLW_NONE};

/* シリアル出力用文字列 */
const VB demo_str[] = "\r\n<<< Serial Comunication Program. >>>\r\n";
const VB ok[] = "ok\n\r";
const VB usage[] = "Usage: LED {number|ALL} {ON|OFF|TOGGLE}.\r\n";
const VB prompt[] = "$ ";

/**********************************************************************
 * static int toupper(int ch);
 * -------------------------------------------------------------------- 
 * 機能：アルファベット小文字を大文字に変換する
 * 引数：int ch - 変換する文字
 * 戻値：int    - 変換後の文字
 * 大域：
 * 補足：
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
 * 機能：文字列の指示に従ってLEDの点灯状態を変更する
 * 引数：char * d - 支持の格納された文字列へのアドレス
 *       int s    - 文字列の長さ
 * 戻値：int      - 文字列を解釈結果（0:正常終了、-1:エラー発生)
 * 大域：
 * 補足：文字列は '\0' で終わるので通常はこれを目印に処理するが、本関数は
 *       sで与えられた場所が文字列の終端として動作する。
 **********************************************************************/
int ledChangeStatus( char * d, int s )
{
	char * words[MAX_WORDS];        /* 単語記録用の文字列ポインタ配列 */
	char delim[] = " \t\n\r";       /* 単語間の区切り文字 */
	char * p;
	int i;
	unsigned char bit;
	
	/* アルファベットを大文字に変換 */
	for ( i=0; i<s; i++ ){
		d[i] = (char)toupper( (int)d[i] );
	}

	/* 単語記録場所を初期化 */
	for ( i=0; i<MAX_WORDS; i++ ){
		words[i] = NULL;
	}
	
	/* 単語の先頭アドレスを記録 */
	for ( i=0, p=strtok( d, delim ); i<MAX_WORDS && p!= NULL; p=strtok( NULL, delim ), i++ ){
			words[i] = p;
	}
	
	/* 文章を解析 */
	if ( strcmp( words[0], "LED" ) == 0 ){  /* 先頭の単語がLED */
		/* 2番目の単語を解析して処理ビットを決定 */
		if ( strcmp( words[1], "ALL" )==0 ){
			bit = 0xff;
		} else {
			if ( *words[1] <'0' || *words[1] >'7' ){
				return -1;
			}else{
				bit = 0x1 << (*words[1] - '0');
			}
		} 
			
		/* 3番目の単語を解析して処理 */
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
    ctr_com(DID_UART0,CLN_TXBUF,200);  /* 送信バッファの送出待ち */
 
	while(1){
		/* プロンプト送出 */
		size = 2;
		puts_com(DID_UART0, (VB *)prompt, &size, TMO_FEVR);
		ctr_com(DID_UART0,CLN_TXBUF,200);  
		
		/* シリアル入力用バッファの確保と入力 */
		get_mpf( ID_SCI0MPF, (VP*)&cmd ); 
		size = MSG_DATA_SIZE;
        gets_com(DID_UART0, (VB *)cmd->data, (UB *)0, '\r', &size, TMO_FEVR );
		putc_com(DID_UART0, '\r', TMO_FEVR);
		putc_com(DID_UART0, '\n', TMO_FEVR);
		
		/* ステータス出力用バッファの確保 */
		get_mpf( ID_SCI0MPF, (VP*)&msg );
		
		/* 入力文字列を処理、状況に応じてメッセージを出力 */
		if ( ledChangeStatus( cmd->data, size ) == 0 ){
			strncpy( msg->data, ok, strlen(ok));
			size = strlen( ok );
		} else {
			strncpy ( msg->data, usage, strlen( usage) );
			size = strlen( usage );			
		}
		puts_com(DID_UART0, (VB *)msg->data, &size, TMO_FEVR);
		ctr_com(DID_UART0,CLN_TXBUF,200);  
		
		/* バッファの解放 */
		rel_mpf( ID_SCI0MPF, msg );
		rel_mpf( ID_SCI0MPF, cmd );
    }
}

/* }}UC3_TASK */
