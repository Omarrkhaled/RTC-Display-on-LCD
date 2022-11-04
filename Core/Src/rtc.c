/*
 * rtc.c
 *
 *  Created on: Oct 30, 2022
 *      Author: omark
 */


#include "main.h"

void configure_time(RTC_TimeTypeDef *time)
{
	time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	time->StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);
}

void configure_date(RTC_DateTypeDef *date)
{
	HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);
}

void show_time_date(void)
{
	static char time_holder[50];
	static char date_holder[50];

	static char *time = time_holder;
	static char *date = date_holder;

	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM)? "AM": "PM";

	sprintf((char*)time_holder, "%s:\t%02d:%02d:%02d\t[%s]\n", "Current time", rtc_time.Hours,
				rtc_time.Minutes, rtc_time.Seconds, format);
	xQueueSend(print_queue, &time, portMAX_DELAY);

	sprintf((char*)date_holder, "%s:\t%02d-%02d-%2d [%s]\n","Current date", rtc_date.Date, rtc_date.Month,
					rtc_date.Year + 2000, convert_to_string(rtc_date.WeekDay));
	xQueueSend(print_queue, &date, portMAX_DELAY);
}

void show_dateTime_LCD(void)
{
	static char time_holder[50];
	static char date_holder[50];

	static char *time = time_holder;
	static char *date = date_holder;

	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM)? "AM": "PM";

	sprintf((char*)time_holder, "%02d:%02d:%02d [%s]", rtc_time.Hours,
				rtc_time.Minutes, rtc_time.Seconds, format);

	lcd_set_cursor(1, 1);
	lcd_print_string(time);

	sprintf((char*)date_holder, "%02d-%02d-%2d", rtc_date.Date, rtc_date.Month,
					rtc_date.Year + 2000);

	lcd_set_cursor(2, 1);
	lcd_print_string(date);
}

uint8_t toNumber(uint8_t *p, uint8_t len)
{
	if (len > 1)
	{
		return (( (*p) - 48 ) * 10 + ( (*p) - 48 ));
	}
	else
	{
		return ( (*p) - 48 );
	}
}

uint8_t validate_rtc_info(RTC_TimeTypeDef *time, RTC_DateTypeDef *date)
{
	if (time)
	{
		if (time->Hours > 12 || time->Minutes > 59 || time->Seconds > 59)
			return 1;
	}
	if (date)
	{
		if (date->Date > 31 || date->Month > 12 || date->Year > 2022)
			return 1;
	}
	return 0;
}

char* convert_to_string(uint8_t num)
{
	static char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	return days[num-1];
}
