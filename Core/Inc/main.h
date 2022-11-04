/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <string.h>
#include <stdio.h>
#include "lcd.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct
{
	uint8_t payLoad[10];
	uint8_t len;

}Command_t;

typedef enum
{
	STATE_MainMenu,
	STATE_LEDs,
	STATE_RTC,
	STATE_TimeConfig,
	STATE_DateConfig,
	STATE_RTCReport
} State_t;

typedef enum
{
	TimeCfg_HH,
	TimeCfg_MM,
	TimeCfg_SS,
	TimeCfg_AMPM
} TimeCfg_State_t;

typedef enum
{
	DateCfg_DD,
	DateCfg_MM,
	DateCfg_YY,
	DateCfg_Day
} DateCfg_State_t;

extern UART_HandleTypeDef huart3;

extern TaskHandle_t menuTask_handle;
extern TaskHandle_t printTask_handle;
extern TaskHandle_t ledTask_handle;
extern TaskHandle_t rtcTask_handle;
extern TaskHandle_t cmdTask_handle;

extern QueueHandle_t q_data;
extern QueueHandle_t print_queue;

extern State_t curr_state;

extern TimerHandle_t timer_handle[4];
extern TimerHandle_t rprting_timer;

extern RTC_HandleTypeDef hrtc;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void MenuTask_Handler(void *parameters);
void PrintTask_Handler(void *parameters);
void CMDTask_Handler(void *parameters);
void LEDTask_Handler(void *parameters);
void RTCTask_Handler(void *parameters);
void led_no_action(void);
void led_action(uint8_t id);
void led_action1(void);
void led_action2(void);
void led_action3(void);
void led_action4(void);
void turn_off_leds(void);
void turn_on_leds(void);
void turn_on_even(void);
void turn_on_odd(void);
void configure_time(RTC_TimeTypeDef *time);
void configure_date(RTC_DateTypeDef *date);
void show_time_date(void);
void LedEffectsCallback(TimerHandle_t timer_h);
void RTCReportCallback(TimerHandle_t rprt_timer);
void show_dateTime_LCD(void);
uint8_t toNumber(uint8_t *p, uint8_t len);
uint8_t validate_rtc_info(RTC_TimeTypeDef *time, RTC_DateTypeDef *date);
char* convert_to_string(uint8_t num);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LED_PORT		GPIOD
#define LED_1			GPIO_PIN_4
#define LED_2			GPIO_PIN_5
#define LED_3			GPIO_PIN_6
#define LED_4			GPIO_PIN_7

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
