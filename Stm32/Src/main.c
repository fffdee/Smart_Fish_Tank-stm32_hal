#include "main.h"
#include "led.h"
#include "motor.h"
#include "beep.h"
#include "key.h"
#include "ds18b20.h"
#include "uarst1.h"
#include "oled.h"
#include "tsw_30.h"
#include "stm32f0xx.h"
#include <cstring>
#include "function.h"
extern ADC_HandleTypeDef hadc;
static void SystemClock_Config(void);
extern UART_HandleTypeDef huart1;
unsigned char check_ds18b20; 

extern unsigned char Uart1_RxBuff[256];
extern short temp_T;
extern int T,i;
uint8_t  rcData[1];
extern uint16_t ADC_Value;  //ADC值

extern unsigned char SettingFlag ;
extern unsigned char MotorFlag1 ;
extern unsigned char MotorFlag2 ;
extern unsigned char MotorFlag3 ;
extern unsigned char ExtTemp ;
extern unsigned int ExtZD; 

Fishtank fishtank;
State_Unit state_unit;
uint32_t int32_data[4] = {0};
void Stata_Unit_Init(void)
{
		state_unit.wifi_ip = NULL;
		state_unit.hotFlag = 0;
		state_unit.MotorFlag1 = 0;
		state_unit.MotorFlag2 = 0;
		state_unit.ptemp = 25;
		state_unit.waterFlagUP = 1;
		state_unit.waterFlagDOWN = 1;
		state_unit.temp = DS18B20_Get_Temp();
		state_unit.pzhuod = 3600;
	  state_unit.autoMode = 1;
//		if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //如果转换结果完成
//	  {
//			state_unit.zhuod = HAL_ADC_GetValue(&hadc);
//	  }
		
	
}
uint8_t LED_FLAG = 0;
uint8_t BEEP_FLAG = 0;

uint8_t Uart1_Rx_Cnt = 0; //接收缓冲计数

#define uchar unsigned char	  


uchar func_index=0;	 

void (*current_operation_index)(); 

typedef struct
{

         uchar current;

         uchar up;

         uchar down;

         uchar enter;

         void (*current_operation)();	 

} key_table;

key_table table[30]=

{



    {0,0,0,1,fun1},

    {1,4,2,5,fun2_1},	

    {2,1,3,6,fun2_2},                

    {3,2,4,7,fun2_3},
		
		{4,3,1,0,fun2_4},
		
		{5,5,5,1,fun3_1},
		
		{6,6,6,2,fun3_2}, 
		     	
		{7,10,8,7,fun3_3_1},

		{8,7,9,8,fun3_3_2},
		
		{9,8,10,9,fun3_3_3},
		
		{10,9,7,1,fun3_3_4},

	
                                                                                 

};

void State_update(State_Unit unit){
	
				if(HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK )  //如果转换结果完成
				{
					unit.zhuod = HAL_ADC_GetValue(&hadc);
				}
				unit.temp = DS18B20_Get_Temp();
	
}
void Exchange_Water_Handle(State_Unit unit){
		
	if(unit.zhuod<unit.pzhuod){
			if(unit.waterFlagDOWN==1)
			unit.MotorFlag1 = 1;
			else if((unit.waterFlagDOWN==0)&&(unit.waterFlagDOWN==0))
			unit.MotorFlag1 = 0;
			unit.MotorFlag2 = 1;

	}
	if((unit.waterFlagDOWN==1)&&(unit.waterFlagUP==0))
		unit.MotorFlag2 = 1;
	else if((unit.waterFlagDOWN==1)&&(unit.waterFlagUP==01))
		unit.MotorFlag2 = 0;

}
void logic_handle(State_Unit unit){

		MOTOR_Handle(unit);
		Exchange_Water_Handle(unit);
		
}

