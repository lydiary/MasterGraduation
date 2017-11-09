#include "sil9013.h"
bool sil9013_init(void)
{
	bool result;
	result = I2C_WriteByte(0x10,0x05,0x60);//auto reset
	result &= I2C_WriteByte(0x07,0x08,0x60);//power on;24-bits mode
	result &= I2C_WriteByte(0x01,0x09,0x60);
	//result &= I2C_WriteByte(0x0a,0x4a,0x60);//0b0000 1010
											//bit7 Insert SYNC (SAV/EAV) into ITU.656 stream
											//bit6 Multiplex YC 4:2:2 into a single channel
											//bit5 10-bit to 8-bit dithering
											//bit4 RGB-to-YCbCr range scaling
											//bit3 RGB-to-YCbCr color-space convert
											//bit2 422 to 444
											//bit1 444 to 422
	return result;
}
