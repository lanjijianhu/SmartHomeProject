#include "esp8266.h"
#include "edpkit.h"
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

#define DEBUG_ESP8266


EdpPacket* send_pkg;
char send_buf[MAX_SEND_BUF_LEN];
const u8 mydata[]=",;red_statu,6.76;green_statu,0;yellow_statu,0;blue_statu,0;";
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
	
		printf("%s\r\n","[ESP8266_Init]ENTER AT.");
		SendCmd(AT,"OK",10); 			
		printf("%s\r\n","[ESP8266_Init]EXIT AT.");
		delay_ms(100);
		printf("%s\r\n","[ESP8266_Init]ENTER CWMODE.");
		SendCmd(CWMODE,"OK",10);		
		printf("%s\r\n","[ESP8266_Init]EXIT CWMODE.");
		delay_ms(100);
		printf("%s\r\n","[ESP8266_Init]ENTER RST.");
		SendCmd(RST,"OK",20);	
		printf("%s\r\n","[ESP8266_Init]EXIT RST.");
		delay_ms(100);
		printf("%s\r\n","[ESP8266_Init]ENTER CIFSR.");
		SendCmd(CIFSR,"OK",10);	
		printf("%s\r\n","[ESP8266_Init]EXIT CIFSR.");
		delay_ms(100);
		printf("%s\r\n","[ESP8266_Init]ENTER CWJAP.");
		SendCmd(CWJAP,"OK",22);	
		SendCmd(NULL,"IP",50);	//等待WiFi连接
		printf("%s\r\n","[ESP8266_Init]EXIT CWJAP.");
	    delay_ms(100);
		printf("%s\r\n","[ESP8266_Init]ENTER CIPSTART.");
		SendCmd(CIPSTART,"OK",30);
		printf("%s\r\n","[ESP8266_Init]EXIT CIPSART.");
		delay_ms(100);
		printf("%s\r\n","[ESP8266_Init]ENTER CIPMODE.");
		SendCmd(CIPMODE,"OK",10);
		printf("%s\r\n","[ESP8266_Init]EXIT CIPMODE.");
#ifdef 	DEBUG_ESP8266
printf("Exit ESP8266_Init\r\n");	
#endif
}

/**
  * @brief  生成LED当前状态的上传数据，分割字符串格式
**/
void GetSendBuf(void)
{
//		char text[25] = {0};
//		static float data=1;	
//		LED_GetValue();
//		
//		memset(send_buf,0,MAX_SEND_BUF_LEN);
//		data +=0.12;
//		if(data > 250)
//			data=0;
//		strcat(send_buf, ",;");	
//		strcat(send_buf, "red_statu,");
////		sprintf(text,"%d",red_value);
//		sprintf(text,"%.2f",data);
//		strcat(send_buf, text);
//		strcat(send_buf, ";");
//		
//		strcat(send_buf, "green_statu,");
//		sprintf(text,"%d",green_value);
//		strcat(send_buf, text);
//		strcat(send_buf, ";");

//		strcat(send_buf, "yellow_statu,");
//		sprintf(text,"%d",yellow_value);
//		strcat(send_buf, text);
//		strcat(send_buf, ";");

//		strcat(send_buf, "blue_statu,");
//		sprintf(text,"%d",blue_value);
//		strcat(send_buf, text);
//		strcat(send_buf, ";");
//		printf("\r\n\r\nsend_buf=%s\r\n\r\n",send_buf);
}


/**
  * @brief  发送一条AT指令,返回1应答成功，0应答失败
**/
u8 SendCmd(char* cmd, char* result, int timeOut)
{
	u8 buf[200]={'0'};
	u16 len=0;
	USART2_ReceiveData(buf,&len);//读取数据，把之前的清零
	len=0;
	if(NULL != cmd) //不发送，只等待
	{
		USART2_SendData(cmd,strlen((const char *)cmd));
	}
	
	if(NULL == result) //不用等待
	{
		return 1;
	}
	do
	{
		delay_ms(100);
		USART2_ReceiveData(buf,&len);
		if(len >= strlen(result)) //收到数据
		{
			if((NULL != strstr((const char *)buf, result)))	//判断是否有预期的结果
	        {
	            break;
	        }
		}

	}
	while(timeOut--);
	if(timeOut)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;

    
}

