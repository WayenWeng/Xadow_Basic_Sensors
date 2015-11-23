
#include "main.h"
#include "i2csw.h"


GPIO_InitTypeDef        GPIO_I2C_InitStructure; 


/** 
  * @brief  IIC Init 
  * @param  A: 
  * @retval None 
  */  
void IIC_Sw_Init(void)  
{                          
    //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE );
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE );      
	
		// I2C SW SCL PA7
    GPIO_I2C_InitStructure.GPIO_Pin = I2C_SW_SCL_PIN;  
    GPIO_I2C_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //推挽输出  
    GPIO_I2C_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_I2C_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(I2C_SW_SCL_PORT, &GPIO_I2C_InitStructure);  
    GPIO_SetBits(I2C_SW_SCL_PORT,I2C_SW_SCL_PIN);  //PA7 输出高  
	
		// I2C SW SDA PB1
		GPIO_I2C_InitStructure.GPIO_Pin = I2C_SW_SDA_PIN;  
    GPIO_I2C_InitStructure.GPIO_Mode = GPIO_Mode_OUT ;   //推挽输出  
    GPIO_I2C_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_I2C_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(I2C_SW_SDA_PORT, &GPIO_I2C_InitStructure);  
    GPIO_SetBits(I2C_SW_SDA_PORT,I2C_SW_SDA_PIN);  //PB1 输出高  
}  
  
/** 
  * @brief  Set SDA Pin as Output Mode 
  * @retval None 
  */  
void SDA_OUT()    
{    
		GPIO_StructInit(&GPIO_I2C_InitStructure);    
		GPIO_I2C_InitStructure.GPIO_Pin   = I2C_SW_SDA_PIN;    
		GPIO_I2C_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;    
		GPIO_I2C_InitStructure.GPIO_OType = GPIO_OType_PP;    
		GPIO_I2C_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
		GPIO_Init(I2C_SW_SDA_PORT, &GPIO_I2C_InitStructure);    
}    
  
/** 
  * @brief  Set SDA Pin as Input Mode 
  * @retval None 
  */  
void SDA_IN()    
{    
  GPIO_StructInit(&GPIO_I2C_InitStructure);    
  GPIO_I2C_InitStructure.GPIO_Pin   = I2C_SW_SDA_PIN;    
  GPIO_I2C_InitStructure.GPIO_Mode  = GPIO_Mode_IN;  
  GPIO_I2C_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;// !!!  
  GPIO_I2C_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
  GPIO_Init(I2C_SW_SDA_PORT, &GPIO_I2C_InitStructure);    
}   

/** 
  * @brief  read input voltage from SDA pin 
  * @retval None 
  */  
unsigned char SDA_READ()  
{  
  return GPIO_ReadInputDataBit(I2C_SW_SDA_PORT, I2C_SW_SDA_PIN);  
}  
  
/** 
  * @brief  output high form SDA pin 
  * @retval None 
  */  
void IIC_SDA_1()  
{  
		GPIO_SetBits(I2C_SW_SDA_PORT, I2C_SW_SDA_PIN);  
}  
  
/** 
  * @brief  output low form SDA pin 
  * @retval None 
  */  
void IIC_SDA_0()  
{  
		GPIO_ResetBits(I2C_SW_SDA_PORT, I2C_SW_SDA_PIN);  
}  
  
/** 
  * @brief  output high form SCL pin 
  * @retval None 
  */  
void IIC_SCL_1()  
{  
		GPIO_SetBits(I2C_SW_SCL_PORT, I2C_SW_SCL_PIN);  
}  
  
/** 
  * @brief  output LOW form SCL pin 
  * @retval None 
  */  
void IIC_SCL_0()  
{  
		GPIO_ResetBits(I2C_SW_SCL_PORT, I2C_SW_SCL_PIN);    
} 

/** 
* @brief  Simulate IIC conmunication :Create Start signal 
  * @retval None 
  */  
