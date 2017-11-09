#include "sil9134.h"
bool sil9134_init(void)
{
	bool result;
	result = I2C_WriteByte(0x35, 0x08, 0x72);//System Control Register #1 bit0:When LOW, the TMDS core is powered down and interrupts are in power-down
	result &= I2C_WriteByte(0x00, 0x2f, 0x7a);
	return result;
}
