#include "main.h"
#include "led.h"
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
extern unsigned char  rcData;
extern uint16_t ADC_Value;  //ADC值

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

    {2,1,3,5,fun2_2},                

    {3,2,4,6,fun2_3},
		
		{4,3,1,0,fun2_4},
		
		{5,5,5,1,fun3_1},
		
		{6,6,6,2,fun3_2}, 
		     	
		{7,7,7,3,fun3_3},


	
                                                                                 

};

int main(void)
{

  HAL_Init();


  SystemClock_Config();
	UARST1_Init();
	NVIC_Init();
	LED_Init();
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
	
	
  while (1)
  {

													if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)||(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0))

                            {

                                     delay_us(2000);

                                     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==0)

                                     {

                                     func_index=table[func_index].up; 

                                     while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4));

                                     }

                                     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==0)

                                     {

                                     func_index=table[func_index].down;

                                     while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)); 

                                     }

                                     if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==0)

                                     {

                                     func_index=table[func_index].enter; 

                                     while(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5));

                                     }


                              OLED_Init();

                           }                                    

                            current_operation_index=table[func_index].current_operation;

                            (*current_operation_index)();



//		if(LED_FLAG==1){
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//				delay_us(50000);
//				delay_us(50000);
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//				delay_us(50000);
//				delay_us(50000);
//				BEEP_FLAG=0;
//		}
//		else{
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//		}
//		if(BEEP_FLAG==1){
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
//				delay_us(50000);
//				delay_us(50000);
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//				delay_us(50000);
//				delay_us(50000);
//			LED_FLAG=0;
//		}else{
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
//		}
		
//		delay_us(50000);
//		delay_us(50000);
//		printf("ok\n");
			//HAL_ADC_Start_IT(&hadc); //开启ADC转换中
	
  }

}
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) //进入ADC中断
//{
//	if(hadc->Instance==ADC1) //判断是否为ADC1的中断
//	{
//		
//		OLED_ShowNum(8,2,HAL_ADC_GetValue(hadc),3,16);
//	}
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
		UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
		
		
		Uart1_RxBuff[Uart1_Rx_Cnt]=rcData;
		Uart1_Rx_Cnt+=1;
		
		OLED_Clear();
		OLED_ShowString(8,0,"show receive:",16);
		OLED_ShowString(8,4,"show Temp:",16);
		OLED_ShowString(64,6,"C",16);
		OLED_ShowString(48,6,".",16);
		
		HAL_UART_Transmit(&huart1, (uint8_t *)Uart1_RxBuff , sizeof(Uart1_RxBuff),0xFFFF);
		
		
 		if(rcData=='+')
		{
			
				BEEP_FLAG=1;
				
			Uart1_Rx_Cnt = 0;
			
		}
		if(rcData=='!')
		{
			LED_FLAG=1;
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			for(i=0;i<sizeof(Uart1_RxBuff);i++){
				Uart1_RxBuff[i]=0;
			}
			Uart1_Rx_Cnt = 0;
			//memset(Uart1_RxBuff, 0x00, sizeof(Uart1_RxBuff));
		
				
		}
		if(rcData=='*')
		{
				LED_FLAG=0;
				BEEP_FLAG=0;
			OLED_Clear_H(2);
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
				
				
		}
		
		

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
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






