///#define tw2867_addr 0x50
#ifndef __TW2867_H
#define __TW2867_H
#define RSTN_GPIO GPIOA
#define RSTN_PIN GPIO_Pin_8

void tw2867_init(void);
void tw2867_cfg(void);

#endif 
