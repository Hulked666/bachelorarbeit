#
#ifndef INC_STATS_H_
#define INC_STATS_H_


#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct{
	float min;
	float max;
	float sum;
	uint32_t count;
}StatsStruct_t;

typedef struct{
    uint8_t day;
    uint8_t month;
    uint16_t year;

	float temp_min;
	float temp_max;
	float temp_mean;

	float hum_min;
	float hum_max;
	float hum_mean;

	float press_min;
	float press_max;
	float press_mean;
}DailyStats_t;


HAL_StatusTypeDef STATS_UpdateAll(float temperature, float pressure, float humidity);
void STATS_Reset(void);
HAL_StatusTypeDef CREATE_Daily(DailyStats_t *daily);
HAL_StatusTypeDef ADD_Daily(DailyStats_t *stats);
HAL_StatusTypeDef STATS_UpdateAll(float temperature, float pressure, float humidity);
void STATS_Reset(void);
HAL_StatusTypeDef STATS_GetHistory(DailyStats_t *buffer, uint8_t *count);

#endif /* INC_STATS_H_ */
