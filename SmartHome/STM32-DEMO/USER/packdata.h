#ifndef PACKDATA_H
#define PACKDATA_H
#include "cfg.h"
#include "sys.h"


#define GetSensorMSG_MAX  20

typedef struct 
{
	unsigned char SensorName;
	unsigned char DataH;
	unsigned char DataL;
}SensorMSG;

typedef struct  
{
	unsigned char DataHead; //帧头
	unsigned char DestAdd; //目标地址
	unsigned char SourceAdd;//源地址
	unsigned char cmd;//命令码
	SensorMSG *SensorMSGPck;//传感器数据
}TypedefData;



//声明变量
extern unsigned char SendBuf[128];
extern SensorMSG GetSensorMSG[GetSensorMSG_MAX];
extern char ServerBuff[1024];
extern SensorMSG SensorData;
//声明函数
unsigned char *packMSG(TypedefData Pckdata,unsigned char *datalen);
SensorMSG *unpackMSG(unsigned char *datastr,unsigned char DestAdd,unsigned char *SourceAdd,unsigned char *Cmd,unsigned char *len);
void EquipmentControl(SensorMSG *pckdata,unsigned char len);
void AnalysisPckMSG(SensorMSG *pckdata,unsigned char cmd,unsigned char len);
char * PckServerMSG(unsigned char SourceAdd,SensorMSG *pckdata,unsigned char len);
float SensorCal(SensorMSG data);
TypedefData UnpckServerMSG(char *pdata);

void Zigbee_Test(void);
void Server_Test(void);


#endif


