#include "flash_storage.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>


#define FLASH_STORAGE_ADDRESS 0x080FF800U
#define FLASH_MAGIC 0x57454154U

HAL_StatusTypeDef FLASH_ErasePage(void){
	HAL_StatusTypeDef status;
	status = HAL_FLASH_Unlock();
	if(status != HAL_OK){
		return status;
	}

	 FLASH_EraseInitTypeDef erase;
	 erase.TypeErase = FLASH_TYPEERASE_PAGES;
	 erase.Banks     = FLASH_BANK_2;
	 erase.Page      = 255;
	 erase.NbPages   = 1;

	 uint32_t page_error;

	 status = HAL_FLASHEx_Erase(&erase, &page_error);
	 HAL_FLASH_Lock();
	 if(status != HAL_OK){
		 return status;
	 }

	 return HAL_OK;
}

HAL_StatusTypeDef FLASH_Save(void){
	HAL_StatusTypeDef status;
	FlashStorage_t flash = {0};
	flash.magic = FLASH_MAGIC;
	status = STATS_GetHistory(flash.history, &flash.history_count);
	if(status != HAL_OK){
		return status;
	}
	status = FLASH_ErasePage();
	if(status != HAL_OK){
		return status;
	}
	status = HAL_FLASH_Unlock();
	if(status != HAL_OK){
		return status;

	}
	for(uint32_t i = 0; i < sizeof(FlashStorage_t)/8; i++){
	    uint64_t data;

	    memcpy(&data,
	           ((uint8_t *)&flash) + (i * 8),
	           sizeof(data));
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_STORAGE_ADDRESS + (i *8), data);
		if(status != HAL_OK){
			HAL_FLASH_Lock();
			return status;
		}
	}
	HAL_FLASH_Lock();
	return HAL_OK;
}

HAL_StatusTypeDef FLASH_Load(void){
	HAL_StatusTypeDef status;
	const FlashStorage_t *flash;
	flash = (const FlashStorage_t *)FLASH_STORAGE_ADDRESS;
	if(flash->magic != FLASH_MAGIC){
		return HAL_ERROR;
	}

	if(flash->history_count > 7){
		return HAL_ERROR;
	}
	status = STATS_RestoreHistory(flash->history, flash->history_count);
	if(status != HAL_OK){
		return status;
	}

	return HAL_OK;
}
