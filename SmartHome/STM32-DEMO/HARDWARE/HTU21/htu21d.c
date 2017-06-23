/**
  ******************************************************************************
  * @file    ucos_task_app.c
  * @author  Hoire Application Team
  * @version V2.1.0
  * @date    20-December-2016
  * @brief   WSN节点和网关的应用程序框架-应用层
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "htu21d.h"
#include "delay.h"
//#include "myiic.h"

#define DEBUG_HTU21D
#ifdef DEBUG_HTU21D
#include "usart.h"
#endif
/*******************************************************************************************

				软件复位程序，每次打开电源前都要重新启动传感器系统
				传感器系统开始初始化，并恢复默认的设置状态 
******************************************************************************************/
void Reset_HTU21D(void)
{
		IIC_Start();                  //起始信号
		
		IIC_Send_Byte(0x80);	   //发送地址+写命令 最低位为0是写
	
		IIC_Wait_Ack();	 //等待器件ACK回复
	
		IIC_Send_Byte(0xfe);	   //发送软件复位命令
	
		IIC_Wait_Ack();	 //等待器件ACK回复
	
		IIC_Stop();//产生一个停止条件	 
	
		delay_ms(20);
}


/*******************************************************************************************

				配置用户寄存器 先要读出用户寄存器的值 再用逻辑运算加入其它位写入寄存器
				设置用户寄存器的7和0位为00 即RH和T 的分辨率分别为12位和14位
******************************************************************************************/
void Write_User_Register(void)
{
	u8 dat;
	
	IIC_Start();                  //起始信号
	
	IIC_Send_Byte(0x80);	   //发送地址+写命令 最低位为0是写
	
	IIC_Wait_Ack();	 //等待器件ACK回复
	
	IIC_Send_Byte(0xe7);	   //发送读用户寄存器命令
	
	IIC_Wait_Ack();	 //等待器件ACK回复
	
	IIC_Start();                  //起始信号
	
	IIC_Send_Byte(0x81);	   //发送地址+读命令 最低位为1是读
	
	IIC_Wait_Ack();	 //等待器件ACK回复
	
	dat=IIC_Read_Byte(0);	//读出用户寄存器的值，并发送NACK信号  
	
	IIC_Start();                  //起始信号
	
	IIC_Send_Byte(0x80);	   //发送地址+写命令 最低位为0是写
	
	IIC_Wait_Ack();	 //等待器件ACK回复
	
	IIC_Send_Byte(0xe6);	   //发送写用户寄存器命令
	
	IIC_Wait_Ack();	 //等待器件ACK回复 
	dat&=~(1<<7);
	dat&=~(1<<0);
	IIC_Send_Byte(dat);	   //设置用户寄存器的7和0位为00 即RH和T 的分辨率分别为12位和14位
	
	IIC_Wait_Ack();	 //等待器件ACK回复 
	
	IIC_Stop();//产生一个停止条件
	
	delay_ms(20);
}

/****************************************************************************************

						在非主机模式下读取温度或湿度的值。湿度分辨率为12位，温度分辨率为14位
						target 为 1   读取湿度值 
									 为 0   读取温度值
****************************************************************************************/
u16 Read_Temperature_humidity(unsigned char target)
{
		u8 DATA_MSB,DATA_LSB;
	
		u16 result;
	
		IIC_Start();                  //起始信号
	
		IIC_Send_Byte(0x80);	   //发送地址+写命令 最低位为0是写
	
		IIC_Wait_Ack();	 //等待器件ACK回复 
	
		if(target)//判断需测量温度还是湿度
		
			IIC_Send_Byte(0xe5);	   //发送主机模式配置下测量湿度RH命令
	
		else 	IIC_Send_Byte(0xe3);	   //发送主机模式配置下测量温度T命令
		
		IIC_Wait_Ack();	 //等待器件ACK回复 
		
		IIC_Start();                  //起始信号
		
		IIC_Send_Byte(0x81);	   //发送地址+读命令 最低位为1是读
		
		IIC_Wait_Ack();	 //等待器件ACK回复 
		
		delay_ms(100);
		
		DATA_MSB=IIC_Read_Byte(1);//读出高位字节数据并发送 ACK信号
		
		DATA_LSB=IIC_Read_Byte(0);//读出低位字节数据并发送 NACK信号
		
		IIC_Stop();//产生一个停止条件
		
		delay_ms(200);
		if(target) DATA_LSB&=~(0x0f);
		else 			 DATA_LSB&=~(0x03);
		result=((DATA_MSB<<8)+(DATA_LSB));//合并两字节，低字节的后两位要置0
//		printf("result is %d \r\n",result);
		return  result ;
		 
}

