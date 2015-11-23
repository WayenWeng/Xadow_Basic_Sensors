
#include "main.h"


unsigned char sys_time = 0;

unsigned char sys_gpio_test = 0;


int main(void)
{
	//RCC_Configuration();
	delay_init();
	sys_gpio_init();
	I2C_Configuration();
	IIC_Sw_Init();
	adxl345_init();
	tls2561t_init();
	lm75a_init();
	//sensor_stream_init();	
	delay_ms(1000);
	
	while(1)
	{		
		adxl345_get_xyz_data();
		tls2561t_get_light_data();
		if(sys_time >= 5)
		{	
				sys_time = 0;
				lm75a_get_temp_data();
		}
			
		GPIO_Test_for_TE();
		
		sys_time ++;
		delay_ms(50);
	}
}

void sys_gpio_init(void)
{
		GPIO_InitTypeDef        GPIO_InitStructure;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
		// For test
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_0);  //INT1 Êä³ö¸ß  
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void RCC_Configuration(void)
{
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);///*!< PLL input clock*12 */
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08){}
		RCC->CR &= 0xFFFEFFFF;
}

void GPIO_Test_for_TE(void)
{
		GPIO_InitTypeDef	GPIO_InitStructure;
	
		if(sys_gpio_test)
		{
				sys_gpio_test = 0;
				delay_ms(50);
			
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_9 | GPIO_Pin_10; // INT1, SCL, SDA output.
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
			
				GPIO_SetBits(GPIOA,GPIO_Pin_0); // INT1
				GPIO_SetBits(GPIOA,GPIO_Pin_9); // SCL
				GPIO_SetBits(GPIOA,GPIO_Pin_10); // SDA
			
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // INT2 input.
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
				GPIO_Init(GPIOA, &GPIO_InitStructure);

				while(1)
				{
						if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)) // Read INT2 port
						{
								GPIO_ResetBits(GPIOA,GPIO_Pin_0); // INT1 low
								GPIO_SetBits(GPIOA,GPIO_Pin_9); // SCL high
								GPIO_ResetBits(GPIOA,GPIO_Pin_10); // SDA low
						}
						else
						{
								GPIO_SetBits(GPIOA,GPIO_Pin_0); // INT1 high
								GPIO_ResetBits(GPIOA,GPIO_Pin_9); // SCL low
								GPIO_SetBits(GPIOA,GPIO_Pin_10); // SDA high
						}
						
						delay_ms(100);
				}
		}
}
