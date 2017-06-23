#ifndef HTU21D_H 
#define HTU21D_H 			   

#include "sys.h"
#include "myiic.h"

u16 Task_Read_Temperature_humidity(unsigned char target);
void Reset_HTU21D(void);
void Write_User_Register(void);
u16 Read_Temperature_humidity(unsigned char target);

typedef enum Mode
{
	TemperatureMode=1,
	HumidityMode=2
} HTU21DdataMode ; 



void HTU21D_Reset(void);
u8 HTU21D_ReadReg(void);
void HTU21D_WriteReg(u8 cmd);
u16  HTU21D_GetData(HTU21DdataMode mode);
void HTU21D_Test(void);
void HumidityCal(u16 data);
void TemperatureCal(u16 data);
#endif


