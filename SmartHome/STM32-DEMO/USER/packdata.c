#include "packdata.h"
#include <string.h>
#include "control.h"
#include "usart.h"
#include <stdlib.h>
//#include "edpkit.h"
#include "delay.h"
#include "esp8266.h"
//定义变量
unsigned char SendBuf[128]={0};
SensorMSG GetSensorMSG[GetSensorMSG_MAX]={0};
char ServerBuff[1024]={0};
SensorMSG SensorData;
//定义函数
//打包zigbee内网数据函数
//返回：数据帧首地址，数据帧长度
//输入：数据结构体
unsigned char  *packMSG(TypedefData Pckdata,unsigned char *datalen)
//帧头、目的地址、源地址、功能/命令码、数据长度、数据、校验和
{
	
	
	unsigned char len=0;
	unsigned char check=0x00;
	unsigned char i=0;
	unsigned char j=0;
	*datalen=0;
	memset(SendBuf,0,sizeof(SendBuf));//清零缓冲区
	SendBuf[i++]=Pckdata.DataHead;//帧头
	SendBuf[i++]=Pckdata.DestAdd;//目标地址
	SendBuf[i++]=Pckdata.SourceAdd;//源地址
	SendBuf[i++]=Pckdata.cmd;//功能码
	SendBuf[i++]=len;//数据长度

	for(j=0;0 != Pckdata.SensorMSGPck->SensorName;j++)
	{
		SendBuf[i++]=Pckdata.SensorMSGPck->SensorName;
		SendBuf[i++]=Pckdata.SensorMSGPck->DataH;
		SendBuf[i++]=Pckdata.SensorMSGPck->DataL;
		Pckdata.SensorMSGPck++;
	}
	
	len=j*sizeof(SensorMSG);//统计数据长度
	SendBuf[4]=len;//数据长度
	
	for(j=0;j<i;j++)//校验和累加
	{
		check += SendBuf[j];
	}
	
	SendBuf[i]=check;//添加校验和
	*datalen=len+6;//数据总长度
	return SendBuf;
	
}



//解析zigbee内网数据包
//返回：数据段、数据段数量（长度）、功能（命令）码
//输入：数据帧首地址、本机地址
SensorMSG *unpackMSG(unsigned char *datastr,unsigned char DestAdd,unsigned char *SourceAdd,unsigned char *Cmd,unsigned char *len)
{
	unsigned char check=0;
	unsigned char datalen=0;
	unsigned char i=0;
	unsigned char j=0;
	memset(GetSensorMSG,0,sizeof(GetSensorMSG));//清零缓冲区
	if(DataHeader == datastr[0] && DestAdd == datastr[1]) //数据帧
	{
//		printf("1\r\n");
		datalen = datastr[4];//提取数据段长度
		printf("datalen=%d\r\n",datalen);
		for(i=0;i<4;i++)//前面四个数据累加和
		{
			check += datastr[i];
		}
			
		for(i=4;i<datalen+5;i++) //累加和校验
		{
			check += datastr[i];
		}
		if(check == datastr[i]) //校验正确
		{
//			printf("10\r\n");
			for(j=0,i=5;i<datalen+5;j++)
			{
				GetSensorMSG[j].SensorName = datastr[i++];
				GetSensorMSG[j].DataH = datastr[i++];
				GetSensorMSG[j].DataL = datastr[i++];
			}
			*SourceAdd=datastr[2];
			*Cmd=datastr[3];
			*len=datalen/3;
//			printf("len=%d\r\n",*len);
			return GetSensorMSG;
		}
		else //校验错误
		{
//			printf("11\r\n");
//			printf("check=%2x\r\n",check);
//			printf("datastr=%2x\r\n",datastr[i]);
			*SourceAdd=0;
			*Cmd=0;
			*len=0;
			return NULL;
		}
	}
	else //非数据帧
	{
//		printf("2\r\n");
		*SourceAdd=0;
		*Cmd=0;
		*len=0;
		return NULL;
	}
}


void EquipmentControl(SensorMSG *pckdata,unsigned char len)
{
	unsigned char i=0;
	u16 data=0;
	for(i=0;i<len;i++)
	{
		data=pckdata[i].DataH*256+pckdata[i].DataL;
		
		switch (pckdata[i].SensorName) //提取设备名字
		{
			case BeepControler :  //蜂鸣器控制函数
				ControlBeep(data);
			break;
			case LEDControler : //LED控制函数
				ControlLed(data);
			break;
			case RelayControler : //继电器控制函数
				ControlRelay(data);
			break;
			
			default :  //其他情况
				break;
			
		}
	}
}



