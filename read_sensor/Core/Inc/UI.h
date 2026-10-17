#include "stm32l4xx_hal.h"
#include <stdint.h>

#ifndef INC_UI_H_
#define INC_UI_H_

typedef enum{
	CURRENT_VIEW,
	DAILY_VIEW,
	HISTORY_VIEW
} UI_View_t;

typedef enum
{
    HISTORY_TEMP,
    HISTORY_HUM,
    HISTORY_PRESS
} UI_HistoryMetric_t;


HAL_StatusTypeDef UI_DrawCurrent(void);
void UI_SetMeasurement(float temperature, int humidity, int pressure);
HAL_StatusTypeDef UI_DrawValues(void);
HAL_StatusTypeDef UI_DrawHistoryStatic(void);
HAL_StatusTypeDef UI_DrawHistoryValues(void);
HAL_StatusTypeDef UI_DrawDailyValues(void);
HAL_StatusTypeDef UI_DrawDailyStatic(void);



#endif /* INC_UI_H_ */
