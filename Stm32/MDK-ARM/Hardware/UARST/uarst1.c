#include "uarst1.h"

UART_HandleTypeDef huart1;
uint8_t aRxBuffer;			//接收中断缓冲

		
uint8_t	cAlmStr[] = "数据溢出(大于256)\r\n";
struct __FILE
{
        int handle;
};
FILE __stdout;
int fputc(int ch, FILE *f)
{
 HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);//更具实际情况更改驱动
         return (ch);
}

 
void UARST1_Init(void)
{
	  /* USER CODE BEGIN USART1_Init 0 */
	  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
	 __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = 3;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */





