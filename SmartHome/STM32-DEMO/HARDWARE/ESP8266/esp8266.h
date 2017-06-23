#ifndef   ESP8266_H_H
#define   ESP8266_H_H
#include "sys.h"


#define   AT          	"AT\r\n"	
#define   RST         	"AT+RST\r\n"
#define   CWMODE      	"AT+CWMODE=3\r\n"
#define   CWSAP       	"AT+CWSAP=\"APAP\",\"12345678\",1,3\r\n"
#define   CIPMUX    	"AT+CIPMUX=1\r\n" 
#define   CIPSERVER     "AT+CIPSERVER=1,8080\r\n"
#define   CIPSTO     	"AT+CIPSTO=2880\r\n"


#define   MAX_SEND_BUF_LEN  1024




extern  char send_buf[MAX_SEND_BUF_LEN];





extern  void ESP8266_Rst(void);
extern  void ESP8266_Init(void);
//extern  void GetSendBuf(void);
extern  u8 SendCmd(char* cmd, char* result, int timeOut);
extern  void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut);
extern  char ESP8266_CheckStatus(int timeOut);
extern  void ESP8266_SendDataToPhone(const unsigned char* string);
extern  void ESP8266_IntoAtCmd(void);
extern  void ESP8266_IntoCIPSEND(void);

extern  void ESP8266_Test(void);
static void test1(void);
#endif


