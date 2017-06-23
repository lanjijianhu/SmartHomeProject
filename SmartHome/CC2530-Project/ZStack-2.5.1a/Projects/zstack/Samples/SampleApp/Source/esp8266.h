#ifndef   ESP8266_H_H
#define   ESP8266_H_H
#include "hal_types.h"

//#define   AT          "AT\r\n"	
//#define   CWMODE      "AT+CWMODE=3\r\n"
//#define   RST         "AT+RST\r\n"
#define   CIFSR       "AT+CIFSR\r\n"
#define   CWJAP       "AT+CWJAP=\"ygnwifi\",\"ygn123456ygn\"\r\n" //WiFi名字和密码
#define   CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"  //服务器连接方式、地址和端口
#define   CIPMODE     "AT+CIPMODE=1\r\n"
#define   CIPSEND     "AT+CIPSEND\r\n"
#define   CIPSTATUS   "AT+CIPSTATUS\r\n"

#define   AT          "AT\r\n"	
#define   CWMODE      "AT+CWMODE=3\r\n"
#define   RST         "AT+RST\r\n"
#define   CWSAP       "AT+CWSAP=\"ljx\",\"12345678\",1,3\n"
#define   CIPMUX      "AT+CIPMUX=1\n"
#define   CIPSTO      "AT+CIPSTO=2880\n"
#define   SERVER      "AT+CIPSERVER=1,8080\n"


#define   MAX_SEND_BUF_LEN  1024
#define   MAX_RECEIVE_BUF_LEN  1024 //接收数据缓冲区长度，不可更改，否则很容易出现缓冲区过小内存泄露情况



extern  char SendBuff[MAX_SEND_BUF_LEN];//发送数据缓冲区
extern  char ReceiveBuff[MAX_RECEIVE_BUF_LEN];//接收数据缓存区

/*****
说明：
WiFi模块和cc2530使用串口透传的方式通信，WiFi模块主要包含的功能是初始化、复位、发送数据、接收数据、配置AT指令、检测WiFi模块是否连接成功、测试等功能函数
其中硬件复位函数、发送数据和接收数据为底层函数，需要用到cc2530的硬件资源，其他的是在发送和接收函数的基础上进行开发的应用层函数，所以测试函数主要是测试
三个底层函数是否能正常工作后，才能对应用层的相关函数进行测试。
ygn
*****/


extern  void ESP8266_Rst(void);//硬件复位函数
extern  void ESP8266_Init1(void);//初始化函数
extern  uint8 SendCmd(char* cmd, char* result, int timeOut);//AT指令，包含应答和超时等待
extern  char ESP8266_CheckStatus(uint16 timeOut);//检测WiFi模块是否连接
extern  void ESP8266_SendData(const unsigned char *send_buf,uint16 buflen);//芯片发送数据给WiFi模块
extern  void ESP8266_ReceiveData(unsigned char *receive_buf,uint16 *buflen);//芯片读取WiFi模块发送过来的数据
extern  void ESP8266_IntoAtCmd(void);//进入AT指令模式
extern  void ESP8266_IntoCIPSEND(void);//进入透传模式
static  void ESP8266_DelayMs(uint16 time);//延时函数
extern  void ESP8266_SendDataToService(const unsigned char *send_buf,uint16 buflen);//芯片发送数据给WiFi模块，透传模式
extern  void ESP8266_Test(void);//测试
#endif


