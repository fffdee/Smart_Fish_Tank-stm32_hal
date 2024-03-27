#include "function.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "ds18b20.h"
#include "uarst1.h"
#include "oled.h"
#include "tsw_30.h"
#include "main.h"
extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart1;
unsigned char Uart1_RxBuff[256]="ready recive";

int T,i;
unsigned char  rcData = 's';
uint16_t ADC_Value;  //ADCֵ 
unsigned char SettingFlag =1;



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
		OLED_ShowNum(24,4,state_unit.MotorFlag1,1,16);
		OLED_ShowNum(64,4,state_unit.MotorFlag2,1,16);
		OLED_ShowNum(104,4,state_unit.MotorFlag3,1,16);
	
		HAL_ADC_Start( &hadc);  //����ADC
		
	  if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //���ת��������
	  {
		  
		printf("the adc value is %d\r\n",HAL_ADC_GetValue(&hadc));//��ȡ��ӡ������ô��ڴ�ӡ�������
		OLED_ShowNum(24,2,HAL_ADC_GetValue(&hadc),4,16);
	  }
	  delay_us(50000);
		OLED_ShowNum(96,6,state_unit.ptemp,2,16);
	  OLED_ShowNum(96,2,state_unit.pzhuod,4,16);
		//HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);
		state_unit.temp_T=DS18B20_Get_state_unit.temp();
		T=state_unit.temp_T%100;
		OLED_ShowNum(24,6,state_unit.temp_T/10,2,16);
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
			OLED_ShowString(32,2,"state_unit.temp Set",16);
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
			OLED_ShowString(32,2,"state_unit.temp Set",16);
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
			OLED_ShowString(32,2,"state_unit.temp Set",16);
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
		OLED_ShowString(40,0,"state_unit.temp SET",16);
		//while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));
		OLED_ShowString(32,2,"state_unit.tempNOW:",16);
		state_unit.temp_T=DS18B20_Get_state_unit.temp();
		T=state_unit.temp_T%100;
		OLED_ShowNum(96,2,state_unit.temp_T/10,2,16);
		OLED_ShowString(112,2,".",16);
		OLED_ShowNum(120,2,T,1,16);
		OLED_ShowString(0,4,"->",16);
		OLED_ShowString(32,4,"state_unit.ptemp",16);
		OLED_ShowNum(96,4,state_unit.ptemp,2,16);
		OLED_ShowString(32,6,"PEnter2EXIT",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
		}
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

    {
					
					state_unit.ptemp+=1;
          while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

     }

     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

     {

				state_unit.ptemp-=1;
				if(state_unit.ptemp<1)state_unit.ptemp=1;

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
		HAL_ADC_Start( &hadc);  //����ADC
		
	  if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //���ת��������
	  {
		  
		printf("the adc value is %d\r\n",HAL_ADC_GetValue(&hadc));//��ȡ��ӡ������ô��ڴ�ӡ�������
		OLED_ShowNum(96,2,HAL_ADC_GetValue(&hadc),4,16);
	  }
		OLED_ShowString(32,4,"ZD-SET",16);
		OLED_ShowNum(88,4,state_unit.pzhuod,4,16);
		OLED_ShowString(32,6,"PEnter2EXIT",16);
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
		}
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

    {

					state_unit.pzhuod+=50;
          while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

     }

     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

     {

					state_unit.pzhuod-=50;
				 if(state_unit.pzhuod<100)state_unit.pzhuod=100;
				 
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
		if(state_unit.MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(state_unit.MotorFlag3==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(state_unit.MotorFlag1==0) state_unit.MotorFlag1=1;
			 else state_unit.MotorFlag1=0;
					
				 
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
			if(state_unit.MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(state_unit.MotorFlag3==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(state_unit.MotorFlag2==0) state_unit.MotorFlag2=1;
			 else state_unit.MotorFlag2=0;
					
				 
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
			if(state_unit.MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(state_unit.MotorFlag3==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(state_unit.MotorFlag3==0) state_unit.MotorFlag3=1;
			 else state_unit.MotorFlag3=0;
					
				 
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

		if(state_unit.MotorFlag2==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag3==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
			}
		
		
}




