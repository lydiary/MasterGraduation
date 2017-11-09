#include "ch7301.h"
#include "I2C_Driver.h"
bool ch7301_init()
{
	u8 cnt=10;
	u8 i;
	bool result;
	while(cnt--)
	{
		result = I2C_WriteByte(0xc0,0x49,0xec);//power on ;dvi normal
		result &= I2C_WriteByte(0x00,0x1c,0xec);//CM
		result &= I2C_WriteByte(15,0x1d,0xec);//
		result &= I2C_WriteByte(0x09,0x21,0xec);//RGB bypass
		//result &= I2C_WriteByte(0x08,0x33,0xec);//TPCP
		//result &= I2C_WriteByte(0x16,0x34,0xec);//TPD
		//result &= I2C_WriteByte(0x60,0x36,0xec);//TPF
      
		result &= I2C_WriteByte(0x06,0x33,0xec);//TPCP
		result &= I2C_WriteByte(0x26,0x34,0xec);//TPD
		result &= I2C_WriteByte(0xa0,0x36,0xec);//TPF
      
		result &= I2C_WriteByte(0x98,0x1f,0xec);//IDF = 0 V H polarity 1
		result &= I2C_WriteByte(0x01,0x56,0xec);//
          //I2C_WriteByte(8,0x1c,0xec);//IC
		if(result) break;
	}
	for(i = 0; i < 16;i++)
	 result &= I2C_WriteByte(i,0x1d,0xec);
	return result;
}
