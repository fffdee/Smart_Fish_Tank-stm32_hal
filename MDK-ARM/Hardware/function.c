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
unsigned char SettingFlag =0;
unsigned char MotorFlag1 =0;
unsigned char MotorFlag2 =0;
unsigned char MotorFlag3 =0;


void fun1(void)
{
		
		OLED_ShowString(32,0,"Main UI",16);
		OLED_ShowString(0,2,"ZD",16);
		OLED_ShowString(64,2,"Set",16);
		OLED_ShowString(0,6,"T",16);
		OLED_ShowString(64,6,"Set",16);
		OLED_ShowString(40,6,".",16);
		OLED_ShowString(0,4,"M1:",16);
		OLED_ShowString(40,4,"M2:",16);
		OLED_ShowString(80,4,"M3:",16);
		OLED_ShowNum(24,4,MotorFlag1,1,16);
		OLED_ShowNum(64,4,MotorFlag2,1,16);
		OLED_ShowNum(104,4,MotorFlag3,1,16);
	
		HAL_ADC_Start( &hadc);  //开启ADC
		
	  if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //如果转换结果完成
	  {
		  
		printf("the adc value is %d\r\n",HAL_ADC_GetValue(&hadc));//读取打印结果并用串口打印出来结果
		OLED_ShowNum(24,2,HAL_ADC_GetValue(&hadc),4,16);
	  }
	  delay_us(50000);
	 

		//HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);
		temp_T=DS18B20_Get_Temp();
		T=temp_T%100;
		OLED_ShowNum(24,6,temp_T/10,2,16);
		OLED_ShowNum(46,6,T,1,16);
	//	OLED_ShowString(8,2,Uart1_RxBuff,16);
		SettingFlag =1;
}

void fun2_1(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
			OLED_ClearSet();
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"Temp Set",16);
			OLED_ShowString(0,2,"->",16);
			OLED_ShowString(32,4,"ZD Set",16);
			OLED_ShowString(32,6,"Motor Test",16);
	

		
}


void fun2_2(void)
{
			SettingFlag =1;
			OLED_ClearSet();
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"Temp Set",16);
			OLED_ShowString(0,4,"->",16);
			OLED_ShowString(32,4,"ZD Set",16);
			OLED_ShowString(32,6,"Motor Test",16);
}


void fun2_3(void)
{
			
			OLED_ClearSet();
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"Temp Set",16);
			OLED_ShowString(0,6,"->",16);
			OLED_ShowString(32,4,"ZD Set",16);
			OLED_ShowString(32,6,"Motor Test",16);
}
void fun2_4(void)
{
			OLED_ClearSet();
			if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
			}
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"Temp Set",16);
			OLED_ShowString(0,6,"->",16);
			OLED_ShowString(32,4,"ZD Test",16);
			OLED_ShowString(32,6,"EXIT",16);
}

void fun3_1(void)
{
}

void fun3_2(void)
{
}

void fun3_3(void)
{
}





