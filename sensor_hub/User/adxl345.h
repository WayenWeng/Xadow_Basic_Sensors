
#ifndef __ADXL345_H__
#define __ADXL345_H__

#include "main.h"


#define ADXL345_IIC_ADR     0x53

/* ------- Register names ------- */
#define ADXL345_DEVID 0x00
#define ADXL345_RESERVED1 0x01
#define ADXL345_THRESH_TAP 0x1d
#define ADXL345_OFSX 0x1e
#define ADXL345_OFSY 0x1f
#define ADXL345_OFSZ 0x20
#define ADXL345_DUR 0x21
#define ADXL345_LATENT 0x22
#define ADXL345_WINDOW 0x23
#define ADXL345_THRESH_ACT 0x24
#define ADXL345_THRESH_INACT 0x25
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27
#define ADXL345_THRESH_FF 0x28
#define ADXL345_TIME_FF 0x29
#define ADXL345_TAP_AXES 0x2a
#define ADXL345_ACT_TAP_STATUS 0x2b
#define ADXL345_BW_RATE 0x2c
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_INT_ENABLE 0x2e
#define ADXL345_INT_MAP 0x2f
#define ADXL345_INT_SOURCE 0x30
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37
#define ADXL345_FIFO_CTL 0x38
#define ADXL345_FIFO_STATUS 0x39

#define ADXL345_2_G_FLAG	0x0200 // 2g
#define ADXL345_4_G_FLAG	0x0400 // 4g
#define ADXL345_8_G_FLAG	0x0800 // 8g
#define ADXL345_16_G_FLAG	0x1000 // 16g
#define ADXL345_MIN_BIT		4

#define ADXL345_DATA_FLAG ADXL345_2_G_FLAG


unsigned char adxl345_read_data(unsigned char Data_Adr);
void adxl345_write_data(unsigned char Data_Adr,unsigned char Data_Value);
void adxl345_init(void);
void adxl345_get_xyz_data(void);


#endif
