#include "FPGA.h"
#include "I2C_Driver.h"
VIDEO_CFG video_cfg_ch0;
VIDEO_CFG video_cfg_ch1;
VIDEO_CFG video_cfg_ch2;
VIDEO_CFG video_cfg_ch3;
void video_cfg(unsigned short base,unsigned char* video_cfg)
{
	unsigned char size,i;
	size = sizeof(VIDEO_CFG);
	for(i = 0; i < size;i++)
		I2C_WriteByte(video_cfg[i],base + i,FPGA_DEV_ADDR);
}
void Init_FPGA(void)   
{   
}
void config_disp_src(unsigned char lay, unsigned char src)
{
	I2C_WriteByte(src,lay,FPGA_DEV_ADDR);
}
unsigned char get_state()
{
  unsigned char s = 0;
  I2C_ReadByte(&s,1,0xff,FPGA_DEV_ADDR);
  return s;
}
void  disp_one(unsigned char ch)
{
	video_cfg_ch0.vin_s_width    = 720  ;
	video_cfg_ch0.vin_s_height   = 576  ;
	video_cfg_ch0.clipper_left   = 0    ;
	video_cfg_ch0.clipper_width  = 720  ;
	video_cfg_ch0.clipper_top    = 0    ;
	video_cfg_ch0.clipper_height = 576  ;
	video_cfg_ch0.vout_t_width   = 1920 ;
	video_cfg_ch0.vout_t_height  = 1080 ;
	video_cfg_ch0.alpha          = 255  ;
	video_cfg_ch0.disp_top       = 0    ;
	video_cfg_ch0.disp_left      = 0    ;
	video_cfg_ch0.source         = ch   ;
	video_cfg_ch0.scaler_k_h     = (video_cfg_ch0.clipper_width << 8)/video_cfg_ch0.vout_t_width;
	video_cfg_ch0.scaler_k_v     = (video_cfg_ch0.clipper_height << 8)/video_cfg_ch0.vout_t_height;
	
	video_cfg_ch1.vin_s_width    = 720  ;
	video_cfg_ch1.vin_s_height   = 576  ;
	video_cfg_ch1.clipper_left   = 0    ;
	video_cfg_ch1.clipper_width  = 720  ;
	video_cfg_ch1.clipper_top    = 0    ;
	video_cfg_ch1.clipper_height = 576  ;
	video_cfg_ch1.vout_t_width   = 0  ;
	video_cfg_ch1.vout_t_height  = 0  ;
	video_cfg_ch1.alpha          = 0  ;
	video_cfg_ch1.disp_top       = 0    ;
	video_cfg_ch1.disp_left      = 720  ;
	video_cfg_ch1.source         = 0    ;
	video_cfg_ch1.scaler_k_h     = (video_cfg_ch1.clipper_width << 8)/video_cfg_ch1.vout_t_width;
	video_cfg_ch1.scaler_k_v     = (video_cfg_ch1.clipper_height << 8)/video_cfg_ch1.vout_t_height;	
	
	video_cfg_ch2.vin_s_width    = 720  ;
	video_cfg_ch2.vin_s_height   = 576  ;
	video_cfg_ch2.clipper_left   = 0    ;
	video_cfg_ch2.clipper_width  = 720  ;
	video_cfg_ch2.clipper_top    = 0    ;
	video_cfg_ch2.clipper_height = 576  ;
	video_cfg_ch2.vout_t_width   = 0  ;
	video_cfg_ch2.vout_t_height  = 0  ;
	video_cfg_ch2.alpha          = 0  ;
	video_cfg_ch2.disp_top       = 576  ;
	video_cfg_ch2.disp_left      = 0    ;
	video_cfg_ch2.source         = 1    ;
	video_cfg_ch2.scaler_k_h     = (video_cfg_ch2.clipper_width << 8)/video_cfg_ch2.vout_t_width;
	video_cfg_ch2.scaler_k_v     = (video_cfg_ch2.clipper_height << 8)/video_cfg_ch2.vout_t_height;	
	
	video_cfg_ch3.vin_s_width    = 720  ;
	video_cfg_ch3.vin_s_height   = 576  ;
	video_cfg_ch3.clipper_left   = 0    ;
	video_cfg_ch3.clipper_width  = 720  ;
	video_cfg_ch3.clipper_top    = 0    ;
	video_cfg_ch3.clipper_height = 576  ;
	video_cfg_ch3.vout_t_width   = 0  ;
	video_cfg_ch3.vout_t_height  = 0  ;
	video_cfg_ch3.alpha          = 0  ;
	video_cfg_ch3.disp_top       = 576  ;
	video_cfg_ch3.disp_left      = 720  ;
	video_cfg_ch3.source         = 2    ;
	video_cfg_ch3.scaler_k_h     = (video_cfg_ch3.clipper_width << 8)/video_cfg_ch3.vout_t_width;
	video_cfg_ch3.scaler_k_v     = (video_cfg_ch3.clipper_height << 8)/video_cfg_ch3.vout_t_height;	
	video_cfg(0x00,(unsigned char*)&video_cfg_ch0);
	video_cfg(0x20,(unsigned char*)&video_cfg_ch1);
	video_cfg(0x40,(unsigned char*)&video_cfg_ch2);
	video_cfg(0x60,(unsigned char*)&video_cfg_ch3);
} 

