
#ifndef __SENSOR_STREAM_H__
#define __SENSOR_STREAM_H__


#define ADXL345_X_TYPE         0x01
#define ADXL345_Y_TYPE         0x01
#define ADXL345_Z_TYPE         0x01
#define TLS2561T_LIGHT_TYPE    0x00
#define LM75A_TEMP_TYPE        0x01

#define SENSOR_ID_NUM        0x05
#define ADXL345_X_ID         0x00
#define ADXL345_Y_ID         0x01
#define ADXL345_Z_ID         0x02
#define TLS2561T_LIGHT_ID    0x03
#define LM75A_TEMP_ID        0x04

#define EXPRESSION_MAX             0x03
#define EXPRESSION_EQUAL_TO        0x00
#define EXPRESSION_GREATER_THAN    0x01
#define EXPRESSION_LESS_THAN       0x02
#define EXPRESSION_NULL            0xff

#define SENSOR_TYPE_0      ADXL345_X_TYPE
#define SENSOR_TYPE_1      ADXL345_Y_TYPE
#define SENSOR_TYPE_2      ADXL345_Z_TYPE
#define SENSOR_TYPE_3      TLS2561T_LIGHT_TYPE
#define SENSOR_TYPE_4      LM75A_TEMP_TYPE
#define SENSOR_TYPE_NULL   0xff

#define SENSOR_ID_MAX    0x05
#define SENSOR_ID_0      ADXL345_X_ID
#define SENSOR_ID_1      ADXL345_Y_ID
#define SENSOR_ID_2      ADXL345_Z_ID
#define SENSOR_ID_3      TLS2561T_LIGHT_ID
#define SENSOR_ID_4      LM75A_TEMP_ID
#define SENSOR_ID_NULL   0xff


#define EVENT_INDEX_MAX    0x08
#define EVENT_INDEX_0      SENSOR_ID_NULL
#define EVENT_INDEX_1      SENSOR_ID_NULL
#define EVENT_INDEX_2      SENSOR_ID_NULL
#define EVENT_INDEX_3      SENSOR_ID_NULL
#define EVENT_INDEX_4      SENSOR_ID_NULL
#define EVENT_INDEX_5      SENSOR_ID_NULL
#define EVENT_INDEX_6      SENSOR_ID_NULL
#define EVENT_INDEX_7      SENSOR_ID_NULL

#define EVENT_EXPRESSION_0      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_1      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_2      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_3      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_4      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_5      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_6      EXPRESSION_GREATER_THAN
#define EVENT_EXPRESSION_7      EXPRESSION_GREATER_THAN

/*
typedef struct
{
    unsigned int  data_flag;
    unsigned char event_flag;  
    unsigned char decive address; 
}SENSOE_SCAN_STREAM;

typedef struct
{
    unsigned char sensor_type;
    unsigned char reserved;
    unsigned int  sensor_id;  
    union
    {
        unsigned long u32,
        long          i32,
        float         f32,
        void          *p,
    }
}SENSOR_DATA_STREAM;

typedef struct
{
    unsigned char data_index;
    unsigned char expression_id;
    unsigned char requirement;
    unsigned char reserved1;
    union
    {
        unsigned long u32,
        long          i32,
        float         f32,
        void          *p,
    }
}SENSOR_EVENT_STREAM;
*/
extern unsigned char sensor_scan[4];
extern unsigned char sensor_id_type[SENSOR_ID_MAX][4];
extern unsigned char sensor_data[SENSOR_ID_MAX][4];


void sensor_stream_init(void);
void TIM3_Call_Back(void);
	

#endif
