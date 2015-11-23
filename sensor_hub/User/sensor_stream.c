
#include "main.h"

/*
SENSOE_SCAN_STREAM sensor_scan =
{
    0x0000,    // Data flag
    0x00,      // Event flag
};

SENSOR_DATA_STREAM sensor_data[SENSOR_ID_MAX]=
{
    {SENSOR_TYPE_0, 0, SENSOR_ID_0, SENSOR_DATA_0},
    {SENSOR_TYPE_1, 0, SENSOR_ID_1, SENSOR_DATA_1},
    {SENSOR_TYPE_2, 0, SENSOR_ID_2, SENSOR_DATA_2},
    {SENSOR_TYPE_3, 0, SENSOR_ID_3, SENSOR_DATA_3},
    {SENSOR_TYPE_4, 0, SENSOR_ID_4, SENSOR_DATA_4},
};

SENSOR_EVENT_STREAM sensor_event[SENSOR_INDEX_MAX]=
{
    {EVENT_INDEX_0, EVENT_EXPRESSION_0, 0, 0 ,0},
    {EVENT_INDEX_1, EVENT_EXPRESSION_1, 0, 0 ,0},
    {EVENT_INDEX_2, EVENT_EXPRESSION_2, 0, 0 ,0},
    {EVENT_INDEX_3, EVENT_EXPRESSION_3, 0, 0 ,0},
    {EVENT_INDEX_4, EVENT_EXPRESSION_4, 0, 0 ,0},
    {EVENT_INDEX_5, EVENT_EXPRESSION_5, 0, 0 ,0},
    {EVENT_INDEX_6, EVENT_EXPRESSION_6, 0, 0 ,0},
    {EVENT_INDEX_7, EVENT_EXPRESSION_7, 0, 0 ,0},
};
*/

unsigned char sensor_scan[4] = { 0,0,0,SENSOR_ID_NUM };

unsigned char sensor_id_type[SENSOR_ID_MAX][4] = 
{
    { SENSOR_TYPE_0, 0, SENSOR_ID_0, 0 },
    { SENSOR_TYPE_1, 0, SENSOR_ID_1, 0 },
    { SENSOR_TYPE_2, 0, SENSOR_ID_2, 0 },
    { SENSOR_TYPE_3, 0, SENSOR_ID_3, 0 },
    { SENSOR_TYPE_4, 0, SENSOR_ID_4, 0 },
};

unsigned char sensor_data[SENSOR_ID_MAX][4] = 
{
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
};

unsigned char sensor_event[EVENT_INDEX_MAX][3] = 
{
  { EVENT_INDEX_0,EVENT_EXPRESSION_0,0x00 },
  { EVENT_INDEX_1,EVENT_EXPRESSION_1,0x00 },
  { EVENT_INDEX_2,EVENT_EXPRESSION_2,0x00 },
  { EVENT_INDEX_3,EVENT_EXPRESSION_3,0x00 },
  { EVENT_INDEX_4,EVENT_EXPRESSION_4,0x00 },
  { EVENT_INDEX_5,EVENT_EXPRESSION_5,0x00 },
  { EVENT_INDEX_6,EVENT_EXPRESSION_6,0x00 },
  { EVENT_INDEX_7,EVENT_EXPRESSION_7,0x00 },
};

unsigned char event_data[EVENT_INDEX_MAX][4] = 
{
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
  { 0,0,0,0 },
};

unsigned char sensor_time = 0;

unsigned int CCR1_Val = 50000;
unsigned int PrescalerValue = 0;
unsigned int capture = 0;

extern unsigned char sys_time;


void sensor_stream_init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* TIM3 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		/* Enable the TIM3 gloabal Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		/* Compute the prescaler value */
		PrescalerValue = (unsigned int) (SystemCoreClock  / 6000000) - 1;

		/* Time base configuration */
		TIM_TimeBaseStructure.TIM_Period = 65535;
		TIM_TimeBaseStructure.TIM_Prescaler = 0;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		/* Prescaler configuration */
		TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

		/* Output Compare Timing Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

		/* TIM Interrupts enable */
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
		/* TIM3 enable counter */
		TIM_Cmd(TIM3, ENABLE);
}

void TIM3_Call_Back(void)
{
		if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) // 50ms
		{
				TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
				capture = TIM_GetCapture1(TIM3);
				TIM_SetCompare1(TIM3, capture + CCR1_Val);
				sys_time ++;
		}
}