/**
  * @brief  和平台建立设备连接
**/
void ESP8266_DevLink(const char* devid, const char* auth_key, int timeOut)
{	
#ifdef 	DEBUG_ESP8266
	printf("Into ESP8266_DevLink\r\n");	
#endif
		ESP8266_IntoAtCmd();
		delay_ms(200);
		SendCmd(CIPSEND, ">",timeOut); //进入透传模式
		send_pkg = PacketConnect1(devid,auth_key);
		delay_ms(200);
		USART2_SendData(send_pkg->_data,send_pkg->_write_pos);  //发送设备连接请求数据
		delay_ms(500);
		DeleteBuffer(&send_pkg);
		delay_ms(200);
		ESP8266_IntoAtCmd();
		delay_ms(500);
#ifdef 	DEBUG_ESP8266
	printf("Exit ESP8266_DevLink\r\n");	
#endif		
}

/**
  * @brief  检测ESP8266连接状态
**/
int ESP8266_CheckStatus(int timeOut)
{
		u8 res=0;
#ifdef 	DEBUG_ESP8266
	printf("Into ESP8266_CheckStatus\r\n");	
#endif
		ESP8266_IntoAtCmd();
		res=SendCmd(CIPSTATUS, "STATUS:3", 20);
#ifdef 	DEBUG_ESP8266
	printf("res is %d\r\n",res);		
	printf("Exit ESP8266_CheckStatus\r\n");	
#endif
		return res;	
}

/**
  * @brief  向平台上传LED当前状态数据
**/
void ESP8266_SendDataToService(u8 *send_buf)
{		
	
#ifdef 	DEBUG_ESP8266
	printf("Into ESP8266_SendDataToService\r\n");	
#endif
	ESP8266_IntoAtCmd();//进入AT模式
	delay_ms(100);
	SendCmd(CIPSEND, ">", 40);//进入透传模式
	delay_ms(200);
	send_pkg = PacketSavedataSimpleString(DEVICEID,send_buf);//打包数据
	USART2_SendData(send_pkg->_data,send_pkg->_write_pos);	//向平台上传数据点
	DeleteBuffer(&send_pkg);
	delay_ms(500);
	ESP8266_IntoAtCmd();//进入AT模式
	delay_ms(500);
#ifdef 	DEBUG_ESP8266
	printf("Exit ESP8266_SendDataToService\r\n");	
#endif	

}


void ESP8266_IntoAtCmd(void)
{
	u8 flag=0;
	do
	{
		flag=SendCmd("+++", "+++",10); //进入AT模式
	}
	while(!flag);
}

void ESP8266_IntoCIPSEND(void)
{
	ESP8266_IntoAtCmd();
	SendCmd("AT+CIPMODE=1\r\n", "OK",10); //"设置透传模式"
	SendCmd("AT+CIPSEND\r\n", ">",10);//进入透传模式，可以开始传输数据
}










void ESP8266_Test(void)
{
	u8 buf[1024]={'0'};
	u16 len=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART1_Init(115200);	 	//串口初始化为115200
	USART2_Init(115200);	 	//串口初始化为115200
	while(1)
	{		
		ESP8266_Init();
		ESP8266_DevLink(DEVICEID, APIKEY, 20);
		while(1)
		{
			if (1== ESP8266_CheckStatus(100))
			{
				ESP8266_SendDataToService((u8*)mydata);
				delay_ms(800);
//				SendCmd(CIPSEND, ">", 40);
//				USART2_SendData("04-57186609-1",13);
//				while(1)
//				{
//					delay_ms(100);
//					USART2_ReceiveData(buf,&len);
//					if(len>10)
//					{
//					
//						USART1_SendData(buf,strlen(buf));
//						break;
//					}
//				}
//				USART2_SendData("+++",3);  //向ESP8266发送+++结束透传，使ESP8266返回指令模式
//				delay_ms(200);
			}
			else
			{
				delay_ms(200);
				break;
			}
		}

	}
}



