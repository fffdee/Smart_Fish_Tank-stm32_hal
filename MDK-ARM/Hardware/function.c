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
unsigned char SettingFlag =1;
unsigned char MotorFlag1 =0;
unsigned char MotorFlag2 =0;
unsigned char MotorFlag3 =0;
unsigned char ExtTemp =25;
unsigned int ExtZD =3400;

void fun1(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
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
		OLED_ShowNum(96,6,ExtTemp,2,16);
	  OLED_ShowNum(96,2,ExtZD,4,16);
		//HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);
		temp_T=DS18B20_Get_Temp();
		T=temp_T%100;
		OLED_ShowNum(24,6,temp_T/10,2,16);
		OLED_ShowNum(46,6,T,1,16);
	//	OLED_ShowString(8,2,Uart1_RxBuff,16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
		}
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
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
			}
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

    {
					
					SettingFlag =1;
          //while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

     }
		
}


void fun2_2(void)
{
			if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		 }
			OLED_ClearSet();
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"Temp Set",16);
			OLED_ShowString(0,4,"->",16);
			OLED_ShowString(32,4,"ZD Set",16);
			OLED_ShowString(32,6,"Motor Test",16);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
			}
}


void fun2_3(void)
{
			if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		 } 
			OLED_ClearSet();
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"Temp Set",16);
			OLED_ShowString(0,6,"->",16);
			OLED_ShowString(32,4,"ZD Set",16);
			OLED_ShowString(32,6,"Motor Test",16);
		 if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

    {
					
					SettingFlag =1;
          //while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));

     }
			
}
void fun2_4(void)
{
			OLED_ClearSet();
			if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
			}
			OLED_ShowString(40,0,"Setting",16);
			OLED_ShowString(32,2,"ZD Set",16);
			OLED_ShowString(0,6,"->",16);
			OLED_ShowString(32,4,"Motor Test",16);
			OLED_ShowString(32,6,"EXIT",16);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
			}
}

void fun3_1(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
		OLED_ClearSet();
		OLED_ShowString(40,0,"Temp SET",16);
		//while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
		OLED_ShowString(32,2,"TempNOW:",16);
		temp_T=DS18B20_Get_Temp();
		T=temp_T%100;
		OLED_ShowNum(96,2,temp_T/10,2,16);
		OLED_ShowString(112,2,".",16);
		OLED_ShowNum(120,2,T,1,16);
		OLED_ShowString(0,4,"->",16);
		OLED_ShowString(32,4,"ExtTemp",16);
		OLED_ShowNum(96,4,ExtTemp,2,16);
		OLED_ShowString(32,6,"PEnter2EXIT",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
		}
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

    {
					
					ExtTemp+=1;
          while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

     }

     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

     {

				ExtTemp-=1;
				if(ExtTemp<1)ExtTemp=1;

         while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)); 

     }
}

void fun3_2(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}	
		OLED_ClearSet();
		OLED_ShowString(40,0,"ZD Set",16);
		//while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
		OLED_ShowString(32,2,"ZD-NOW:",16);
		OLED_ShowString(0,4,"->",16);
		HAL_ADC_Start( &hadc);  //开启ADC
		
	  if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //如果转换结果完成
	  {
		  
		printf("the adc value is %d\r\n",HAL_ADC_GetValue(&hadc));//读取打印结果并用串口打印出来结果
		OLED_ShowNum(96,2,HAL_ADC_GetValue(&hadc),4,16);
	  }
		OLED_ShowString(32,4,"ZD-SET",16);
		OLED_ShowNum(88,4,ExtZD,4,16);
		OLED_ShowString(32,6,"PEnter2EXIT",16);
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
		}
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

    {

					ExtZD+=50;
          while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

     }

     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

     {

					ExtZD-=50;
				 if(ExtZD<100)ExtZD=100;
				 
         while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)); 

     }
		
}

void fun3_3_1(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
		OLED_ClearSet();
		OLED_ShowString(40,0,"Motor SeT",16);
		OLED_ShowString(32,2,"Motor 1",16);
		OLED_ShowString(0,2,"->",16);
		OLED_ShowString(32,4,"Motor 2",16);
		OLED_ShowString(32,6,"Motor 3",16);
		if(MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(MotorFlag3==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(MotorFlag1==0) MotorFlag1=1;
			 else MotorFlag1=0;
					
				 
         while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)); 

     }
		  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

    {
					
					SettingFlag =1;
          //while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));

     }
}



void fun3_3_2(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
		OLED_ClearSet();
		OLED_ShowString(40,0,"Motor SeT",16);
		OLED_ShowString(32,2,"Motor 1",16);
		OLED_ShowString(32,4,"Motor 2",16);
		OLED_ShowString(0,4,"->",16);
		OLED_ShowString(32,6,"Motor 3",16);
			if(MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(MotorFlag3==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(MotorFlag2==0) MotorFlag2=1;
			 else MotorFlag2=0;
					
				 
         while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)); 

     }
}

void fun3_3_3(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
		OLED_ClearSet();
		OLED_ShowString(40,0,"Motor SeT",16);
		OLED_ShowString(32,2,"Motor 1",16);
		OLED_ShowString(32,4,"Motor 2",16);
		OLED_ShowString(32,6,"Motor 3",16);
		OLED_ShowString(0,6,"->",16);
			if(MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(MotorFlag3==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(MotorFlag3==0) MotorFlag3=1;
			 else MotorFlag3=0;
					
				 
         while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)); 

     }
		 if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

    {
					
					SettingFlag =1;
          //while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));

     }
}

void fun3_3_4(void)
{
		if(SettingFlag ==1){
			OLED_Clear();
			SettingFlag =0;
		}
		OLED_ClearSet();
		OLED_ShowString(40,0,"Motor SeT",16);
		OLED_ShowString(32,2,"Motor 2",16);
		OLED_ShowString(32,4,"Motor 3",16);
		OLED_ShowString(32,6,"EXIT",16);
		OLED_ShowString(0,6,"->",16);

		if(MotorFlag2==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(MotorFlag3==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
			}
		
		
}




