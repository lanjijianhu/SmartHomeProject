/**
  ******************************************************************************
  * @file    ucos_task_app.c
  * @author  Hoire Application Team
  * @version V2.1.0
  * @date    20-December-2016
  * @brief   WSN½ÚµãºÍÍø¹ØµÄÓ¦ÓÃ³ÌÐò¿ò¼Ü-Ó¦ÓÃ²ã
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "BH1750.h"
#include "delay.h"
#include <math.h>

#define DEBUG_BH1750
#ifdef DEBUG_BH1750
#include "usart.h"
#endif


void BH1750_WriteCmd(u8 cmd)
{
    IIC_Start();                  //ÆðÊ¼ÐÅºÅ
    IIC_Send_Byte(BH1750_Addr+0);   //·¢ËÍÉè±¸µØÖ·+Ð´ÐÅºÅ
	IIC_Wait_Ack();
    IIC_Send_Byte(cmd);    //ÄÚ²¿¼Ä´æÆ÷µØÖ·
	IIC_Wait_Ack();
    IIC_Stop();                   //·¢ËÍÍ£Ö¹ÐÅºÅ
}
void BH1750_Init(void)
{

#ifdef DEBUG_BH1750
	printf("Into BH1750_Init\r\n");
#endif

	// BH1750_WriteCmd(BH1750_ON);	 //power on
	// delay_ms(5);
	// BH1750_WriteCmd(BH1750_RSET);	//clear
	// delay_ms(5);
	// BH1750_WriteCmd(BH1750_ONE);  //Ò»´ÎH·Ö±æÂÊÄ£Ê½£¬ÖÁÉÙ120ms£¬Ö®ºó×Ô¶¯¶ÏµçÄ£Ê½  
	// delay_ms(5);
	 BH1750_WriteCmd(BH1750_ON);  //Power On
	 delay_ms(5);
	 BH1750_WriteCmd(BH1750_RSET);	//Software Rest
	 delay_ms(5);
	 BH1750_WriteCmd(0x42);  //
	 delay_ms(5);
	 BH1750_WriteCmd(0x65);  //ÉèÖÃÍ¸¹âÂÊÎª100%
	 delay_ms(5);
	 BH1750_WriteCmd(0x11);  //ÉèÖÃÎª¸ß¾«¶ÈÄ£Ê½£¬Á¬Ðø²É¼¯
	 delay_ms(10);

#ifdef DEBUG_BH1750
	printf("Exit BH1750_Init\r\n");
#endif	

}

u16 BH1750_ReadData(void)
{ 
	u8 buf[2]={0,0};
	u16 result=0;
#ifdef DEBUG_BH1750
	printf("Into BH1750_ReadData\r\n");
#endif 		
  	IIC_Start();                          //ÆðÊ¼ÐÅºÅ
  	IIC_Send_Byte(BH1750_Addr+1);         //·¢ËÍÉè±¸µØÖ·+¶ÁÐÅºÅ
	IIC_Wait_Ack();
	buf[0]=IIC_Read_Byte(1);  //·¢ËÍACK
	buf[1]=IIC_Read_Byte(0);  //·¢ËÍNACK
  	IIC_Stop();                          //Í£Ö¹ÐÅºÅ
  	delay_ms(1);
	result = (buf[0]<<8)+buf[1];

	if(result == 0xff00)
	{

#ifdef DEBUG_BH1750
	printf("BH1750_ReadData Get data error\r\n");
#endif			
		return 0xffff;
	}

#ifdef DEBUG_BH1750
	printf("BH1750_ReadData Get data is%d\r\n",result);	
	printf("Exit BH1750_ReadData\r\n");
#endif	

	return result ;
}


u16 BH1750_Cal(u16 data)
{
	u16 res=0;
	u16 temp=0;
	u8 bitCount=0;
	u8 i=0;
#ifdef DEBUG_BH1750
	printf("Into BH1750_Cal\r\n");
#endif 
	for(bitCount=0;bitCount<16;bitCount++)
	{
		temp=(data<<bitCount) & 0x8000;
		if(temp)
		{
			i=15-bitCount;
			res=res+pow(2,i);
		}
	}
	res =(float)res/1.2;
#ifdef DEBUG_BH1750
	printf("BH1750_Cal Get data is%d\r\n",res);	
	printf("Exit BH1750_Cal\r\n");
#endif	
	return res;
}

void BH1750_Test(void)
{
	u16 temp=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART1_Init(115200);	 	//串口初始化为115200
	IIC_Init();
	printf("BH1750_Test\r\n");
	BH1750_Init();
	while(1)
	{
		temp=BH1750_ReadData();
		BH1750_Cal(temp);
		printf("\r\n\r\n\r\n");
		delay_ms(500);
	}
}