int main(void)
{

  HAL_Init();

  SystemClock_Config();
	UARST1_Init();
	NVIC_Init();
	LED_Init();
	MOTOR_Init();
	BEEP_Init();
	KEY_Init();
	DS18B20_Init();
	DS18B20_Rst();
	
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
	delay_us(50000);
	delay_us(50000);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
	delay_us(50000);
	delay_us(50000);
	OLED_GPIO_Init();
	OLED_Init();
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	TSW_Init();
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData , 1);   //再开启接收中断
	OLED_Clear();
	Stata_Unit_Init();
  while (1)
  {

													if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0))

                            {

                                     delay_us(2000);

                                     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

                                     {

                                     func_index=table[func_index].up; 

                                     while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

                                     }

                                     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

                                     {

                                     func_index=table[func_index].down;

                                     while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)); 

                                     }

                                     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

                                     {

																				 func_index=table[func_index].enter; 

																				 while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6));

                                     }


                              

                           }                                    

                            current_operation_index=table[func_index].current_operation;

                            (*current_operation_index)();
 
														State_update(state_unit);
													  logic_handle(state_unit);


		


			//HAL_ADC_Start_IT(&hadc); //开启ADC转换中
	
  }

}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) //进入ADC中断
{
	if(hadc->Instance==ADC1) //判断是否为ADC1的中断
	{
		
		OLED_ShowNum(8,2,HAL_ADC_GetValue(hadc),3,16);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
		UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
		
			Uart1_Rx_Cnt+=1;
			Uart1_RxBuff[Uart1_Rx_Cnt-1]=rcData[0];
			
			if(rcData[0] == 0x7F){
			 for(int i=0;i<Uart1_Rx_Cnt;i++)
				printf("UART DataBuff[%d] = 0x%x\r\n",i,Uart1_RxBuff[i]);	
			memcpy((uint8_t *)&fishtank, &Uart1_RxBuff,Uart1_Rx_Cnt ); 	
			state_unit.ptemp = fishtank.ptemp;
			//printf("UART DataBuff[11] = 0x%x\r\n",fishtank.pzhuod);
			int32_data[0] = Uart1_RxBuff[12];
			int32_data[1] = Uart1_RxBuff[13];
			int32_data[2] = Uart1_RxBuff[14];
			int32_data[3] = Uart1_RxBuff[15];
			//state_unit.pzhuod = int32_data[0]|int32_data[1]<<8|int32_data[2]<<16|int32_data[3]<<24;
				state_unit.pzhuod = fishtank.pzhuod;
			state_unit.MotorFlag1 = fishtank.motorFlag1;
			state_unit.MotorFlag2 = fishtank.motorFlag2;
			state_unit.hotFlag = fishtank.hotFlag;
			memset(Uart1_RxBuff,0,sizeof(Uart1_RxBuff));
			Uart1_Rx_Cnt =0;	
			}
		
	//	memcpy((uint8_t *)&fishtank,(uint8_t *)&Uart1_RxBuff,Uart1_Rx_Cnt ); 

				
		 
//		state_unit.ptemp = fishtank.ptemp;
//			
//    state_unit.pzhuod = fishtank.pzhuod;
//    state_unit.MotorFlag1 = Uart1_RxBuff[0];
//    state_unit.MotorFlag2 = Uart1_RxBuff[1];
//    state_unit.hotFlag = Uart1_RxBuff[2];
//			
//		state_unit.ptemp = fishtank.ptemp;
//			
//    state_unit.pzhuod = fishtank.pzhuod;
//    state_unit.MotorFlag1 = fishtank.motorFlag1;
//    state_unit.MotorFlag2 = fishtank.motorFlag2;
//    state_unit.hotFlag = fishtank.hotFlag;
//	
		
   		//HAL_UART_Transmit(&huart1, rcData,1,0xFFFF);
//		printf("收到:%d",Uart1_Rx_Cnt);
//		printf("or1:%d\n\t",Uart1_RxBuff[0]);
//		printf("or2:%d\n\t",Uart1_RxBuff[1]);
//		printf("or3:%d\n\t",Uart1_RxBuff[2]);
//		printf("or4:%d\n\t",Uart1_RxBuff[3]);
//		printf("or5:%d\n\t",Uart1_RxBuff[4]);
//		printf("or6:%d\n\t",Uart1_RxBuff[5]);
//		printf("or7:%d\n\t",Uart1_RxBuff[6]);
//		printf("or8:%d\n\t",Uart1_RxBuff[7]);
//		printf("or9:%d\n\t",Uart1_RxBuff[8]);
//		printf("or10:%d\n\t",Uart1_RxBuff[9]);
//		printf("or11:%d\n\t",Uart1_RxBuff[10]);
//		printf("or12:%d\n\t",Uart1_RxBuff[11]);
	//	memset(Uart1_RxBuff, 0x00, sizeof(Uart1_RxBuff));
// 		if(rcData=='+')
//		{
//			state_unit.MotorFlag1 = 1;
//			
//						Uart1_Rx_Cnt =0;
//		}
//			if(rcData=='*')
//		{
//			state_unit.MotorFlag1 = 0;
//							Uart1_Rx_Cnt =0;
//		}
//		
//		if(rcData=='(')
//		{
//			state_unit.MotorFlag2 = 1;
//							Uart1_Rx_Cnt =0;
//		}
//		if(rcData==')')
//		{
//			state_unit.MotorFlag2 = 0;
//							Uart1_Rx_Cnt =0;
//		}
//		
//	
	

	HAL_UART_Receive_IT(&huart1, (uint8_t *)&rcData, 1);   //再开启接收中断
}


void SystemClock_Config(void)
{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

//  /** Initializes the RCC Oscillators according to the specified parameters
//  * in the RCC_OscInitTypeDef structure.
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
//  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Initializes the CPU, AHB and APB buses clocks
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
//  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}



#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */






