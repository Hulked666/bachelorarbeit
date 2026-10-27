
#ifndef INC_FLASH_STORAGE_H_
#define INC_FLASH_STORAGE_H_

#include "stm32l4xx_hal.h"
#include "stats.h"

typedef struct
{
    uint32_t magic;
    uint8_t history_count;
    uint8_t reserved[3];

    DailyStats_t history[7];

} FlashStorage_t;

HAL_StatusTypeDef FLASH_Load(void);
HAL_StatusTypeDef FLASH_Save(void);



#endif /* INC_FLASH_STORAGE_H_ */
