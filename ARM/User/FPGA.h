#ifndef __FPGA_H__
#define __FPGA_H__
#define FPGA_DEV_ADDR 0x78
#define LAY0 0x1a
#define LAY1 0x2a
#define LAY2 0x3a
#define LAY3 0x4a
#define SRC0 0x0
#define SRC1 0x1
#define SRC2 0x2
#define SRC3 0x3
#define SRC4 0x4
#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为4字节对齐
typedef struct{
	unsigned short vin_s_width    ;
	unsigned short vin_s_height   ;
	unsigned short clipper_left   ;
	unsigned short clipper_width  ;
	unsigned short clipper_top    ;
	unsigned short clipper_height ;
	unsigned short vout_t_width   ;
	unsigned short vout_t_height  ;
	unsigned char  alpha          ;
	unsigned short disp_top       ;
	unsigned short disp_left      ;
	unsigned char  source         ;
	unsigned short scaler_k_h     ;
	unsigned short scaler_k_v     ;	
} VIDEO_CFG;
#pragma pack(pop)//恢复对齐状态
void Init_FPGA(void);
void config_disp_src(unsigned char lay, unsigned char src);
void disp_one(unsigned char ch);
void  disp_four(unsigned char ch0,unsigned char ch1,unsigned char ch2,unsigned char ch3);
void  disp_pip(unsigned char ch0,unsigned char ch1);
unsigned char get_state(void);
#endif