/**************************************************************************************************

												测量温湿度任务函数
												target 为 1   读取湿度值 
															 为 0   读取温度值
**************************************************************************************************/
u16 Task_Read_Temperature_humidity(unsigned char target)
{
		u16 result=0;
//		u8 time=1;
//		while(time--){
//			delay_ms(30);
//			Reset_HTU21D();
//			delay_ms(30);
//			Write_User_Register();
//		
//			result=Read_Temperature_humidity(target);
//		
//			if(result == 0xFFFC){
//					printf("Senor HTU21D read failed,try again.\r\n");
//				continue;					//未接传感器，直接返回错误代码
//			}
//			printf("Air=%x\r\n",result);
//			if(target)	SaveSensorData(Air_Hum_Data,result);
//			else				SaveSensorData(Air_Tem_Data,result);
//			return result;
//		}
//		if(target)	ReadSensorData(Air_Hum_Data,&result);
//		else				ReadSensorData(Air_Tem_Data,&result);
////		TaskSenorErr_Warning( );
		return result;
}

void HTU21D_Reset(void)
{
#ifdef DEBUG_HTU21D
	printf("Into HTU21D_Reset\r\n");
#endif
	IIC_Start();
	IIC_Send_Byte(0x80);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xfe);
	IIC_Wait_Ack();
	IIC_Stop();
#ifdef DEBUG_HTU21D
	printf("Exit HTU21D_Reset\r\n");
#endif
}
u8 HTU21D_ReadReg(void)
{
	u8 res=0;
#ifdef DEBUG_HTU21D
	printf("Into HTU21D_ReadReg\r\n");
#endif
	IIC_Start();
	IIC_Send_Byte(0x80);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xe7);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0x81);
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0); //不应答
	IIC_Stop();
#ifdef DEBUG_HTU21D
	printf("HTU21D_ReadReg Result is %x \r\n",res);
	printf("Exit HTU21D_ReadReg\r\n");
#endif	
	return res;
}
void HTU21D_WriteReg(u8 cmd)
{
#ifdef DEBUG_HTU21D
	printf("Into HTU21D_WriteReg\r\n");
#endif
	IIC_Start();
	IIC_Send_Byte(0x80);
	IIC_Wait_Ack();
	IIC_Send_Byte(0xe5);
	IIC_Wait_Ack();
	IIC_Send_Byte((cmd & 0xe3));//预留数据不可改
	IIC_Wait_Ack();
	IIC_Stop();
#ifdef DEBUG_HTU21D
	printf("Exit HTU21D_WriteReg\r\n");
#endif	

}
//采用非保持主机方式
u16  HTU21D_GetData(HTU21DdataMode mode)
{
	u8 i=150;
	u8 AckRes=1;
	u8 DATA_LSB=0;
	u8 DATA_MSB=0;
	u16 res=0xff;
#ifdef DEBUG_HTU21D
	printf("Into HTU21D_GetData\r\n");
#endif
	IIC_Start();
	IIC_Send_Byte(0x80);
	IIC_Wait_Ack();
	if(TemperatureMode == mode)
	{
		IIC_Send_Byte(0xf3);
	}
	else if(HumidityMode == mode)
	{
		IIC_Send_Byte(0xf5);
	}
	else
	{
		IIC_Stop();
#ifdef DEBUG_HTU21D
	printf("HTU21D_GetData Input data error\r\n");
#endif	
		return 0xffff;
	}
	IIC_Wait_Ack();
	//等待数据采集完毕
	do
	{
		delay_ms(10); 
		IIC_Start();
		IIC_Send_Byte(0x81);
		AckRes=IIC_Wait_Ack();
	}
	while(i-- && AckRes); //超时等待应答成功
	if(i) //数据采集完成
	{
		DATA_MSB=IIC_Read_Byte(1);
		DATA_LSB=IIC_Read_Byte(0); //读取数据
		IIC_Stop();
		if(0x02 == (0x02 & DATA_LSB) && (HumidityMode == mode)) //返回的是湿度数据,请求和返回一致
		{
			DATA_LSB &= 0xFC;
			res=DATA_MSB*256+DATA_LSB;
#ifdef DEBUG_HTU21D
	printf("HTU21D_GetData Humidity data is%d\r\n",res);
#endif
			return res;
		}
		else if(0==(0x02 & DATA_LSB) && (TemperatureMode == mode)) //返回的是温度数据，请求和返回一致
		{
			DATA_LSB &= 0xFC;
			res=DATA_MSB*256+DATA_LSB;
#ifdef DEBUG_HTU21D
	printf("HTU21D_GetData Temperature data is%d\r\n",res);
#endif
			return res;
		}
		else
		{
#ifdef DEBUG_HTU21D
	printf("HTU21D_GetData Get data error\r\n");
#endif
			return 0xffff;
		}

	}
	else
	{
		IIC_Stop();
#ifdef DEBUG_HTU21D
	printf("HTU21D_GetData Timeout\r\n");
#endif	
		return 0xffff;
	}
}