void AnalysisPckMSG(SensorMSG *pckdata,unsigned char cmd,unsigned char len)
{
	switch(cmd)
	{
		case DataMode :  //数据模式
			break;
		case ControlMode: //控制模式
			EquipmentControl(pckdata,len);
			break;
		default : break;
	}
}

//打包成服务器能识别的数据帧
//输入：源地址、传感器参数、传感器数据数量
//输出：符合EDP协议的数据帧
char * PckServerMSG(unsigned char SourceAdd,SensorMSG *pckdata,unsigned char len)
{
	char text[25]={0};
	char devicename[9]={0};
	u8 i=0;
	float datares=0.0;
	switch(SourceAdd)
	{
		case BalconyAdd : strcat(devicename,BALC); //阳台
			break;
		case LivingRoomAdd : strcat(devicename,LIVI); //客厅
			break;
		case KitchenAdd : strcat(devicename,KITC); //厨房
			break;
		default : 
			break;
	
	}
	memset(ServerBuff,0,sizeof(ServerBuff));//清零缓冲区
	strcat(ServerBuff,",;"); //数据头
	for(i=0;i<len;i++) //打包数据
	{
		strcat(ServerBuff, devicename);
		{
			switch(pckdata[i].SensorName)
			{
				case LightSensor : 
					strcat(ServerBuff,LIGHT); //光照
					break;
				case AirTemperatureSensor : 
					strcat(ServerBuff,TEMP);//温度
					break;
				case AirHumiditySensor : 
					strcat(ServerBuff,HUMIDITY);//湿度
					break;
				case SoilSensor : 
					strcat(ServerBuff,SOIL);//土壤湿度
					break;
				case RaindropSensor : 
					strcat(ServerBuff,RAINDROP);//雨滴
					break;
				case CombustibleGasSensor : 
					strcat(ServerBuff,COMBUSTIBLE);//液化气体
					break;
				case CarbonMonoxideSensor : 
					strcat(ServerBuff,CARBON);//一氧化碳
					break;
				case FormaldehydeSensor : 
					strcat(ServerBuff,FORMALDEHYDE);//甲醛
					break;
				case InfraRedSensor : 
					strcat(ServerBuff,INFRARED);//人体红外
					break;
				case BeepControler : 
					strcat(ServerBuff,BEEP);//蜂鸣器
					break;
				case LEDControler : 
					strcat(ServerBuff,LED);//led
					break;
				case RelayControler : 
					strcat(ServerBuff,RELAY);//继电器
					break;
				default :
					break;
				
			}
		}
		
		datares=SensorCal(pckdata[i]);
		sprintf(text,"%.2f",datares);
		strcat(ServerBuff, text);
		strcat(ServerBuff, ";");
		
	}
	return ServerBuff;
	
}


float SensorCal(SensorMSG data)
{
	float res=0.0;
	res=data.DataH*256+data.DataL;
	res /=10.0;
	return res;
}

//解析服务器数据包
//输入：服务器帧数据
//输出：分析函数能够识别的数据
TypedefData UnpckServerMSG(char *pdata)
{
	char *p=pdata;
	u8 i=0;
	u16 idata=0;
	TypedefData data={0};
//	SensorMSG SensorData;
	data.DataHead=DataHeader;
	data.SensorMSGPck=&SensorData;
//	memset(GetSensorMSG,0,sizeof(GetSensorMSG));//清零缓冲区
	while(*p != '{' && i < 250)
	{
		p++;
		i++;
	}
	
	if(NULL != strstr(p,"{KITC_")) //厨房节点数据
	{
//		printf("1\r\n");
		p +=strlen("{KITC_");
		data.DestAdd=KitchenAdd;
	}
	else if(NULL != strstr(p,"{LIVI_")) //客厅节点数据
	{
		
		p +=strlen("{LIVI_");
		data.DestAdd=LivingRoomAdd;
	}
	else if(NULL != strstr(p,"{BALC_")) //阳台节点数据
	{
		
		p +=strlen("{BALC_");
		data.DestAdd=BalconyAdd;
	}
	else 
	{
		
		;
	}
	
	data.SourceAdd=GatawayAdd;
	data.cmd=ControlMode;
	
	if(NULL != strstr(p,"LED"))
	{
		
		
		p +=strlen("LED");
		idata=atoi(p);
//		printf("idata=%d\r\n",idata);
		data.SensorMSGPck->SensorName  =LEDControler;
		data.SensorMSGPck->DataH = idata/256;
		data.SensorMSGPck->DataL = idata%256;
	}
	
	
	else if(NULL != strstr(p,"BEEP"))
	{
		
		p +=strlen("BEEP");
		idata=atoi(p);
//		printf("idata=%d\r\n",idata);
		data.SensorMSGPck->SensorName  =BeepControler;
		data.SensorMSGPck->DataH = idata/256;
		data.SensorMSGPck->DataL = idata%256;
	}
	
	else if(NULL != strstr(p,"RELAY"))
	{
		
		p +=strlen("RELAY");
		idata=atoi(p);
//		printf("idata=%d\r\n",idata);
		data.SensorMSGPck->SensorName  =RelayControler;
		data.SensorMSGPck->DataH = idata/256;
		data.SensorMSGPck->DataL = idata%256;
	}
	
	else
	{
		
		;
	}
	return  data;
}


