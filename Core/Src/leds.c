/*
 * leds.c
 *
 *  Created on: Oct 30, 2022
 *      Author: omark
 */


#include "main.h"


void turn_off_leds(void)
{
	HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_RESET);
}


void turn_on_leds(void)
{
	HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_SET);
}

void turn_on_even(void)
{
	HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_SET);
}

void turn_on_odd(void)
{
	HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_RESET);
}

void led_no_action(void)
{
	turn_off_leds();
	for (uint8_t i = 0; i < 4; i++)
	{
		xTimerStop(timer_handle[i], portMAX_DELAY);
	}
}

void led_action(uint8_t id)
{
	led_no_action();
	xTimerStart(timer_handle[id-1], portMAX_DELAY);
}

void led_action1(void)
{
	// turn off all LEDs, then turn them all on every 500 ms
	static uint8_t flag = 1;
	(flag ^= 1) ? turn_off_leds() : turn_on_leds();
}

void led_action2(void)
{
	// turn on/off even LEDs
	static uint8_t flag = 1;
	(flag ^= 1)? turn_on_even() : turn_on_odd();
}

void led_action3(void)
{
	// progress through LEDs
	static int i = 0;

	switch(i)
	{
	case 0:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_SET);
		i++;
		break;
	}
	case 1:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_RESET);
		i++;
		break;
	}

	case 2:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_RESET);
		i++;
		break;
	}

	case 3:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_RESET);
		i = 0;
		break;
	}
	}

}

void led_action4(void)
{
	// progress through LEDs in-reverse

	static int j = 0;

	switch(j)
	{
	case 0:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_SET);
		j++;
		break;
	}
	case 1:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, LED_4, GPIO_PIN_RESET);
		j++;
		break;
	}

	case 2:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, LED_3, GPIO_PIN_RESET);
		j++;
		break;
	}

	case 3:
	{
		HAL_GPIO_WritePin(LED_PORT, LED_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_PORT, LED_2, GPIO_PIN_RESET);
		j = 0;
		break;
	}
	}
}
