#include "tsw_30.h"
#include "stm32f0xx.h"
#include "core_cm0.h"            /* Cortex-M0 processor and core peripherals */
#include "system_stm32f0xx.h"    /* STM32F0xx System Header */
#include <stdint.h>

ADC_HandleTypeDef hadc;
void TSW_Init(void)
{

//  ADC_ChannelConfTypeDef sConfig = {0};
//  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};

//  /* USER CODE BEGIN ADC_Init 1 */

//  /* USER CODE END ADC_Init 1 */
//  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
//  */
//  hadc.Instance = ADC1;
//  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
//  hadc.Init.Resolution = ADC_RESOLUTION_12B;
//  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
//  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  hadc.Init.LowPowerAutoWait = DISABLE;
//  hadc.Init.LowPowerAutoPowerOff = DISABLE;
//  hadc.Init.ContinuousConvMode = ENABLE;
//  hadc.Init.DiscontinuousConvMode = DISABLE;
//  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc.Init.DMAContinuousRequests = DISABLE;
//  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//  if (HAL_ADC_Init(&hadc) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure for the selected ADC regular channel to be converted.
//  */
//  sConfig.Channel = ADC_CHANNEL_0;
//  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
//  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure the analog watchdog
//  */
//  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
//  AnalogWDGConfig.Channel = ADC_CHANNEL_0;
//  AnalogWDGConfig.ITMode = DISABLE;
//  AnalogWDGConfig.HighThreshold = 0;
//  AnalogWDGConfig.LowThreshold = 0;
//  if (HAL_ADC_AnalogWDGConfig(&hadc, &AnalogWDGConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
  ADC_ChannelConfTypeDef sConfig = {0};
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the analog watchdog
  */
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
  AnalogWDGConfig.Channel = ADC_CHANNEL_0;
  AnalogWDGConfig.ITMode = DISABLE;
  AnalogWDGConfig.HighThreshold = 0;
  AnalogWDGConfig.LowThreshold = 0;
  if (HAL_ADC_AnalogWDGConfig(&hadc, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */


}






