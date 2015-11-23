
#ifndef __I2CSW_H__
#define __I2CSW_H__


#define I2C_SW_SCL_PORT	GPIOA
#define I2C_SW_SCL_PIN	GPIO_Pin_7

#define I2C_SW_SDA_PORT	GPIOB
#define I2C_SW_SDA_PIN	GPIO_Pin_1

#define i2cDelay	0


void IIC_Sw_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(void);


#endif
