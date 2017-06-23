#include "esp8266.h"
#include "edpkit.h"
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include <string.h>


char PingReqFlag=1;
#define DEBUG_ESP8266


EdpPacket* send_pkg;
char send_buf[MAX_SEND_BUF_LEN];
const char mydata[]={",;red_statu,6.76;green_statu,0;yellow_statu,0;blue_statu,0;"};
/**
  * @brief  ESP8266硬件复位
**/
void ESP8266_Rst(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    //Pb5--对应ESP8266的reset引脚;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	delay_ms(100);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	delay_ms(100);
}

/**
  * @brief  ESP8266初始化
**/
void ESP8266_Init(void)
{
#ifdef 	DEBUG_ESP8266
printf("Into ESP8266_Init\r\n");	
#endif
		ESP8266_Rst();
		delay_ms(1000);
		SendCmd(AT,"OK",10); 			
		delay_ms(1000);
		SendCmd(RST,"OK",10); 			
		delay_ms(1000);
		SendCmd(CWMODE,"OK",10); 			
		delay_ms(1000);
		SendCmd(CWSAP,"OK",10); 			
		delay_ms(1000);
		SendCmd(CIPMUX,"OK",10); 			
		delay_ms(1000);
		SendCmd(CIPSERVER,"OK",10); 			
		delay_ms(1000);
		SendCmd(CIPSTO,"OK",10); 			
		delay_ms(1000);
	
		
#ifdef 	DEBUG_ESP8266
printf("Exit ESP8266_Init\r\n");	
#endif
}




/**
  * @brief  发送一条AT指令,返回1应答成功，0应答失败
**/
u8 SendCmd(char* cmd, char* result, int timeOut)
{
	char buf[1024]={'0'};
	u16 len=0;
	USART2_ReceiveData((u8*)buf,&len);//读取数据，把之前的清零
	memset(buf,0,sizeof(buf));
	len=0;
	if(NULL != cmd) //不发送，只等待
	{
		USART2_SendData((u8*)cmd,strlen((const char *)cmd));
	}
	
	if(NULL == result) //不用等待
	{
		return 1;
	}
	do
	{
		delay_ms(100);
		USART2_ReceiveData((u8*)buf,&len);
		if(len >= strlen(result)) //收到数据
		{
			if(NULL != strstr((const char *)buf, (const char *)result))	//判断是否有预期的结果
	        {
				
#ifdef 	DEBUG_ESP8266
				printf("SendCmd() get %s\r\n",buf);
#endif
	            return 1;
	        }
			else 
			{
				;
			}
		}

	}
	while(timeOut--);
	return 0;

    
}

/**
  * @brief  和平台建立设备连接
**/
void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut)
{	
//#ifdef 	DEBUG_ESP8266
//	printf("Into ESP8266_DevLink\r\n");	
//#endif
//		ESP8266_IntoAtCmd();
//		delay_ms(200);
//		SendCmd(CIPSEND, ">",timeOut); //进入透传模式
//		send_pkg = PacketConnect1(devid,auth_key);
//		delay_ms(200);
//		USART2_SendData(send_pkg->_data,send_pkg->_write_pos);  //发送设备连接请求数据
//		delay_ms(500);
//		DeleteBuffer(&send_pkg);
//		delay_ms(200);
//		ESP8266_IntoAtCmd();
//		delay_ms(500);
//#ifdef 	DEBUG_ESP8266
//	printf("Exit ESP8266_DevLink\r\n");	
//#endif		
}

/**
  * @brief  检测ESP8266连接状态
**/
char ESP8266_CheckStatus(int timeOut)
{
		char res=0;
//#ifdef 	DEBUG_ESP8266
//	printf("Into ESP8266_CheckStatus\r\n");	
//#endif
//		ESP8266_IntoAtCmd();
//		res=SendCmd(CIPSTATUS, "STATUS:3", 20);
//#ifdef 	DEBUG_ESP8266
//	printf("res is %d\r\n",res);		
//	printf("Exit ESP8266_CheckStatus\r\n");	
//#endif
		return res;	
}

/**
  * @brief  向平台上传LED当前状态数据
**/
void ESP8266_SendDataToService(const char *send_buf)
{		
	
//#ifdef 	DEBUG_ESP8266
//	printf("Into ESP8266_SendDataToService\r\n");	
//#endif
//	
//	ESP8266_IntoAtCmd();//进入AT模式
//	delay_ms(100);
//	SendCmd(CIPSEND, ">", 20);//进入透传模式
//	delay_ms(200);
//	
//	send_pkg = PacketSavedataSimpleString(DEVICEID,send_buf);//打包数据

//	USART2_SendData(send_pkg->_data,send_pkg->_write_pos);	//向平台上传数据点
//	
//	DeleteBuffer(&send_pkg);
//	
//	delay_ms(300);
//	ESP8266_IntoAtCmd();//进入AT模式
//	
//	delay_ms(200);
//	
//	
//#ifdef 	DEBUG_ESP8266
//	printf("Exit ESP8266_SendDataToService\r\n");	
//#endif	

}


void ESP8266_IntoAtCmd(void)
{
	u8 flag=0;
	do
	{
		flag=SendCmd("+++", "+++",5); //进入AT模式
	}
	while(!flag);
}

void ESP8266_IntoCIPSEND(void)
{
	ESP8266_IntoAtCmd();
	SendCmd("AT+CIPSEND=1,6\r\n", ">",10);//进入透传模式，可以开始传输数据
}


void ESP8266_SendDataToPhone(const unsigned char* string)
{
//	USART3_SendData(string,len);
	SendCmd((char*)string, "OK",10);
}







void ESP8266_Test(void)
{
//	u8 buf[1024]={'0'};
//	u16 len=0;
//	u32 count=0;
//	delay_init();	    	 //延时函数初始化	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	USART1_Init(115200);	 	//串口初始化为115200
//	USART2_Init(115200);	 	//串口初始化为115200
//	while(1)
//	{		
//		ESP8266_Init();
//		ESP8266_DevLink(DEVICEID, APIKEY, 20);
//		while(1)
//		{
//			if (1== ESP8266_CheckStatus(10))
//			{
////				ESP8266_SendDataToService(mydata);
//				
//				delay_ms(1000);
////				ESP8266_PingReq();
//				count++;
//				if(count > 120) //2分钟发送一次心跳包
//				{
//					count =0;
//					ESP8266_PingReq();
//				}
//				USART2_ReceiveData(buf,&len);
//				if(len >2)
//				{
//					printf("I get %s\r\n",buf);
//				}
////				delay_ms(1000);
////				test1();
//			}
//			else
//			{
//				delay_ms(200);
//				printf("duan kai lian jie %d\r\n",count);
////				while(1);
//				break;
//			}
//		}

//	}
}

static void test1(void)
{
	u8 buf[1024];
	u16 len=0;
//		SendCmd(CIPSEND, ">", 40);
		USART2_SendData("04-57186609-1",13);
		while(1)
		{
			delay_ms(100);
			USART2_ReceiveData(buf,&len);
			if(len>10)
			{
			
				USART1_SendData(buf,strlen(buf));
				break;
			}
		}
		USART2_SendData("+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
		delay_ms(200);
}


