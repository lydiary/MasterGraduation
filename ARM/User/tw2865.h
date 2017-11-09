#ifndef __TW2865_H__
#define __TW2865_H__

#include "I2C_Driver.h"



#define TW2865_NTSC	        1
#define TW2865_PAL	        2
#define TW2865_AUTO	        3
#define TW2865_INVALID_MODE	4

typedef struct _tw2865_video_norm
{
    unsigned int chip;
    unsigned int mode;
} tw2865_video_norm;


#define TW2865_1D1_MODE     0
#define TW2865_2D1_MODE     1
#define TW2865_4CIF_MODE    2
#define TW2865_4D1_MODE     3

typedef struct _tw2865_work_mode
{
    unsigned int chip;
    unsigned int mode;
} tw2865_work_mode;


typedef struct _tw2865_video_loss
{
    unsigned int chip;
    unsigned int ch;
    unsigned int is_lost;
} tw2865_video_loss;


#define TW2865_SET_HUE          0x01
#define TW2865_SET_CONTRAST     0x02
#define TW2865_SET_BRIGHT       0x04
#define TW2865_SET_SATURATION   0x08

typedef struct _tw2865_image_adjust
{
    unsigned char chip;
    unsigned char chn;
	unsigned char hue;
	unsigned char contrast;
	unsigned char brightness;
	unsigned char saturation;
	unsigned long item_sel;/* use such as "TW2865_SET_HUE|TW2865_SET_CONTRAST" */
} tw2865_image_adjust;


typedef struct _tw2865_audio_format
{
	unsigned char format;   /* 0:i2s; 1:dsp */
} tw2865_audio_format;

typedef struct _tw2865_audio_output
{
	unsigned char PlaybackOrLoop;   /*0:playback; 1:loop*/
	unsigned char channel;          /*[0, 15]*/
	unsigned char reserve[6];
} tw2865_audio_output;


typedef enum _tw2865_audio_samplerate
{
	TW2865_SAMPLE_RATE_8000,
	TW2865_SAMPLE_RATE_16000,
	TW2865_SAMPLE_RATE_32000,
	TW2865_SAMPLE_RATE_44100,
	TW2865_SAMPLE_RATE_48000,
	TW2865_SAMPLE_RATE_BUTT
} tw2865_audio_samplerate;

typedef struct _tw2865_audio_playback
{
    unsigned int chip;
    unsigned int chn;
} tw2865_audio_playback;

typedef struct _tw2865_audio_da_mute
{
    unsigned int chip;
} tw2865_audio_da_mute;

typedef struct _ttw2865_audio_da_volume
{
    unsigned int chip;
    unsigned int volume;
} tw2865_audio_da_volume;

typedef enum _tw2865_audio_bitwidth
{
	TW2865_AUDIO_BITWIDTH_16 = 0,
	TW2865_AUDIO_BITWIDTH_8 = 1,
	TW2865_AUDIO_BITWIDTH_BUTT
} tw2865_audio_bitwidth;

int tw2865_set_audio_samplerate(tw2865_audio_samplerate samplerate);
int tw2865_set_audio_format(tw2865_audio_format audio_format);
bool tw2865_comm_init(unsigned char addr);
void tw2865_video_init(unsigned char addr, unsigned char mode);
int tw2865_set_work_mode_1d1(unsigned chip_addr, unsigned char ch);
#endif

