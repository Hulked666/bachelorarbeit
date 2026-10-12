#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifndef INC_RTC_H_
#define INC_RTC_H_


#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


typedef struct{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;

	uint8_t day;
	uint8_t month;
	uint16_t year;
}RTC_DateTime_t;

HAL_StatusTypeDef RTC_GetDateTime(RTC_DateTime_t *date_time);

#endif /* INC_RTC_H_ */
