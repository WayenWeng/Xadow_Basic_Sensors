
#include "lm75a.h"


extern unsigned char sensor_scan[4];
extern unsigned char sensor_data[SENSOR_ID_MAX][4];


unsigned int lm75a_read_data(unsigned char Data_Adr)
{
    unsigned int Data = 0; 
    
    IIC_Start(); 
    IIC_Send_Byte((LM75A_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
    IIC_Start(); 
    IIC_Send_Byte((LM75A_IIC_ADR<<1) | 0x01);
    IIC_Wait_Ack();
    
    switch(Data_Adr)
    {   
        case LM75A_TEMP:
        case LM75A_THYST:
        case LM75A_TOS:
        Data = IIC_Read_Byte(); 
				IIC_Ack();
        Data <<= 8;
        Data |= IIC_Read_Byte();
				IIC_Ack();				
        Data >>= 7;
        break;
        
        case LM75A_CONF:
        //case LM75A_ID:
        Data = IIC_Read_Byte(); 
				IIC_Ack();	
        break;
        
        default:
        break;
    }
    
    IIC_Stop(); 
    
    return Data;
}

void lm75a_write_data(unsigned char Data_Adr,unsigned int Data_Value)
{    
    IIC_Start(); 
    IIC_Send_Byte((LM75A_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
    
    switch(Data_Adr)
    {
        case LM75A_THYST:
        case LM75A_TOS:
        if(Data_Value > 0x1ff)Data_Value = 0x1fff;
        IIC_Send_Byte(Data_Value >> 1); 
        IIC_Wait_Ack();
        IIC_Send_Byte((Data_Value & 0x01) << 7); 
        IIC_Wait_Ack();
        break;
        
        case LM75A_CONF:
        IIC_Send_Byte(Data_Value); 
        IIC_Wait_Ack();
        break;
        
        default:
        break;
    }
    
    IIC_Stop(); 
}

void lm75a_init(void)
{
    lm75a_write_data(LM75A_CONF,0x00);   // Power on
    //lm75a_write_data(LM75A_CONF,0x01);   // Power off
		//temperature conversion time need 100 ms
}

void lm75a_get_temp_data(void)
{   
    unsigned int DataNew = 0;
    unsigned int DataLast = 0;
    
    //lm75a_write_data(LM75A_CONF,0x00);   // Power on
    //delay_ms(100);
    
    DataLast |= (sensor_data[SENSOR_ID_4][1] & 0x01);
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_4][0];  

    DataNew = lm75a_read_data(LM75A_TEMP);
    
		sensor_scan[0] |= 0x10;
	
    if(DataLast != DataNew)
    {
        sensor_data[SENSOR_ID_4][0] = (DataNew & 0xff) / 2;
        sensor_data[SENSOR_ID_4][1] = (DataNew >> 8);
        //sensor_scan[0] |= 0x10;
        if(sensor_data[4][1] & 0x01)
        {
            sensor_data[SENSOR_ID_4][1] |= 0xfe;
            sensor_data[SENSOR_ID_4][2] = 0xff;
            sensor_data[SENSOR_ID_4][3] = 0xff;
        }
        else
        {
            sensor_data[SENSOR_ID_4][1] &= (~0xfe);
            sensor_data[SENSOR_ID_4][2] = 0;
            sensor_data[SENSOR_ID_4][3] = 0;
        }
    }
    else
    {
        //sensor_scan[0] &= (~0x10);
    }
    
    //lm75a_write_data(LM75A_CONF,0x01);   // Power off
}
