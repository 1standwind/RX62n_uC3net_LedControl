/**************************************************************************
* CGI Sample Program
**************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "kernel.h"
#include "net_hdr.h"
#include "net_strlib.h"

#include "http_server.h"
#include "dns_client.h"
#include "sntp_client.h"
#include "ping_client.h"

#define CONTENTS_SIZE  256 /* response contents size */

#ifdef NET_C
#include "kernel_id.h"
#include "net_id.h"
#else
ID ID_CONTENTS_MPF=0;
const T_CMPF c_contents_mpf = {TA_TFIFO, 2, CONTENTS_SIZE, "contents_mpf"};
#endif

TMO LedTmo = 1000;
static char tmp[16];

/*******************************
  CGI Script
 ******************************/
static void itoa_std(UW num, char* str)
{
    char c, *p, *q;

    p = q = str;

    /* Convert to ascii */
    do {
        c = num%10;
        *p++ = '0'+ c;
        num = num/10;
    } while(num);
    *p-- = '\0';
    
    /* Reverse the string */
    do {
       c = *p;
      *p = *q;
      *q = c;
      p--; q++;
    } while (q < p);
}

static ER get_contents_buf(char**buf)
{
    ER ercd;
    *buf = (char*)0;
    ercd = tget_mpf(ID_CONTENTS_MPF, (VP*)buf, TMO_POL);
    if (ercd == E_OK && *buf) {
        net_memset(*buf, 0, CONTENTS_SIZE);
    }
    return ercd;
}

static void ret_contents_buf(char *buf)
{
    if (!buf) {
        return;
    }
    rel_mpf(ID_CONTENTS_MPF, buf);
    return;
}

ER led_blink(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
    char *contents;
    ER ercd;
    int i;
    for (i = 0; i < cnt; i++) {
        if (net_strcmp(name[i], "led") == 0) {
            LedTmo = atoi(value[i]) * 100;
            if (LedTmo < 0) {
                LedTmo = 0;
            }
            break;
        }
    }
    if (i == cnt) {
        return E_OBJ;
    }
    ercd = get_contents_buf(&contents);
    if (ercd != E_OK) {
        return ercd;
    }

    net_strcat(contents, "<html><body><center>Set LED interval ");
    net_strcat(contents, value[i]);
    net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

    HttpSendText(http, contents, net_strlen(contents));
    ret_contents_buf(contents);
    return ercd;
}

ER ping_send(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
    T_PING_CLIENT ping = {0};
    char *contents;
    ER ercd;
    int i, j;

    for (i = 0, j = 0; i < cnt; i++) {
        if (net_strcmp(name[i], "remote") == 0) {
            ping.ipa = ip_aton(value[i]);
            j = i;
        }
        if (net_strcmp(name[i], "timeout") == 0) {
            ping.tmo = atoi(value[i]);
        }
    }
    ercd = get_contents_buf(&contents);
    if (ercd != E_OK) {
        return ercd;
    }

#ifdef NET_C
    ping.sid = ID_ICMP;
#endif
    ping.devnum = http->NetChannel;
    ping.tmo = PING_TIMEOUT;
    ercd = ping_client(&ping);

    net_strcat(contents, "<html><body><center>");
    if (ercd == E_OK) {
        net_strcat(contents, "Success reply from ");
    } else {
        net_strcat(contents, "No response from ");
    }
    net_strcat(contents, value[j]);
    net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

    HttpSendText(http, contents, net_strlen(contents));
    ret_contents_buf(contents);
    return E_OK;
}

