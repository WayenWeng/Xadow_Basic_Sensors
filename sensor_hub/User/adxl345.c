
#include "adxl345.h"


extern unsigned char sensor_scan[4];
extern unsigned char sensor_data[SENSOR_ID_MAX][4];


unsigned char adxl345_read_data(unsigned char Data_Adr)
{
    unsigned char Data = 0; 
    
    IIC_Start(); 
    IIC_Send_Byte((ADXL345_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
    IIC_Start(); 
    IIC_Send_Byte((ADXL345_IIC_ADR<<1) | 0x01);
    IIC_Wait_Ack();
    Data = IIC_Read_Byte(); 
		IIC_Ack();
    IIC_Stop(); 
    
    return Data;
}

void adxl345_read_data_number(unsigned char Data_Adr, unsigned char Data_len, unsigned char *Buffer)
{
    unsigned char i; 
    
    IIC_Start(); 
    IIC_Send_Byte((ADXL345_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
		IIC_Start(); 
		IIC_Send_Byte((ADXL345_IIC_ADR<<1) | 0x01);
		IIC_Wait_Ack();
	
		for(i=0;i<(Data_len - 1);i++)
		{
				
				Buffer[i] = IIC_Read_Byte(); 
				IIC_Ack();
		}
		Buffer[i] = IIC_Read_Byte(); 
		IIC_NAck();
		
    IIC_Stop(); 
}

void adxl345_write_data(unsigned char Data_Adr,unsigned char Data_Value)
{
    IIC_Start(); 
    IIC_Send_Byte((ADXL345_IIC_ADR<<1) | 0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Adr); 
    IIC_Wait_Ack();
    IIC_Send_Byte(Data_Value); 
    IIC_Wait_Ack();
    IIC_Stop(); 
}

void adxl345_init(void)
{
		adxl345_write_data(ADXL345_DATA_FORMAT, 0x08); // 2g		
		adxl345_write_data(ADXL345_BW_RATE, 0x0a); // 100 Hz

    adxl345_write_data(ADXL345_POWER_CTL, 0);      
    adxl345_write_data(ADXL345_POWER_CTL, 16);
		adxl345_write_data(ADXL345_POWER_CTL, 8);   // Power on and measure
		//adxl345_write_data(ADXL345_POWER_CTL, 4);   // Power down  
}

void adxl345_get_xyz_data(void)
{  
    unsigned DataLast = 0;
    unsigned DataNew = 0;
    unsigned char DataBuf[6] = {0};
    unsigned long ulData = 0;
    float fData = 0;
    long lData = 0;
		
		unsigned char temp = 0;
    
    //adxl345_write_data(ADXL345_POWER_CTL, 8); // Power on and measure
    //delay_ms(5);
		/*
    DataBuf[1] = adxl345_read_data(ADXL345_DATAX0);
		DataBuf[0] = adxl345_read_data(ADXL345_DATAX1);

    DataBuf[3] = adxl345_read_data(ADXL345_DATAY0);
		DataBuf[2] = adxl345_read_data(ADXL345_DATAY1);
    
    DataBuf[5] = adxl345_read_data(ADXL345_DATAZ0);
		DataBuf[4] = adxl345_read_data(ADXL345_DATAZ1);
		*/
		adxl345_read_data_number(ADXL345_DATAX0, 6, DataBuf);
		
		temp = DataBuf[0];
		DataBuf[0] = DataBuf[1];
		DataBuf[1] = temp;
		
		temp = DataBuf[2];
		DataBuf[2] = DataBuf[3];
		DataBuf[3] = temp;
		
		temp = DataBuf[4];
		DataBuf[4] = DataBuf[5];
		DataBuf[5] = temp;
		
    // Get X value
    DataLast = sensor_data[SENSOR_ID_0][1];
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_0][0];
    
    DataNew = DataBuf[0];
    DataNew <<= 8;
    DataNew |= DataBuf[1];  
		
		sensor_scan[0] |= 0x01;
    
    if(DataLast != DataNew)
    {
        if((DataNew & ADXL345_DATA_FLAG) == ADXL345_DATA_FLAG)
        {
            sensor_data[SENSOR_ID_0][3] = 0xff;
            sensor_data[SENSOR_ID_0][2] = 0xff;
        }
        else
        {
            sensor_data[SENSOR_ID_0][3] = 0;
            sensor_data[SENSOR_ID_0][2] = 0;
        }
        sensor_data[SENSOR_ID_0][0] = (DataNew & 0xff);
        sensor_data[SENSOR_ID_0][1] = ((DataNew >> 8) & 0xff);
        //sensor_scan[0] |= 0x01;
        
        fData = 0;
        ulData = 0;
        
        ulData |=sensor_data[SENSOR_ID_0][3];
        ulData <<= 8; 
        ulData |=sensor_data[SENSOR_ID_0][2];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_0][1];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_0][0];
        
        if((ulData & ADXL345_DATA_FLAG) == 0)
				{
					ulData &= (ADXL345_DATA_FLAG - 1);
					fData = ulData * ADXL345_MIN_BIT;
					lData = (long)fData;
				}
				else
				{
					ulData ^= 0xffffffff;
					ulData += 1;
					ulData &= (ADXL345_DATA_FLAG - 1);
					fData = ulData * ADXL345_MIN_BIT;
					lData = (long)fData;
					lData = 0 - lData;
				}
        
        sensor_data[SENSOR_ID_0][0] = lData & 0xff;
        sensor_data[SENSOR_ID_0][1] = (lData >> 8) & 0xff;
        sensor_data[SENSOR_ID_0][2] = (lData >> 16) & 0xff;
        sensor_data[SENSOR_ID_0][3] = (lData >> 24) & 0xff;    
    }
    else
    {
        //sensor_scan[0] &= (~0x01);
    }
    
    // Get Y value
    DataLast = sensor_data[SENSOR_ID_1][1];
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_1][0];
    
    DataNew = DataBuf[2];
    DataNew <<= 8; 
    DataNew |= DataBuf[3]; 
    
		sensor_scan[0] |= 0x02;
		
    if(DataLast != DataNew)
    {
        if((DataNew & ADXL345_DATA_FLAG) == ADXL345_DATA_FLAG)
        {
            sensor_data[SENSOR_ID_1][3] = 0xff;
            sensor_data[SENSOR_ID_1][2] = 0xff;
        }
        else
        {
            sensor_data[SENSOR_ID_1][3] = 0;
            sensor_data[SENSOR_ID_1][2] = 0;
        }
        sensor_data[SENSOR_ID_1][0] = (DataNew & 0xff);
        sensor_data[SENSOR_ID_1][1] = ((DataNew >> 8) & 0xff);
        //sensor_scan[0] |= 0x02;
        
        fData = 0;
        ulData = 0;
        
        ulData |=sensor_data[SENSOR_ID_1][3];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_1][2];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_1][1];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_1][0];
        
        if((ulData & ADXL345_DATA_FLAG) == 0)
				{
					ulData &= (ADXL345_DATA_FLAG - 1);
					fData = ulData * ADXL345_MIN_BIT;
					lData = (long)fData;
				}
				else
				{
					ulData ^= 0xffffffff;
					ulData += 1;
					ulData &= (ADXL345_DATA_FLAG - 1);
					fData = ulData * ADXL345_MIN_BIT;
					lData = (long)fData;
					lData = 0 - lData;
				}
        
        sensor_data[SENSOR_ID_1][0] = lData & 0xff;
        sensor_data[SENSOR_ID_1][1] = (lData >> 8) & 0xff;
        sensor_data[SENSOR_ID_1][2] = (lData >> 16) & 0xff;
        sensor_data[SENSOR_ID_1][3] = (lData >> 24) & 0xff;   
    }
    else
    {
        //sensor_scan[0] &= (~0x02);
    }
    
    // Get Z value
    DataLast = sensor_data[SENSOR_ID_2][1];
    DataLast <<= 8;
    DataLast |= sensor_data[SENSOR_ID_2][0];

    DataNew = DataBuf[4];
    DataNew <<= 8;
    DataNew |= DataBuf[5];
		
    sensor_scan[0] |= 0x04;
		
    if(DataLast != DataNew)
    {
        if((DataNew & ADXL345_DATA_FLAG) == ADXL345_DATA_FLAG)
        {
            sensor_data[SENSOR_ID_2][3] = 0xff;
            sensor_data[SENSOR_ID_2][2] = 0xff;
        }
        else
        {
            sensor_data[SENSOR_ID_2][3] = 0;
            sensor_data[SENSOR_ID_2][2] = 0;
        }
        sensor_data[SENSOR_ID_2][0] = (DataNew & 0xff);
        sensor_data[SENSOR_ID_2][1] = ((DataNew >> 8) & 0xff);
        //sensor_scan[0] |= 0x04;
        
        fData = 0;
        ulData = 0;
        
        ulData |=sensor_data[SENSOR_ID_2][3];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_2][2];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_2][1];
        ulData <<= 8;
        ulData |=sensor_data[SENSOR_ID_2][0];
        
        if((ulData & ADXL345_DATA_FLAG) == 0)
				{
					ulData &= (ADXL345_DATA_FLAG - 1);
					fData = ulData * ADXL345_MIN_BIT;
					lData = (long)fData;
				}
				else
				{
					ulData ^= 0xffffffff;
					ulData += 1;
					ulData &= (ADXL345_DATA_FLAG - 1);
					fData = ulData * ADXL345_MIN_BIT;
					lData = (long)fData;
					lData = 0 - lData;
				}
        
        sensor_data[SENSOR_ID_2][0] = lData & 0xff;
        sensor_data[SENSOR_ID_2][1] = (lData >> 8) & 0xff;
        sensor_data[SENSOR_ID_2][2] = (lData >> 16) & 0xff;
        sensor_data[SENSOR_ID_2][3] = (lData >> 24) & 0xff;  
    }
    else
    {
        //sensor_scan[0] &= (~0x04);
    }
    
    //adxl345_write_data(ADXL345_POWER_CTL, 4);   // Power down
}
