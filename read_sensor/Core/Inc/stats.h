#
#ifndef INC_STATS_H_
#define INC_STATS_H_

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


#endif /* INC_STATS_H_ */
