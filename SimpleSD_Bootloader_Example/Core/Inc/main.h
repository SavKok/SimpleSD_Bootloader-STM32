/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_RESET_Pin GPIO_PIN_3
#define LCD_RESET_GPIO_Port GPIOE
#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOE
#define SPI5_SCK_Pin GPIO_PIN_7
#define SPI5_SCK_GPIO_Port GPIOF
#define SPI5_MOSI_Pin GPIO_PIN_9
#define SPI5_MOSI_GPIO_Port GPIOF
#define LCD_ENABLE_Pin GPIO_PIN_10
#define LCD_ENABLE_GPIO_Port GPIOF
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define NCS_MEMS_SPI_Pin GPIO_PIN_1
#define NCS_MEMS_SPI_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_2
#define LCD_CS_GPIO_Port GPIOC
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define MEMS_INT1_Pin GPIO_PIN_1
#define MEMS_INT1_GPIO_Port GPIOA
#define MEMS_INT2_Pin GPIO_PIN_2
#define MEMS_INT2_GPIO_Port GPIOA
#define RTC_1HZ_Pin GPIO_PIN_7
#define RTC_1HZ_GPIO_Port GPIOA
#define DISABLE_DC_Pin GPIO_PIN_4
#define DISABLE_DC_GPIO_Port GPIOC
#define R3_Pin GPIO_PIN_0
#define R3_GPIO_Port GPIOB
#define R6_Pin GPIO_PIN_1
#define R6_GPIO_Port GPIOB
#define WDT_IN_Pin GPIO_PIN_12
#define WDT_IN_GPIO_Port GPIOB
#define LCD_RDX_Pin GPIO_PIN_12
#define LCD_RDX_GPIO_Port GPIOD
#define LCD_DC_Pin GPIO_PIN_13
#define LCD_DC_GPIO_Port GPIOD
#define R7_Pin GPIO_PIN_6
#define R7_GPIO_Port GPIOG
#define G6_Pin GPIO_PIN_7
#define G6_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_8
#define SD_CD_GPIO_Port GPIOC
#define R4_Pin GPIO_PIN_11
#define R4_GPIO_Port GPIOA
#define R5_Pin GPIO_PIN_12
#define R5_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define R2_Pin GPIO_PIN_10
#define R2_GPIO_Port GPIOC
#define G7_Pin GPIO_PIN_3
#define G7_GPIO_Port GPIOD
#define IM1_Pin GPIO_PIN_4
#define IM1_GPIO_Port GPIOD
#define IM2_Pin GPIO_PIN_5
#define IM2_GPIO_Port GPIOD
#define IM0_Pin GPIO_PIN_7
#define IM0_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOG
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
