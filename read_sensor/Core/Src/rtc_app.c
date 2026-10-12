#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rtc_app.h>

extern RTC_HandleTypeDef hrtc;


HAL_StatusTypeDef RTC_GetDateTime(RTC_DateTime_t *data){

	HAL_StatusTypeDef status;
	if(data == NULL){
		return HAL_ERROR;
	}

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	status = HAL_RTC_GetTime(&hrtc, &time,RTC_FORMAT_BIN);
	if(status != HAL_OK){
		return status;
	}
	status = HAL_RTC_GetDate(&hrtc, &date,RTC_FORMAT_BIN);
	if(status != HAL_OK){
		return status;
	}
	data->hours = time.Hours;
	data->minutes = time.Minutes;
	data->seconds = time.Seconds;

	data->day = date.Date;
	data->month = date.Month;
	data->year = 2000 + date.Year;

	return HAL_OK;

}
