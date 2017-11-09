
#include "I2C_Driver.h"
#include "stm32f10x_gpio.h"

void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(I2C_SCL_GPIO, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(I2C_SDA_GPIO, &GPIO_InitStructure);
}


void I2C_delay(void)
{	
   u16 i=50; //这里可以优化速度	，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   } 
}

bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_delay();
	return TRUE;
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

bool I2C_WaitAck(void) 	 //返回为:=0有ACK,=1无ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;
	SDA_read;
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return FALSE;
	}
	SCL_L;
	return TRUE;
}

void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}

u8 I2C_ReceiveByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

//写入1字节数据       待写入数据    待写入地址       器件类型(24c16或SD2403)
bool I2C_WriteByte(u8 SendByte, u8 WriteAddress, u8 DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress & 0xfe);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(WriteAddress);   //设置低起始地址      
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(SendByte);
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}   
    I2C_Stop(); 
	//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
    //Systick_Delay_1ms(10);
    return TRUE;
}

//注意不能跨页写
//写入1串数据      待写入数组地址    待写入长度      待写入地址       器件类型(24c16或SD2403)
bool I2C_BufferWrite(u8* pBuffer, u8 length,     u8 WriteAddress, u8 DeviceAddress)
{
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress & 0xFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(WriteAddress);   //设置低起始地址      
	if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	
	  
		while(length--)
		{
		  I2C_SendByte(* pBuffer);
		  if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
          pBuffer++;
		}
	  I2C_Stop();
	  //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
	  //Systick_Delay_1ms(10);
	  return TRUE;
}

//读出1串数据         存放读出数据  待读出长度      待读出地址       器件类型(24c16或SD2403)	
bool I2C_ReadByte(u8* pBuffer,   u8 length,     u8 ReadAddress,  u8 DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_delay();
    I2C_delay();
    I2C_delay();
    I2C_SendByte(DeviceAddress&0xfe);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(ReadAddress);   //设置低起始地址      
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_delay();
    I2C_delay();
    I2C_delay();
    
    I2C_Start();
    I2C_delay();
    I2C_delay();
    I2C_delay();
    I2C_SendByte(DeviceAddress |0x01);
    I2C_WaitAck();
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return TRUE;
}



