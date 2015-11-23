
#include "tls2561t.h"


extern unsigned char sensor_scan[4];
extern unsigned char sensor_data[SENSOR_ID_MAX][4];

unsigned char tls256l_read_data(unsigned char Data_Adr)
{
    unsigned char Data = 0; 

    IIC_Start(); 
    IIC_Send_Byte((TLS2561T_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
    IIC_Start(); 
    IIC_Send_Byte((TLS2561T_IIC_ADR<<1) | 0x01);
    IIC_Wait_Ack();
    Data = IIC_Read_Byte(); 
		IIC_Ack();
    IIC_Stop(); 
    
    return Data;
}

void tls2561t_write_data(unsigned char Data_Adr,unsigned char Data_Value)
{
    IIC_Start(); 
    IIC_Send_Byte((TLS2561T_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Value); 
    IIC_Wait_Ack();
    IIC_Stop(); 
}

void tls2561t_init(void)
{    
    tls2561t_write_data(TSL2561_Control,0x03);  // POWER UP
    tls2561t_write_data(TSL2561_Timing,0x00);   //No High Gain (1x), integration time of 13ms
    tls2561t_write_data(TSL2561_Interrupt,0x00);
    tls2561t_write_data(TSL2561_Control,0x00);  // POWER Down
}

unsigned long calculate_lux(unsigned char iGain, unsigned char tInt,unsigned char iType, unsigned int ch0, unsigned int ch1)
{
    
    unsigned long chScale;
    unsigned long channel0;
    unsigned long channel1;
     long ratio;
    unsigned long ratio1;
     int b;
     int m;
     long temp;
    unsigned long lux;
    
    switch (tInt)
    {
        case 0:  // 13.7 msec
        chScale = CHSCALE_TINT0;
        break;
        case 1: // 101 msec
        chScale = CHSCALE_TINT1;
        break;
        default: // assume no scaling
        chScale = (1 << CH_SCALE);
        break;
    }
    
    // scale 1X to 16X
    if (!iGain)  chScale = chScale << 4; 
    
    // scale the channel values
    channel0 = (ch0 * chScale) >> CH_SCALE;
    channel1 = (ch1 * chScale) >> CH_SCALE;

    ratio1 = 0;
    if (channel0!= 0) ratio1 = (channel1 << (RATIO_SCALE+1))/channel0;
    
    // round the ratio value
    ratio = (ratio1 + 1) >> 1;

    switch (iType)
    {
        case 0: // T package
        if ((ratio >= 0) && (ratio <= K1T))
        {b=B1T; m=M1T;}
        else if (ratio <= K2T)
        {b=B2T; m=M2T;}
        else if (ratio <= K3T)
        {b=B3T; m=M3T;}
        else if (ratio <= K4T)
        {b=B4T; m=M4T;}
        else if (ratio <= K5T)
        {b=B5T; m=M5T;}
        else if (ratio <= K6T)
        {b=B6T; m=M6T;}
        else if (ratio <= K7T)
        {b=B7T; m=M7T;}
        else if (ratio > K8T)
        {b=B8T; m=M8T;}
        break;
        
        case 1:// CS package
        if ((ratio >= 0) && (ratio <= K1C))
        {b=B1C; m=M1C;}
        else if (ratio <= K2C)
        {b=B2C; m=M2C;}
        else if (ratio <= K3C)
        {b=B3C; m=M3C;}
        else if (ratio <= K4C)
        {b=B4C; m=M4C;}
        else if (ratio <= K5C)
        {b=B5C; m=M5C;}
        else if (ratio <= K6C)
        {b=B6C; m=M6C;}
        else if (ratio <= K7C)
        {b=B7C; m=M7C;}
    }
    
    temp = ((channel0*b)-(channel1*m));
    
    if(temp < 0)temp = 0;
    temp += (1<<(LUX_SCALE-1));
    
    // strip off fractional portion
    lux = temp >> LUX_SCALE;
    return (lux);
 }

void tls2561t_get_light_data(void)
{
    unsigned int ch0;
    unsigned int ch1;
    unsigned long lux;
    unsigned long DataLast = 0;
    unsigned long DataNew = 0;
    
    tls2561t_write_data(TSL2561_Control,0x03);  // POWER UP
    delay_ms(14);

    ch0 = tls256l_read_data(TSL2561_Channal0H);
    ch0 <<= 8;
    ch0 |= tls256l_read_data(TSL2561_Channal0L);
    
    //read two bytes from registers 0x0E and 0x0F
    ch1 = tls256l_read_data(TSL2561_Channal1H);
    ch1 <<= 8;
    ch1 = tls256l_read_data(TSL2561_Channal1L);
    
    tls2561t_write_data(TSL2561_Control,0x00);  // POWER Down
    
    if(ch0/ch1 < 2 && ch0 > 4900)
    {
        //ch0 out of range, but ch1 not. the lux is not valid in this situation.
        sensor_data[SENSOR_ID_3][3] = 0;
        sensor_data[SENSOR_ID_3][2] = 0;
        sensor_data[SENSOR_ID_3][1] = 0;
        sensor_data[SENSOR_ID_3][0] = 0;
        return;
    }

    lux = calculate_lux(0, 0, 0, ch0, ch1);
    
    DataLast |= sensor_data[SENSOR_ID_3][3];
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_3][2];
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_3][1];
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_3][0];
    
    DataNew = lux;
    
    sensor_scan[0] |= 0x08;
    
    if(DataLast != DataNew)
    {
        sensor_data[SENSOR_ID_3][0] = (DataNew & 0xff);
        sensor_data[SENSOR_ID_3][1] = ((DataNew >> 8) & 0xff);
        sensor_data[SENSOR_ID_3][2] = ((DataNew >> 16) & 0xff);
        sensor_data[SENSOR_ID_3][3] = ((DataNew >> 24) & 0xff);
        //sensor_scan[0] |= 0x08;
    }
    else
    {
        //sensor_scan[0] &= (~0x08);
    }
    
    tls2561t_write_data(TSL2561_Control,0x00);  // POWER Down
}

