#include "stats.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>


static StatsStruct_t temp_stats;
static StatsStruct_t hum_stats;
static StatsStruct_t press_stats;

static DailyStats_t history[7];

static uint8_t history_count = 0;

HAL_StatusTypeDef STATS_Update(StatsStruct_t *stats, float value){

	if(stats == 0){
		return HAL_ERROR;
	}else if(stats->count == 0){
		stats->min = value;
		stats->max = value;
	}else{
		if(value < stats->min){
			stats->min = value;
		}

		if(value > stats->max){
			stats->max = value;
		}
	}
	stats->sum += value;
	stats->count++;

	return HAL_OK;
}

float STATS_Mean(StatsStruct_t *stats){
	float mean = 0;
	if(stats == NULL){
		return 0.0f;
	}
	if(stats->count != 0){
		mean = stats->sum / stats->count;
	}

	return mean;
}


HAL_StatusTypeDef CREATE_Daily(DailyStats_t *daily){
	if(daily == NULL){
		return HAL_ERROR;
	}

	daily->temp_max = temp_stats.max;
	daily->temp_min = temp_stats.min;
	daily->temp_mean = STATS_Mean(&temp_stats);

	daily->hum_max = hum_stats.max;
	daily->hum_min = hum_stats.min;
	daily->hum_mean = STATS_Mean(&hum_stats);

	daily->press_max = press_stats.max;
	daily->press_min = press_stats.min;
	daily->press_mean = STATS_Mean(&press_stats);

	return HAL_OK;

}

HAL_StatusTypeDef ADD_Daily(DailyStats_t *stats){
	if(stats == NULL){
		return HAL_ERROR;
	}
	if(history_count < 7){
		history[history_count] = *stats;
		history_count++;
	}else{
		history[0] = history[1];
		history[1] = history[2];
		history[2] = history[3];
		history[3] = history[4];
		history[4] = history[5];
		history[5] = history[6];
		history[6] = *stats;
	}
	return HAL_OK;
}

HAL_StatusTypeDef STATS_UpdateAll(float temperature, float pressure, float humidity){
	HAL_StatusTypeDef status;
	status = STATS_Update(&temp_stats,temperature);
	if(status != HAL_OK){
		return status;
	}
	status = STATS_Update(&press_stats,pressure);
	if(status != HAL_OK){
		return status;
	}
	status = STATS_Update(&hum_stats,humidity);
	if(status != HAL_OK){
			return status;
		}
	return HAL_OK;
}

void STATS_Reset(void){
	temp_stats.max = 0.0f;
	temp_stats.min = 0.0f;
	temp_stats.count = 0;
	temp_stats.sum = 0.0f;
	hum_stats.max = 0.0f;
	hum_stats.min = 0.0f;
	hum_stats.count = 0;
	hum_stats.sum = 0.0f;
	press_stats.max = 0.0f;
	press_stats.min = 0.0f;
	press_stats.count = 0;
	press_stats.sum = 0.0f;
}

HAL_StatusTypeDef STATS_GetHistory(DailyStats_t *buffer, uint8_t *count){
	if(buffer == NULL || count == NULL)
	{
	    return HAL_ERROR;
	}
	for(int i = 0; i < (history_count);i++){
		buffer[i] = history[i];
	}
	*count = history_count;
	return HAL_OK;
}


HAL_StatusTypeDef STATS_RestoreHistory(DailyStats_t *stats, uint8_t value){
if(stats == NULL){
return HAL_ERROR;
}
if(value > 7){
return HAL_ERROR;
}
for(uint8_t i = 0; i < value ; i++){
history[i] = stats[i];
}
history_count = value;
return HAL_OK;
}