ER sntp_send(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
    T_SNTP_CLIENT sntp;
    char *contents;
    UW time[2];
    ER ercd;
    int i;

    net_memset(&sntp, 0, sizeof(sntp));
    net_memset(time, 0, sizeof(time));

    for (i = 0; i < cnt; i++) {
        if (net_strcmp(name[i], "sntp") == 0) {
            sntp.ipa = ip_aton(value[i]);
            break;
        }
    }
    if (i == cnt) {
        return E_OBJ;
    }
    ercd = get_contents_buf(&contents);
    if (ercd != E_OK) {
        return ercd;
    }

#ifdef NET_C
    sntp.sid = ID_UDP_SMPL;
#endif
    sntp.devnum = http->NetChannel;
    ercd = sntp_client(&sntp, &time[0], &time[1]);

    net_strcat(contents, "<html><body><center>");
    if (ercd == E_OK) {
        itoa_std((time[0]-NTP_BASE_TIME), tmp);
        net_strcat(contents, "<script type=\"text/javascript\">");
        net_strcat(contents, "var d=new Date(");
        net_strcat(contents, tmp);
        net_strcat(contents, "*1000);");
        net_strcat(contents, "document.write(d.toString());</script>");
    } else {
        net_strcat(contents, "No response from ");
        net_strcat(contents, value[i]);
    }
    net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

    HttpSendText(http, contents, net_strlen(contents));
    ret_contents_buf(contents);
    return E_OK;
}

ER dns_resolve(T_HTTP_SERVER *http, int cnt, char *name[], char *value[])
{
    char *contents;
    ER ercd;
    UW ip,dns;
    int i,j;

    for (i = 0, j = 0, dns = 0; i < cnt; i++) {
        if (net_strcmp(name[i], "dns") == 0) {
            dns = ip_aton(value[i]);
        }
        if (net_strcmp(name[i], "fqdn") == 0) {
            j = i;
        }
    }
    ercd = get_contents_buf(&contents);
    if (ercd != E_OK) {
        return ercd;
    }

    ercd = dns_get_ipaddr(ID_UDP_SMPL, dns, value[j], &ip);

    net_strcat(contents, "<html><body><center>");
    if (ercd == E_OK) {
        ip_ntoa(tmp, ip);
        net_strcat(contents, value[j]);
        net_strcat(contents, "<p>");
        net_strcat(contents, tmp);
    } else {
        net_strcat(contents, "Can not resolve ");
        net_strcat(contents, value[j]);
    }
    net_strcat(contents, "<p><a href=/>[Return]<a></center></body></html>");

    HttpSendText(http, contents, net_strlen(contents));
    ret_contents_buf(contents);
    return E_OK;
}

void ledCgi(T_HTTP_SERVER *http)
{
	char *contents;
	ER ercd;
	
	char *cgi_var[1];
	char *cgi_val[1];
	int cgi_cnt;
	
	char hex[16] = "0123456789ABCDEF";
	int led;
	char str[] = "0x00\n";
	
	get_contents_buf(&cgi_var);
	get_contents_buf(&cgi_val);
	

	CgiGetParam(http->hdr.url_q, http->rxlen, &cgi_var, &cgi_val, &cgi_cnt);
	
	if(strcmp(*cgi_var,"led") == 0){
		ledPut(strtol(*cgi_val,NULL,16));
	}
	ret_contents_buf(**cgi_var);
	ret_contents_buf(**cgi_val);
	
	ercd = get_contents_buf(&contents);
	
	led = ledGet();
	str[2] = hex[led / 16];
	str[3] = hex[led%16];
	
	if (ercd == E_OK){
    		net_strcat(contents, "<html><body><center>");
    		net_strcat(contents, str);
    		net_strcat(contents, "</center></body></html>");

    		HttpSendText(http, contents, net_strlen(contents));
    		ret_contents_buf(contents);
	}
    
}


void ledSet( unsigned char data )
{
	unsigned char l;
	
	wai_sem(ID_LEDSEM);
	l = ledGet();
	ledPut( l | data );
	sig_sem(ID_LEDSEM);
}

void ledClear( unsigned char data )
{
	unsigned char l;
	
	wai_sem(ID_LEDSEM);
	l = ledGet();
	ledPut( l & ~data );
	sig_sem(ID_LEDSEM);
}

void ledToggle( unsigned char data )
{
	unsigned char l;
	
	wai_sem(ID_LEDSEM);
	l = ledGet();
	ledPut( l ^ data );
	sig_sem(ID_LEDSEM);
}