void  disp_four(unsigned char ch0,unsigned char ch1,unsigned char ch2,unsigned char ch3)
{
	video_cfg_ch0.vin_s_width    = 720  ;
	video_cfg_ch0.vin_s_height   = 576  ;
	video_cfg_ch0.clipper_left   = 0    ;
	video_cfg_ch0.clipper_width  = 720  ;
	video_cfg_ch0.clipper_top    = 0    ;
	video_cfg_ch0.clipper_height = 576  ;
	video_cfg_ch0.vout_t_width   = 960  ;
	video_cfg_ch0.vout_t_height  = 576  ;
	video_cfg_ch0.alpha          = 255  ;
	video_cfg_ch0.disp_top       = 0    ;
	video_cfg_ch0.disp_left      = 0    ;
	video_cfg_ch0.source         = ch0  ;
	video_cfg_ch0.scaler_k_h     = (video_cfg_ch0.clipper_width << 8)/video_cfg_ch0.vout_t_width;
	video_cfg_ch0.scaler_k_v     = (video_cfg_ch0.clipper_height << 8)/video_cfg_ch0.vout_t_height;
	
	video_cfg_ch1.vin_s_width    = 720  ;
	video_cfg_ch1.vin_s_height   = 576  ;
	video_cfg_ch1.clipper_left   = 0    ;
	video_cfg_ch1.clipper_width  = 720  ;
	video_cfg_ch1.clipper_top    = 0    ;
	video_cfg_ch1.clipper_height = 576  ;
	video_cfg_ch1.vout_t_width   = 960  ;
	video_cfg_ch1.vout_t_height  = 576  ;
	video_cfg_ch1.alpha          = 255  ;
	video_cfg_ch1.disp_top       = 0    ;
	video_cfg_ch1.disp_left      = 960  ;
	video_cfg_ch1.source         = ch1  ;
	video_cfg_ch1.scaler_k_h     = (video_cfg_ch1.clipper_width << 8)/video_cfg_ch1.vout_t_width;
	video_cfg_ch1.scaler_k_v     = (video_cfg_ch1.clipper_height << 8)/video_cfg_ch1.vout_t_height;	
	
	video_cfg_ch2.vin_s_width    = 720  ;
	video_cfg_ch2.vin_s_height   = 576  ;
	video_cfg_ch2.clipper_left   = 0    ;
	video_cfg_ch2.clipper_width  = 720  ;
	video_cfg_ch2.clipper_top    = 0    ;
	video_cfg_ch2.clipper_height = 576  ;
	video_cfg_ch2.vout_t_width   = 960  ;
	video_cfg_ch2.vout_t_height  = 576  ;
	video_cfg_ch2.alpha          = 255  ;
	video_cfg_ch2.disp_top       = 576  ;
	video_cfg_ch2.disp_left      = 0    ;
	video_cfg_ch2.source         = ch2  ;
	video_cfg_ch2.scaler_k_h     = (video_cfg_ch2.clipper_width << 8)/video_cfg_ch2.vout_t_width;
	video_cfg_ch2.scaler_k_v     = (video_cfg_ch2.clipper_height << 8)/video_cfg_ch2.vout_t_height;	
	
	video_cfg_ch3.vin_s_width    = 720  ;
	video_cfg_ch3.vin_s_height   = 576  ;
	video_cfg_ch3.clipper_left   = 0    ;
	video_cfg_ch3.clipper_width  = 720  ;
	video_cfg_ch3.clipper_top    = 0    ;
	video_cfg_ch3.clipper_height = 576  ;
	video_cfg_ch3.vout_t_width   = 960  ;
	video_cfg_ch3.vout_t_height  = 576  ;
	video_cfg_ch3.alpha          = 255  ;
	video_cfg_ch3.disp_top       = 576  ;
	video_cfg_ch3.disp_left      = 960  ;
	video_cfg_ch3.source         = ch3  ;
	video_cfg_ch3.scaler_k_h     = (video_cfg_ch3.clipper_width << 8)/video_cfg_ch3.vout_t_width;
	video_cfg_ch3.scaler_k_v     = (video_cfg_ch3.clipper_height << 8)/video_cfg_ch3.vout_t_height;	
	video_cfg(0x00,(unsigned char*)&video_cfg_ch0);
	video_cfg(0x20,(unsigned char*)&video_cfg_ch1);
	video_cfg(0x40,(unsigned char*)&video_cfg_ch2);
	video_cfg(0x60,(unsigned char*)&video_cfg_ch3);
} 
void  disp_pip(unsigned char ch0,unsigned char ch1)
{
	video_cfg_ch0.vin_s_width    = 720  ;
	video_cfg_ch0.vin_s_height   = 576  ;
	video_cfg_ch0.clipper_left   = 0    ;
	video_cfg_ch0.clipper_width  = 720  ;
	video_cfg_ch0.clipper_top    = 0    ;
	video_cfg_ch0.clipper_height = 576  ;
	video_cfg_ch0.vout_t_width   = 1920 ;
	video_cfg_ch0.vout_t_height  = 1080 ;
	video_cfg_ch0.alpha          = 255  ;
	video_cfg_ch0.disp_top       = 0    ;
	video_cfg_ch0.disp_left      = 0    ;
	video_cfg_ch0.source         = ch0  ;
	video_cfg_ch0.scaler_k_h     = (video_cfg_ch0.clipper_width << 8)/video_cfg_ch0.vout_t_width;
	video_cfg_ch0.scaler_k_v     = (video_cfg_ch0.clipper_height << 8)/video_cfg_ch0.vout_t_height;
	
	video_cfg_ch1.vin_s_width    = 720  ;
	video_cfg_ch1.vin_s_height   = 576  ;
	video_cfg_ch1.clipper_left   = 0    ;
	video_cfg_ch1.clipper_width  = 720  ;
	video_cfg_ch1.clipper_top    = 0    ;
	video_cfg_ch1.clipper_height = 576  ;
	video_cfg_ch1.vout_t_width   = 720  ;
	video_cfg_ch1.vout_t_height  = 576  ;
	video_cfg_ch1.alpha          = 255  ;
	video_cfg_ch1.disp_top       = 252  ;
	video_cfg_ch1.disp_left      = 600  ;
	video_cfg_ch1.source         = ch1  ;
	video_cfg_ch1.scaler_k_h     = (video_cfg_ch1.clipper_width << 8)/video_cfg_ch1.vout_t_width;
	video_cfg_ch1.scaler_k_v     = (video_cfg_ch1.clipper_height << 8)/video_cfg_ch1.vout_t_height;	
	
	video_cfg_ch2.vin_s_width    = 720  ;
	video_cfg_ch2.vin_s_height   = 576  ;
	video_cfg_ch2.clipper_left   = 0    ;
	video_cfg_ch2.clipper_width  = 720  ;
	video_cfg_ch2.clipper_top    = 0    ;
	video_cfg_ch2.clipper_height = 576  ;
	video_cfg_ch2.vout_t_width   = 0  ;
	video_cfg_ch2.vout_t_height  = 0  ;
	video_cfg_ch2.alpha          = 0  ;
	video_cfg_ch2.disp_top       = 576  ;
	video_cfg_ch2.disp_left      = 0    ;
	video_cfg_ch2.source         = 1    ;
	video_cfg_ch2.scaler_k_h     = (video_cfg_ch2.clipper_width << 8)/video_cfg_ch2.vout_t_width;
	video_cfg_ch2.scaler_k_v     = (video_cfg_ch2.clipper_height << 8)/video_cfg_ch2.vout_t_height;	
	
	video_cfg_ch3.vin_s_width    = 720  ;
	video_cfg_ch3.vin_s_height   = 576  ;
	video_cfg_ch3.clipper_left   = 0    ;
	video_cfg_ch3.clipper_width  = 720  ;
	video_cfg_ch3.clipper_top    = 0    ;
	video_cfg_ch3.clipper_height = 576  ;
	video_cfg_ch3.vout_t_width   = 0  ;
	video_cfg_ch3.vout_t_height  = 0  ;
	video_cfg_ch3.alpha          = 0  ;
	video_cfg_ch3.disp_top       = 576  ;
	video_cfg_ch3.disp_left      = 720  ;
	video_cfg_ch3.source         = 2    ;
	video_cfg_ch3.scaler_k_h     = (video_cfg_ch3.clipper_width << 8)/video_cfg_ch3.vout_t_width;
	video_cfg_ch3.scaler_k_v     = (video_cfg_ch3.clipper_height << 8)/video_cfg_ch3.vout_t_height;	
	video_cfg(0x00,(unsigned char*)&video_cfg_ch0);
	video_cfg(0x20,(unsigned char*)&video_cfg_ch1);
	video_cfg(0x40,(unsigned char*)&video_cfg_ch2);
	video_cfg(0x60,(unsigned char*)&video_cfg_ch3);
}   
