#ifndef BH1750_H
#define BH1750_H
#include "sys.h"
#include "myiic.h"
// AddrΩ”µÕµÁ∆Ω
#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

void BH1750_WriteCmd(u8 cmd); 
void BH1750_Init(void);
u16 BH1750_ReadData(void);
u16 BH1750_Cal(u16 data);
void BH1750_Test(void);
#endif

