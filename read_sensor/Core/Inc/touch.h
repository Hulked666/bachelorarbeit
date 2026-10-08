

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_


#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
HAL_StatusTypeDef TOUCH_ReadRaw(uint16_t *raw_x,
                                uint16_t *raw_y);


HAL_StatusTypeDef TOUCH_Read(uint16_t *x, uint16_t *y );

bool TOUCH_IsPressed(void);


bool TOUCH_WasPressed(void);
#endif /* INC_TOUCH_H_ */

