#ifndef CFG_H
#define CFG_H

//设备名字
#define KITC "kitche_"  //厨房
#define LIVI "living_"  //客厅
#define BALC "balcon_"  //阳台



//传感器类型
#define LIGHT "light,"
#define TEMP "temp,"
#define HUMIDITY "humi,"
#define SOIL  "soil,"
#define RAINDROP  "rain,"
#define COMBUSTIBLE "combus,"
#define CARBON  "carbon,"
#define FORMALDEHYDE "formal,"
#define INFRARED "infra,"
#define BEEP "beep,"
#define LED "led,"
#define RELAY "relay,"


typedef enum
{
	LightSensor = 0x01,  //光照传感器
	AirTemperatureSensor = 0x02, //空气温度传感器
	AirHumiditySensor = 0x03, //空气湿度传感器
	SoilSensor = 0x04, //土壤湿度 传感器
	RaindropSensor=0x05, //雨滴传感器
	CombustibleGasSensor=0x06,//可燃气体传感器
	CarbonMonoxideSensor=0x07, //一氧化碳传感器
	FormaldehydeSensor=0x08, //甲醛传感器
	InfraRedSensor=0x09, //红外传感器
	BeepControler=0x0a, //蜂鸣器
	LEDControler=0x0b, //led
	RelayControler=0x0c //继电器
	
}TypedefEquipmentName;

typedef enum
{
	GatawayAdd = 0x00,
	BalconyAdd = 0x01,
	LivingRoomAdd = 0x02,
	KitchenAdd =0x03	
	
}TypedefEquipmentAdd;

typedef enum
{
	ATHeader=0xfe,
	DataHeader=0xaa	
	
}TypedefDataHead;

typedef enum
{
	ATReadMode=0xfe,
	ATWriteMode=0xaa,
	DataMode=0x1f,
	ControlMode=0x2c
	
	
}TypedefDataCmd;








#endif

