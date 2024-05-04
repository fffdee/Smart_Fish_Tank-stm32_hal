#include "function.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "motor.h"
#include "ds18b20.h"
#include "uarst1.h"
#include "oled.h"
#include "tsw_30.h"
#include "main.h"

extern Fishtank fishtank;
extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart1;

unsigned char Uart1_RxBuff[256]="ready recive";

int T,i;
extern uint8_t  rcData[1] ;
uint16_t ADC_Value;  //ADCֵ 
unsigned char SettingFlag =1;

void State_update(State_Unit unit){
	
				if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //如果转换结果完成
				{
					unit.zhuod = HAL_ADC_GetValue(&hadc);
				}
				unit.temp = DS18B20_Get_Temp();
	
}
void Exchange_Water_Handle(){
		
	if(state_unit.zhuod>state_unit.pzhuod){
			if(state_unit.waterFlagDOWN==1)
			state_unit.MotorFlag1 = 1;
			else if((state_unit.waterFlagDOWN==0)&&(state_unit.waterFlagDOWN==0))
			state_unit.MotorFlag1 = 0;
			state_unit.MotorFlag2 = 1;

	}
//	if((state_unit.waterFlagDOWN==1)&&(state_unit.waterFlagUP==0))
//		state_unit.MotorFlag2 = 1;
//	else if((state_unit.waterFlagDOWN==1)&&(state_unit.waterFlagUP==01))
//		state_unit.MotorFlag2 = 0;

}
void logic_handle(State_Unit unit){

		MOTOR_Handle(unit);
	
		Exchange_Water_Handle();

		if(state_unit.temp<state_unit.ptemp)
		state_unit.hotFlag = 1;
		else state_unit.hotFlag = 0;
}
void updata()
{
		fishtank.temp = state_unit.temp;
		fishtank.zhuod = state_unit.zhuod/10;
		fishtank.ptemp = state_unit.ptemp;
		fishtank.pzhuod = state_unit.pzhuod/10;
	//  printf("UART DataBuff[11] = 0x%x\r\n",fishtank.zhuod);
		fishtank.waterFlagDOWN = state_unit.waterFlagDOWN;
		fishtank.waterFlagUP = state_unit.waterFlagUP;
		fishtank.motorFlag1 = state_unit.MotorFlag1;
		fishtank.motorFlag2 = state_unit.MotorFlag2;
		fishtank.hotFlag = state_unit.hotFlag;
	
		fishtank.rxFlag = 0xff;
		HAL_UART_Transmit(&huart1, (uint8_t *)&fishtank,sizeof(Fishtank),0xFFFF);
}

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
		OLED_ShowString(80,4,"H:",16);
		OLED_ShowNum(24,4,state_unit.MotorFlag1,1,16);
		OLED_ShowNum(64,4,state_unit.MotorFlag2,1,16);
		OLED_ShowNum(104,4,state_unit.hotFlag,1,16);
	
		HAL_ADC_Start( &hadc);  //����ADC
		
	  if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //���ת��������
	  {
		state_unit.zhuod = HAL_ADC_GetValue(&hadc);
	//	printf("the adc value is %d\r\n",state_unit.zhuod);//��ȡ��ӡ������ô��ڴ�ӡ�������
		OLED_ShowNum(24,2,state_unit.zhuod,4,16);
	  }
	  
		for(int i=0; i<10;i++)
		delay_us(50000);
		

		OLED_ShowNum(96,6,state_unit.ptemp,2,16);
	  OLED_ShowNum(96,2,state_unit.pzhuod,4,16);
		//HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);
		state_unit.temp=DS18B20_Get_Temp();
		T=state_unit.temp%100;
		OLED_ShowNum(24,6,state_unit.temp/10,2,16);
		OLED_ShowNum(46,6,T,1,16);
		state_unit.temp = state_unit.temp/10;
	//	OLED_ShowString(8,2,Uart1_RxBuff,16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
		}
		
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);
		State_update(state_unit);
		logic_handle(state_unit);
	  updata();
		
		
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
			OLED_ShowString(32,6,"ControlTest",16);
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
			OLED_ShowString(32,6,"ControlTest",16);
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
			OLED_ShowString(32,6,"ControlTest",16);
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
			OLED_ShowString(32,4,"ControlTest",16);
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
		
		T=state_unit.temp%100;
		OLED_ShowNum(96,2,state_unit.temp/10,2,16);
		OLED_ShowString(112,2,".",16);
		OLED_ShowNum(120,2,T,1,16);
		OLED_ShowString(0,4,"->",16);
		OLED_ShowString(32,4,"Ptemp",16);
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
		  
		//printf("the adc value is %d\r\n",state_unit.zhuod);//��ȡ��ӡ������ô��ڴ�ӡ�������
		OLED_ShowNum(96,2,state_unit.zhuod,4,16);
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
		OLED_ShowString(32,6,"Hot Ctrl",16);
		if(state_unit.MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(state_unit.hotFlag==0)OLED_ShowString(96,6,"OFF",16);
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
		OLED_ShowString(32,6,"Hot Ctrl",16);
			if(state_unit.MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(state_unit.hotFlag==0)OLED_ShowString(96,6,"OFF",16);
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
		OLED_ShowString(32,6,"Hot Ctrl",16);
		OLED_ShowString(0,6,"->",16);
			if(state_unit.MotorFlag1==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.MotorFlag2==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(state_unit.hotFlag==0)OLED_ShowString(96,6,"OFF",16);
		else OLED_ShowString(96,6,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

     {
			 if(state_unit.hotFlag==0) state_unit.hotFlag=1;
			 else state_unit.hotFlag=0;
					
				 
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
		OLED_ShowString(32,4,"Hot Ctrl",16);
		OLED_ShowString(32,6,"EXIT",16);
		OLED_ShowString(0,6,"->",16);

		if(state_unit.MotorFlag2==0)OLED_ShowString(96,2,"OFF",16);
		else OLED_ShowString(96,2,"ON ",16);
		if(state_unit.hotFlag==0)OLED_ShowString(96,4,"OFF",16);
		else OLED_ShowString(96,4,"ON ",16);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0){
				SettingFlag =1;
			}
		
		
}




