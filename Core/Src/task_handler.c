/*
 * task_handler.c
 *
 *  Created on: Oct 30, 2022
 *      Author: omark
 */
#include "main.h"

static int ExtractCMD(Command_t *pCMD);
static void ProcessCMD(Command_t *pCMD);

const char *inv_msg = "Invalid Option!\n";
TimeCfg_State_t rtc_timeCfg_state = TimeCfg_HH;
DateCfg_State_t rtc_dateCfg_state = DateCfg_DD;

void MenuTask_Handler(void *parameters)
{
	Command_t *cmd;
	uint32_t cmd_addr;
	uint8_t option;

	const char* msg_menu = "----------------------\n"
						   "         Menu         \n"
						   "----------------------\n"
						   "  LED Application->  0\n"
						   "  Date and Time  ->  1\n"
						   "          Exit   ->  2\n"
						   "  Enter your choice: ";



	while (1)
	{
		xQueueSend(print_queue, &msg_menu, portMAX_DELAY);

		// block until user enters an option, when notification "which is the command address"
		// is received, it will unblock
		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
		cmd = (Command_t*)cmd_addr;

		if (cmd->len == 1)
		{
			option = cmd->payLoad[0] - 48;
			switch(option)
			{
			case 0:
			{
				// notify the LED application task
				curr_state = STATE_LEDs;
				xTaskNotify(ledTask_handle, 0, eNoAction);
				break;
			}
			case 1:
			{
				// notify the RTC application task
				curr_state = STATE_RTC;
				xTaskNotify(rtcTask_handle, 0, eNoAction);
				break;
			}
			case 2:
			{
				// exit
				break;
			}
			default:
			{
				xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
				break;
			}
			}
		}
		else
		{
			xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
			continue;
		}
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
	}
}

void PrintTask_Handler(void *parameters)
{
	uint32_t *msg;
	while (1)
	{
		xQueueReceive(print_queue, &msg, portMAX_DELAY);
		HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);
	}
}

void CMDTask_Handler(void *parameters)
{
	Command_t cmd;
	while (1)
	{
		if (xTaskNotifyWait(0, 0, NULL, portMAX_DELAY))
		{
			ProcessCMD(&cmd);
		}
	}
}

void LEDTask_Handler(void *parameters)
{
	uint32_t cmd_addr;
	Command_t *cmd;

	const char *led_msg = "----------------------\n"
						  "    LED Application   \n"
						  "----------------------\n"
						  "(none, e1, e2, e3, e4)\n"
						  " Enter your choice:  ";

	while (1)
	{
		// wait for a user input indicates that led application has been chosen
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		// print the application welcome message
		xQueueSend(print_queue, &led_msg, portMAX_DELAY);

		// block until command is received (notification)
		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
		cmd = (Command_t*)cmd_addr;

		if (cmd->len <= 4)
		{
			if (!strcmp((char*)cmd->payLoad, "none"))
			{
				led_no_action();
			}
			else if(!strcmp((char*)cmd->payLoad, "e1"))
			{
				led_action(1);
			}
			else if (!strcmp((char*)cmd->payLoad, "e2"))
			{
				led_action(2);
			}
			else if (!strcmp((char*)cmd->payLoad, "e3"))
			{
				led_action(3);
			}
			else if (!strcmp((char*)cmd->payLoad, "e4"))
			{
				led_action(4);
			}
			else
			{
				xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
			}
		}
		else
		{
			xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
		}

		curr_state = STATE_MainMenu;

		xTaskNotify(menuTask_handle, 0, eNoAction);
	}
}

