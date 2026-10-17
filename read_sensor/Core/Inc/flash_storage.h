
#ifndef INC_FLASH_STORAGE_H_
#define INC_FLASH_STORAGE_H_

#include "stm32l4xx_hal.h"
#include "stats.h"

HAL_StatusTypeDef FLASH_SaveHistory(void);
HAL_StatusTypeDef FLASH_LoadHistory(void);



#endif /* INC_FLASH_STORAGE_H_ */
