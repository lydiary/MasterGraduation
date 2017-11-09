#include "tw2867.h"
#include "I2C_Driver.h"
#include "stm32f10x_gpio.h"

unsigned char tw2867_addr;
unsigned char get_video_status(unsigned char ch);

void tw2867_init(void)
{
  //u8 chip_revision[1];
  GPIO_InitTypeDef  GPIO_InitStructure;
  u32 cnt;
  /* Configure Reset_n */
  GPIO_InitStructure.GPIO_Pin =  RSTN_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(RSTN_GPIO, &GPIO_InitStructure);
  GPIO_ResetBits(RSTN_GPIO, RSTN_PIN);
  cnt = 0xfffff;
  while(cnt--);
  GPIO_SetBits(RSTN_GPIO, RSTN_PIN);
  cnt = 0xfffff;
  while(cnt--);
  I2C_GPIO_Config();
  tw2867_addr = 0x50;
  tw2867_cfg();
  get_video_status(0);
  get_video_status(1);
  get_video_status(2);
  get_video_status(3);
}
void tw2867_cfg()
{
  I2C_WriteByte(0x40, 0xfa, tw2867_addr);
  I2C_WriteByte(0x15, 0x60, tw2867_addr);
  I2C_WriteByte(0x01, 0xca, tw2867_addr);
  I2C_WriteByte(0x01, 0xca, tw2867_addr);  
  I2C_WriteByte(0x01, 0xcc, tw2867_addr);  
  I2C_WriteByte(0x01, 0xcd, tw2867_addr);   
}

unsigned char get_video_status(unsigned char ch)
{
	unsigned char temp;
	if(ch > 3)
	{
		tw2867_addr = 0x52;
		I2C_ReadByte(&temp,1,(ch - 4) << 4, tw2867_addr);
	}else
	{
		tw2867_addr = 0x50;
		I2C_ReadByte(&temp,1,ch << 4, tw2867_addr);
	}
	return temp;
}
