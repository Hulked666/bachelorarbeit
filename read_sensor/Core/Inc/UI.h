#include "stm32l4xx_hal.h"
#include <stdint.h>

#ifndef INC_UI_H_
#define INC_UI_H_

typedef enum{
	CURRENT_VIEW,
	DAILY_VIEW,
	HISTORY_VIEW
} UI_View_t;


HAL_StatusTypeDef UI_DrawCurrent(void);
void UI_SetMeasurement(float temperature, int humidity, int pressure);
HAL_StatusTypeDef UI_DrawValues(void);

#endif /* INC_UI_H_ */
