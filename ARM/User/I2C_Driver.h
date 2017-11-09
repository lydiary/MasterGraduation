#ifndef __I2C_Driver_H
#define __I2C_Driver_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
//#include "stm32f10x_lib.h"
#define I2C_SCL_GPIO GPIOC
#define I2C_SCL_PIN GPIO_Pin_9
#define I2C_SDA_GPIO GPIOC
#define I2C_SDA_PIN GPIO_Pin_8
#define SCL_H         I2C_SCL_GPIO->BSRR = I2C_SCL_PIN
#define SCL_L         I2C_SCL_GPIO->BRR  = I2C_SCL_PIN 
   
#define SDA_H         I2C_SDA_GPIO->BSRR = I2C_SDA_PIN
#define SDA_L         I2C_SDA_GPIO->BRR  = I2C_SDA_PIN

#define SCL_read      (I2C_SCL_GPIO->IDR  & I2C_SCL_PIN)
#define SDA_read      (I2C_SDA_GPIO->IDR  & I2C_SDA_PIN)

void I2C_GPIO_Config(void);
bool I2C_WriteByte(u8 SendByte, u8 WriteAddress, u8 DeviceAddress);
bool I2C_ReadByte(u8* pBuffer,   u8 length, u8 ReadAddress,  u8 DeviceAddress);
bool I2C_BufferWrite(u8* pBuffer, u8 length, u8 WriteAddress, u8 DeviceAddress);
#endif 



