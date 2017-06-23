#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "esp8266.h"
#include <string.h>

u8 Wifibuf[1024]={'0'};
u16 WifiDatalen=0;
u8 zigbeebuf[1024]={'0'};
u16 zigbeeDatalen=0;

static void system_init(void);
static void HAL_Init(void);

 
int main(void)
{	 
	
	
//	u8 Wifibuf[1024]={'0'};
//	u16 WifiDatalen=0;
//	u8 zigbeebuf[1024]={'0'};
//	u16 zigbeeDatalen=0;
	u8* pWifibuf=NULL;
	system_init(); //系统初始化
	HAL_Init();//外设模块初始化
	printf("wait connect\r\n");
	while(1)
	{
		delay_ms(10);
		USART2_ReceiveData(Wifibuf,&WifiDatalen);
		if(WifiDatalen > 5)
		{
			if(NULL != strstr((const char*)Wifibuf,(const char*)"CONNECT"))
			{
				memset(Wifibuf,0,WifiDatalen);
				break;
			}
		}
		memset(Wifibuf,0,WifiDatalen);
	}//等待手机连接
	while(1)
	{
		delay_ms(10);
		USART2_ReceiveData(Wifibuf,&WifiDatalen);
		if(WifiDatalen > 5)
		{
			if(NULL != strstr((const char*)Wifibuf,(const char*)"CONNECT"))
			{
				memset(Wifibuf,0,WifiDatalen);
				break;
			}
		}
		memset(Wifibuf,0,WifiDatalen);
	}//等待手机连接
	
	USART2_ReceiveData(Wifibuf,&WifiDatalen);
	memset(Wifibuf,0,WifiDatalen);
	printf("connected\r\n");
	while(1)
	{
		
		USART2_ReceiveData(Wifibuf,&WifiDatalen); //扫描WiFi模块是否收到数据
		if(WifiDatalen >= 6) //WiFi模块收到数据
		{
			printf("I get WiFi data%s\r\n",Wifibuf);//打印收到的信息
			//提取有用数据
			pWifibuf=strstr((const char*)Wifibuf,":");
			delay_ms(10);
			if(NULL != pWifibuf) //有效数据
			{
				pWifibuf++;
				USART3_SendData((const unsigned char*)pWifibuf,strlen((const char*)pWifibuf));//发送数据给zigbee模块
			}
			else
			{
				;
			}
			
			memset(Wifibuf,0,sizeof(Wifibuf));//清除缓冲区
			
		}
		else
		{
			;
		}
		
		USART3_ReceiveData(zigbeebuf,&zigbeeDatalen); //扫描zigbee模块是否收到数据
		if(zigbeeDatalen >= 6) //zigbee模块收到数据
		{
			printf("I get zigbee data%s\r\n",zigbeebuf);//串口打印收到的zigbee数据
//			memset(buf,0,1024);
			delay_ms(10);
			ESP8266_IntoCIPSEND();//进入透传模式
			delay_ms(100);
			ESP8266_SendDataToPhone((const char *)zigbeebuf);//通过WiFi模块把数据发送到手机
			memset(zigbeebuf,0,sizeof(zigbeebuf));//清除缓冲区
		}
		else
		{
			;
		}
		delay_ms(5);
		
	}
	return 0;
}



static void system_init(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART1_Init(115200);	 	//串口初始化为115200
	USART2_Init(115200);	 	//串口初始化为115200
	USART3_Init(115200);	 	//串口初始化为115200
	
}

static void HAL_Init(void)
{
	ESP8266_Init();
//	LED_Init();
}