void IIC_Start(void)  
{  
    IIC_SDA_1();
		delay_us(i2cDelay);	
    IIC_SCL_1();  
    delay_us(i2cDelay);  
    IIC_SDA_0();   //START:when CLK is high,DATA change form high to low   
    delay_us(i2cDelay);  
    IIC_SCL_0();   //hold scl line, prepare to transmit data  
		//delay_us(i2cDelay);
}       
  
/** 
  * @brief  Simulate IIC conmunication : Create Stop signal 
  * @retval None 
  */  
void IIC_Stop(void)  
{  
    IIC_SDA_0();  //STOP:when CLK is high DATA change form low to high  
    delay_us(i2cDelay);  
    IIC_SCL_1();
		delay_us(i2cDelay);
    IIC_SDA_1();  //indicate transmit over  
    //delay_us(i2cDelay);                               
}
  
/** 
	* @brief  Simulate IIC conmunication : wait for target device's ACK 
	* @retval ACK (0) : receive success 
	* @retval NACK(1) : receive unsuccess 
  */  
unsigned char IIC_Wait_Ack(void)  
{  
    unsigned char ucErrTime = 0;
	
    SDA_IN();      //set as input mode
	       
    IIC_SCL_1();  
		delay_us(i2cDelay);      
    while(SDA_READ())  
    {  
        ucErrTime++;  
        if(ucErrTime > 250)  
        {  
            //IIC_Stop();  
            //return 1;
						break;					
        }  
    }
    IIC_SCL_0(); //release scl line
		//delay_us(i2cDelay);
		
		SDA_OUT(); 
		
    if(ucErrTime > 250)return 0;
		else return 1;
}  
  
/** 
  * @brief  Simulate IIC conmunication : make an ACK 
  * @retval None 
  */  
void IIC_Ack(void)  
{  /*
    IIC_SCL_0();  
    delay_us(i2cDelay); 
    IIC_SDA_0();  
    delay_us(i2cDelay);  
    IIC_SCL_1();  
    //delay_us(i2cDelay); 
*/	
    IIC_SDA_0();  
		delay_us(i2cDelay); 
	  IIC_SCL_1();  
    delay_us(i2cDelay); 
    IIC_SCL_0();  
		//delay_us(i2cDelay); 
}  
  
/** 
  * @brief  Simulate IIC conmunication : don't make an ACK 
  * @retval None 
  */  
void IIC_NAck(void)  
{  /*
    IIC_SCL_0();  
    delay_us(i2cDelay); 
    IIC_SDA_1();  
    delay_us(i2cDelay);  
    IIC_SCL_1();  
    //delay_us(i2cDelay); 
		*/
		IIC_SDA_1();  
		delay_us(i2cDelay); 
	  IIC_SCL_1();  
    delay_us(i2cDelay); 
    IIC_SCL_0();  
		//delay_us(i2cDelay); 
}  

/** 
  * @brief  Simulate IIC conmunication : Transmit one byte Data 
  * @param  txd: data to be transmit 
  * @retval None 
  */  
void IIC_Send_Byte(unsigned char txd)  
{                          
  unsigned char i;
  
  for(i = 0; i < 8; i++)  
  {                
    if(txd & 0x80)  
    {  
      IIC_SDA_1();  
    }  
    else  
    {  
      IIC_SDA_0();  
    }  
    txd <<= 1;        
    //delay_us(i2cDelay);     
    IIC_SCL_1();  
    delay_us(i2cDelay);   
    IIC_SCL_0();      
    //delay_us(i2cDelay);  
  }    
}       
  
/** 
  * @brief  Simulate IIC conmunication : Receive one byte Data 
  * @param  ack: Whether transmit ACK 
  * @retval the data have been receive 
  */  
unsigned char IIC_Read_Byte(void)  
{  
    unsigned char i, res = 0;
	
    SDA_IN();               //SDA input mode 

		for(i = 0; i < 8; i++)  
    {    
				IIC_SCL_1();
				delay_us(i2cDelay); 
				res <<= 1;  
				if(SDA_READ())  
				{  
					res++;   
				}
				IIC_SCL_0();
				//delay_us(i2cDelay);   
		}           

		SDA_OUT(); 
		
		return res;  
}  
