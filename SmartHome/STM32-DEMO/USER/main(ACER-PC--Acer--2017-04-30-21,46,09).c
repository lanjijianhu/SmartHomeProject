#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "htu21d.h" 
#include "BH1750.h"
/************************************************
 ALIENTEK 战舰STM32F103开发板实验17
 ADC 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 
 int main(void)
 {	 
//    u16 adcx;
//	u8 i=0;
//	 float temp=0;
//	//u16 Temperature=0;
//	u8 receivebuf[200]={0};
//	HTU21DdataMode mode=TemperatureMode;
//	mode=HumidityMode;
//	
//	delay_init();	    	 //延时函数初始化	  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
//	USART1_Init(115200);	 	//串口初始化为115200
// 	LED_Init();			     //LED端口初始化
////	LCD_Init();			 	
//// 	Adc_Init();		  		//ADC初始化
//	IIC_Init();
//	BH1750_Init();
//	USART1_SendData("Hello\n",6);
	HTU21D_Test();
	while(1)
	{
		adcx=BH1750_ReadData();
		BH1750_Cal(adcx);
		printf("\r\n\r\n\r\n");
		delay_ms(500);
	}
	while(1)
	{
		
		//////////////////////////////////////////////////////////////////////////////////////
		printf("/////////////////////////////////////\n");
		adcx=Get_Adc_Average(ADC_Channel_0,3);
		temp=(float)adcx*(3.3/4096);
		printf("ADC_Channel_0 is %.2f\n",temp);
		
		adcx=Get_Adc_Average(ADC_Channel_1,3);
		temp=(float)adcx*(3.3/4096);
		printf("ADC_Channel_1 is %.2f\r\n",temp);
		
		adcx=Get_Adc_Average(ADC_Channel_4,3);
		temp=(float)adcx*(3.3/4096);
		printf("ADC_Channel_4 is %.2f\r\n",temp);
		adcx=Get_Adc_Average(ADC_Channel_5,3);
		temp=(float)adcx*(3.3/4096);
		printf("ADC_Channel_5 is %.2f\r\n",temp);
		LED0=!LED0;
		delay_ms(1000);	
	}
 }