void RTCTask_Handler(void *parameters)
{
	uint32_t cmd_addr;
	Command_t *cmd;
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	const char *rtc_welcome_msg = "----------------------\n"
							  "     RTC Application  \n"
							  "----------------------\n"
							  "Configure time   ->  0\n"
							  "Configure date   ->  1\n"
							  "RTC Report       ->  2\n"
							  "Exit             ->  3\n"
							  "Enter your choice: ";

	const char *time_msg[4] = {"Enter hour(1-12): ",
							   "Enter minutes(0-59): ",
							   "Enter seconds(0-59): ",
								"AM/PM?(am/pm): "};

	const char *date_msg[4] = { "Enter date(1-31): ",
							    "Enter month(1-12): ",
								"Enter year(0-99): ",
								"Enter day(1-7(Mon:1)): "
								};

	const char *cfg_msg[2] = {"Configuration successful\n",
							  "Enable time and date report?(y/n)"};

	while (1)
	{
		// block this task until it's notified from the menu task
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		// here, a notification is received
		show_time_date();
		xQueueSend(print_queue, &rtc_welcome_msg, portMAX_DELAY);

		while (curr_state != STATE_MainMenu)
		{
			// block until a choice is sent
			xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
			cmd = (Command_t*)cmd_addr;

			switch(curr_state)
			{
			case STATE_RTC:
			{
				if (cmd->len == 1)
				{
					uint8_t option = cmd->payLoad[0] - 48;
					switch(option)
					{
					case 0:
					{
						curr_state = STATE_TimeConfig;
						xQueueSend(print_queue, &time_msg[0], portMAX_DELAY);
						break;
					}
					case 1:
					{
						curr_state = STATE_DateConfig;
						xQueueSend(print_queue, &date_msg[0], portMAX_DELAY);
						break;
					}
					case 2:
					{
						curr_state = STATE_RTCReport;
						xQueueSend(print_queue, &cfg_msg[1], portMAX_DELAY);
						break;
					}
					case 3:
					{
						curr_state = STATE_MainMenu;
						break;
					}
					default:
					{
						curr_state = STATE_MainMenu;
						xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
					}
					}
				}
				else
				{
					curr_state = STATE_MainMenu;
					xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
				}
				break;
			}

			case STATE_TimeConfig:
			{
				switch(rtc_timeCfg_state)
				{
				case TimeCfg_HH:
				{
					uint8_t hour = toNumber(cmd->payLoad, cmd->len);
					time.Hours = hour;
					rtc_timeCfg_state = TimeCfg_MM;
					xQueueSend(print_queue, &time_msg[1], portMAX_DELAY);
					break;
				}
				case TimeCfg_MM:
				{
					uint8_t min = toNumber(cmd->payLoad, cmd->len);
					time.Minutes = min;
					rtc_timeCfg_state = TimeCfg_SS;
					xQueueSend(print_queue, &time_msg[2], portMAX_DELAY);
					break;
				}
				case TimeCfg_SS:
				{
					uint8_t sec = toNumber(cmd->payLoad, cmd->len);
					time.Seconds = sec;
					rtc_timeCfg_state = TimeCfg_AMPM;
					xQueueSend(print_queue, &time_msg[3], portMAX_DELAY);
					break;
				}
				case TimeCfg_AMPM:
				{
					uint8_t am_pm;
					if (! strcmp((char*)cmd->payLoad, "am"))
					{
						am_pm = 0;
					}
					else if (! strcmp((char*)cmd->payLoad, "pm"))
					{
						am_pm = 1;
					}
					else
					{
						xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
						continue;
					}
					time.TimeFormat = (am_pm == 0)? RTC_HOURFORMAT12_AM : RTC_HOURFORMAT12_PM;
					if (! validate_rtc_info(&time, NULL))
					{
						// time user entered is valid
						configure_time(&time);
						xQueueSend(print_queue, &cfg_msg[0], portMAX_DELAY);
						show_time_date();
					}
					else
					{
						xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
					}
					rtc_timeCfg_state = TimeCfg_HH;
					curr_state = STATE_MainMenu;
					break;
				}
				}
				break;
			}
			case STATE_DateConfig:
			{
				switch(rtc_dateCfg_state)
				{
				case DateCfg_DD:
				{
					uint8_t dayNo = toNumber(cmd->payLoad, cmd->len);
					date.Date = dayNo;
					rtc_dateCfg_state = DateCfg_MM;
					xQueueSend(print_queue, &date_msg[1], portMAX_DELAY);
					break;
				}
				case DateCfg_MM:
				{
					uint8_t mon = toNumber(cmd->payLoad, cmd->len);
					date.Month = mon;
					rtc_dateCfg_state = DateCfg_YY;
					xQueueSend(print_queue, &date_msg[2], portMAX_DELAY);
					break;
				}
				case DateCfg_YY:
				{
					uint8_t yyyy = toNumber(cmd->payLoad, cmd->len);
					date.Year = yyyy;
					rtc_dateCfg_state = DateCfg_Day;
					xQueueSend(print_queue, &date_msg[3], portMAX_DELAY);
					break;
				}
				case DateCfg_Day:
				{
					uint8_t day = toNumber(cmd->payLoad, cmd->len);
					date.WeekDay = day;
					if (! validate_rtc_info(NULL, &date))
					{
						// valid dates
						configure_date(&date);
						xQueueSend(print_queue, &cfg_msg[0], portMAX_DELAY);
						show_time_date();
					}
					else
					{
						// invalid dates
						xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
					}
					rtc_dateCfg_state = DateCfg_DD;
					curr_state = STATE_MainMenu;
					break;
				}
				}
				break;
			}
			case STATE_RTCReport:
			{
				if (cmd->len == 1)
				{
					if (cmd->payLoad[0] == 'y')
					{
						xTimerStart(rprting_timer, portMAX_DELAY);
					}
					else if (cmd->payLoad[0] == 'n')
					{
						xTimerStop(rprting_timer, portMAX_DELAY);
					}
					else
					{
						xQueueSend(print_queue, &inv_msg, portMAX_DELAY);
					}
				}
				else
				{

				}
				break;
			}
			}

		}
	}
}

static void ProcessCMD(Command_t *pCMD)
{
	ExtractCMD(pCMD);

	switch(curr_state)
	{
	case STATE_MainMenu:
	{
		xTaskNotify(menuTask_handle, (uint32_t)pCMD, eSetValueWithOverwrite);
		break;
	}

	case STATE_LEDs:
	{
		xTaskNotify(ledTask_handle, (uint32_t)pCMD, eSetValueWithOverwrite);
		break;
	}

	case STATE_RTC:
	{
		xTaskNotify(rtcTask_handle, (uint32_t)pCMD, eSetValueWithOverwrite);
		break;
	}

	case STATE_TimeConfig:
	{
		xTaskNotify(rtcTask_handle, (uint32_t)pCMD, eSetValueWithOverwrite);
		break;
	}

	case STATE_DateConfig:
	{
		xTaskNotify(rtcTask_handle, (uint32_t)pCMD, eSetValueWithOverwrite);
		break;
	}

	case STATE_RTCReport:
	{
		xTaskNotify(rtcTask_handle, (uint32_t)pCMD, eSetValueWithOverwrite);
		break;
	}
	}
}

static int ExtractCMD(Command_t *pCMD)
{
	if (! uxQueueMessagesWaiting(q_data))
		return -1;

	uint8_t element;
	BaseType_t status;
	uint8_t i = 0;

	do
	{
		status = xQueueReceive(q_data, &element, 0);
		if (status == pdTRUE)
			pCMD->payLoad[i++] = element;
	}while (element != '\n');

	pCMD->payLoad[i-1] = '\0';
	pCMD->len = i - 1;

	return 0;
}