//保持主机模式
//u16  HTU21D_GetData(HTU21DdataMode mode)
//{
//	u8 DATA_MSB,DATA_LSB;
//	
//	u16 result;
//	if(mode !=TemperatureMode && mode !=HumidityMode) //输入参数错误
//	{
//#ifdef DEBUG_HTU21D
//	printf("HTU21D_GetData Input data is %d, error\r\n",mode);
//#endif
//		return 0xffff;
//	}

//	IIC_Start();                  //起始信号

//	IIC_Send_Byte(0x80);	   //发送地址+写命令 最低位为0是写

//	IIC_Wait_Ack();	 //等待器件ACK回复 

//	if(TemperatureMode == mode) //检测温度
//	{
//		IIC_Send_Byte(0xe3);	   //发送主机模式配置下测量温度T命令
//	}
//	else  //检测湿度
//	{
//		IIC_Send_Byte(0xe5);	   //发送主机模式配置下测量湿度RH命令
//	}
//	
//	IIC_Wait_Ack();	 //等待器件ACK回复 
//	
//	IIC_Start();                  //起始信号
//	
//	IIC_Send_Byte(0x81);	   //发送地址+读命令 最低位为1是读
//	
//	IIC_Wait_Ack();	 //等待器件ACK回复 
//	
//	delay_ms(100);
//	
//	DATA_MSB=IIC_Read_Byte(1);//读出高位字节数据并发送 ACK信号
//	
//	DATA_LSB=IIC_Read_Byte(0);//读出低位字节数据并发送 NACK信号
//	
//	IIC_Stop();//产生一个停止条件
//	
//	delay_ms(200);
//	if(TemperatureMode == mode) //检测温度
//	{
//		DATA_LSB&=0xfffc;
//	}
//	else  //检测湿度
//	{
//		DATA_LSB&=0xfff0;
//	}
//	result=DATA_MSB*256+DATA_LSB;//合并两字节，低字节的后两位要置0
////		printf("result is %d \r\n",result);
//	return  result ;
//}





void HTU21D_Test(void)
{
	u16 datares=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	USART1_Init(115200);	 	//串口初始化为115200
	IIC_Init();
	printf("HTU21D_Test\r\n");
	HTU21D_Reset();
	HTU21D_ReadReg();
	while(1)
	{
		datares=HTU21D_GetData(1);
		TemperatureCal(datares);
		delay_ms(500);
		datares=HTU21D_GetData(2);
		HumidityCal(datares);
		delay_ms(500);
		HTU21D_GetData(3);
//		datares=Read_Temperature_humidity(1); //湿度
//		HumidityCal(datares);
//		delay_ms(500);
//		datares=Read_Temperature_humidity(0); //温度
//		TemperatureCal(datares);
//		delay_ms(500);
		printf("\r\n\r\n\r\n");
	}
}

void HumidityCal(u16 data)
{
	float res=0;
	data &=0xfff0;
	res =125*(float)data/65536-6;
	printf("HumidityCal is %.2f\r\n",res);
	
}


void TemperatureCal(u16 data)
{
	float res=0;
	data &=0xfffc;
	res =175.72*(float)data/65536-46.85;
	printf("TemperatureCal is %.2f\r\n",res);
}



