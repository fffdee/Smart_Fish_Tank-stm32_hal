#include "function.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "ds18b20.h"
#include "uarst1.h"
#include "oled.h"
#include "tsw_30.h"

extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart1;
unsigned char Uart1_RxBuff[256]="ready recive";
short temp_T=0;
int T,i;
unsigned char  rcData = 's';
uint16_t ADC_Value;  //ADC值 
void fun1(void)
{
		HAL_ADC_Start( &hadc);  //开启ADC
	  if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //如果转换结果完成
	  {
		  
		printf("the adc value is %d\r\n",HAL_ADC_GetValue(&hadc));//读取打印结果并用串口打印出来结果
		OLED_ShowNum(90,4,HAL_ADC_GetValue(&hadc),4,16);
	  }
	  delay_us(50000);
	  delay_us(50000);

		HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);
		temp_T=DS18B20_Get_Temp();
		T=temp_T%100;
		OLED_ShowNum(32,6,temp_T/10,2,16);
		OLED_ShowNum(54,6,T,1,16);
		OLED_ShowString(8,2,Uart1_RxBuff,16);
}

void fun2(void)
{
}

void fun3(void)
{
}

void fun1_1(void)
{
}

void fun1_2(void)
{
}

void fun1_3(void)
{
}

void fun2_1(void)
{
}

void fun3_1(void)
{
}

void fun1_1_0(void)
{
}

void fun1_2_0(void)
{
}