//测试内网打包、解包、分析包、控制设备

//AA 03 03 1F 09 0B 34 67 0C 45 12 0A 67 4C 9E  打包发送（数据包）
//AA 03 03 2C 09 0B 34 67 0C 45 12 0A 67 4C AB （控制包）
void Zigbee_Test(void)
{
	SensorMSG Sensor[]={
	{LEDControler,0x34,0x67},
	{RelayControler,0x45,0x12},
	{BeepControler,0x67,0x4c},
	};
	TypedefData Pckdata;
	u16 len=0;
	u8 datalen;
	u8 buf[128]={0};
	SensorMSG *p=NULL;
	u8 *pdata;
	u8 MyCmd=0;
	u8 Mylen=0;
	u8 MySourceAdd=0;
	u16 LightData=0;
	Pckdata.DataHead=DataHeader;
	Pckdata.DestAdd=KitchenAdd;
	Pckdata.SourceAdd=KitchenAdd;
	Pckdata.cmd=ControlMode;
	Pckdata.SensorMSGPck=Sensor;
	delay_init();//延时函数初始化
	USART1_Init(115200);
	USART3_Init(115200);
	USART1_SendData("hello",6);
//	pdata=packMSG(Pckdata,&datalen);
//	USART1_SendData(pdata,datalen);
	while(1)
	{	
		USART3_ReceiveData(buf,&len);
		if(len >= 6) //收到数据
		{
			p=unpackMSG(buf,KitchenAdd,&MySourceAdd,&MyCmd,&Mylen);
			AnalysisPckMSG(p,MyCmd,Mylen);
			printf("hello\r\n");
		}
		
		else
		{
			delay_ms(10);
		}
	}

}



//打包服务器数据包、解包服务器数据包
void Server_Test(void)
{
		SensorMSG Sensor[]={
	{LEDControler,0x34,0x67},
	{RelayControler,0x45,0x12},
	{BeepControler,0x67,0x4c},
	};
	TypedefData Pckdata;
	u16 len=0;
	u8 datalen;
	u8 buf[1024]={0};
	SensorMSG *p=NULL;
	u8 *pdata;
	u8 MyCmd=0;
	u8 Mylen=0;
	u8 i=0;
	u8 MySourceAdd=0;
	u16 LightData=0;
	Pckdata.DataHead=DataHeader;
	Pckdata.DestAdd=KitchenAdd;
	Pckdata.SourceAdd=KitchenAdd;
	Pckdata.cmd=ControlMode;
	Pckdata.SensorMSGPck=Sensor;
	delay_init();//延时函数初始化
	USART1_Init(115200);
	USART2_Init(115200);
	USART1_SendData("hello",6);
	
	pdata=PckServerMSG(LivingRoomAdd,Sensor,3);
	printf("data=%s\r\n",pdata);
	while(1)
	{		
		ESP8266_Init();
//		ESP8266_DevLink(DEVICEID, APIKEY, 20);
		while(1)
		{
				delay_ms(100);
				i++;
				if(i>250)
				{
					i=0;
					if (0== ESP8266_CheckStatus(10))
					{
						break;
					}
					ESP8266_SendDataToService(pdata);
				}
				USART2_ReceiveData(buf,&len);
				if(len >10)
				{
					printf("len=%d\r\n",len);
					printf("buf=%s\r\n",buf);
					Pckdata=UnpckServerMSG((char*)buf);
					printf("Pckdata=%2x\r\n",Pckdata.SensorMSGPck->SensorName );
					printf("data=%d\r\n",Pckdata.SensorMSGPck->DataL );
				}
			
		}

	}
}

