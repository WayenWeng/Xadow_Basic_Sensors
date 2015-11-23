
#ifndef __LM75A_H__
#define __LM75A_H__

#include "main.h"


#define LM75A_IIC_ADR     0x48

/* ------- Register names ------- */
#define LM75A_TEMP       0x00
#define LM75A_CONF       0x01
#define LM75A_THYST      0x02
#define LM75A_TOS        0x03
#define LM75A_ID         0x07


unsigned int lm75a_read_data(unsigned char Data_Adr);
void lm75a_write_data(unsigned char Data_Adr,unsigned int Data_Value);
void lm75a_init(void);
void lm75a_get_temp_data(void);


#endif
