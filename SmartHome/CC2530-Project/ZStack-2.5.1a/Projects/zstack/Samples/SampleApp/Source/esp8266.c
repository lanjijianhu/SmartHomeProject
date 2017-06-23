#include "esp8266.h"
#include <string.h>
//除了以上的头文件，需要用到cc2530的串口、延时头文件
#include "hal_uart.h"
#include "Onboard.h"

//硬件复位函数
//esp8266低电平复位，此函数把复位引脚拉低100ms左右再拉高延时1s左右
void ESP8266_Rst(void)
{
  //定义cc2530控制复位的引脚
  //设置引脚为低电平
  //延时100ms左右
  //设置引脚为高电平
  //延时1s左右
  
}


//初始化函数
/*初始化函数主要对WiFi模块进行配置，一般修改路由器连接和服务器连接的参数即可，其他
参数保持默认即可*/
void ESP8266_Init1(void)
{
//  ESP8266_Rst();//硬件复位
  
  ESP8266_DelayMs(1000);
  
  SendCmd(AT,"OK",10);//进入AT指令并进行相关配置		
  
  ESP8266_DelayMs(100);
  
  SendCmd(CWMODE,"OK",10);	
  
  ESP8266_DelayMs(100);
  
  SendCmd(RST,"OK",20);	
  
  ESP8266_DelayMs(1000);

  SendCmd(CWSAP,"OK",200);
  
  ESP8266_DelayMs(1000);
  
//  SendCmd(CIPSTART,"OK",100);
  SendCmd(CIPMUX,"OK",100);
  
  ESP8266_DelayMs(1000);
  
//  SendCmd(CIPMODE,"OK",10);
  SendCmd(CIPSTO,"OK",100);
  
   ESP8266_DelayMs(1000);
  
   SendCmd(SERVER,"OK",100);

}
//发送AT指令函数
/**
功能：发送一条AT指令，返回1应答成功，0应答失败
输入参数：
cmd:待发送的指令
result：期待返回的结果
timeout：超时等待时间，步进为100ms
输出:1，应答成功；0，应答失败
*/
uint8 SendCmd(char* cmd, char* result, int timeOut)//AT指令，包含应答和超时等待
{
  unsigned char buf[1024]={'0'};
  uint16 len=0;
  ESP8266_ReceiveData(buf,&len);//读取数据，把之前的数据清零
  memset(buf,0,sizeof(buf));//清除数据
  len=0;
  if(NULL != cmd) //不发送只等待
  {
    ESP8266_SendData((const unsigned char *)buf,strlen((const char *)cmd)); //发送数据
  }
  else 
  {
    ;
  }
  
  if(NULL == result)//不用等待
  {
    return 1;
  }
  else
  {
    ;
  }
  do
  {
    ESP8266_DelayMs(100);
    ESP8266_ReceiveData(buf,&len);
    if(len >= strlen(result)) //收到数据
    {
      if(NULL != strstr((const char *)buf,(const char *)result))//和预期结果一样
      {
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
//检测WiFi模块是否连接
//返回1表示连接成功，0表示没连接
char ESP8266_CheckStatus(uint16 timeOut)//检测WiFi模块是否连接
{
  char res=0;
  ESP8266_IntoAtCmd();
  res=SendCmd(CIPSTATUS, "STATUS:3", timeOut);
  return res;
}


void ESP8266_SendData(const unsigned char *send_buf,uint16 buflen)//芯片发送数据给WiFi模块
{
  HalUARTWrite(0, (uint8 *)send_buf, buflen);
}
void ESP8266_ReceiveData(unsigned char *receive_buf,uint16 *buflen)//芯片读取WiFi模块发送过来的数据
{
  *buflen= HalUARTRead(0, (uint8 *)receive_buf,sizeof(receive_buf));
}
void ESP8266_IntoAtCmd(void)//进入AT指令模式
{
  uint8 flag=0;
  do
  {
    flag=SendCmd("+++", "+++",5); //进入AT模式
  }
  while(!flag);
}
void ESP8266_IntoCIPSEND(void)//进入透传模式
{
  ESP8266_IntoAtCmd(); //进入AT模式
  ESP8266_DelayMs(100);//延时函数
  SendCmd("AT+CIPMODE=1\r\n", "OK",10); //"设置透传模式"
  SendCmd("AT+CIPSEND\r\n", ">",10);//进入透传模式，可以开始传输数据
}


static void ESP8266_DelayMs(uint16 time)//延时函数
{
  while(time)
  {
    MicroWait (1000);     // Wait 1ms
    time--;
  }
  
}


void ESP8266_SendDataToService(const unsigned char *send_buf,uint16 buflen)//芯片发送数据给WiFi模块，透传模式
{
  ESP8266_IntoCIPSEND();//进入透传模式
  ESP8266_DelayMs(100);//延时函数
  ESP8266_SendData(send_buf,buflen);
  ESP8266_DelayMs(300);//延时函数
  
}

void ESP8266_Test(void)//测试
{
  while(1)
  {
    ESP8266_Init();
    ESP8266_DelayMs(1000);//延时函数
  }
}