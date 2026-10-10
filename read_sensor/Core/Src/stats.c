#include "stats.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>


static StatsStruct_t temp_stats;
static StatsStruct_t hum_stats;
static StatsStruct_t press_stats;

static DailyStats_t history[7];

static uint8_t histoy_count = 0;

HAL_StatusTypeDef STATS_Update(StatsStruct_t *stats, float value){

	if(stats == 0){
		return HAL_ERROR;
	}else if(stats->count == 0){
		stats-> min = value;
		stats-> max = value;
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
