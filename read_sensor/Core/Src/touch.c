#include "main.h"
#include "touch.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#define TOUCH_RAW_Y_LEFT    3650U
#define TOUCH_RAW_Y_RIGHT    370U

#define TOUCH_RAW_X_TOP     3460U
#define TOUCH_RAW_X_BOTTOM   520U

extern SPI_HandleTypeDef hspi1;




static uint16_t touch_read_raw(uint8_t command){
	uint8_t tx[3];
	    uint8_t rx[3];
	    uint16_t value;

	    tx[0] = command;
	    tx[1] = 0;
	    tx[2] = 0;

	    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);

	    HAL_SPI_TransmitReceive(&hspi1,
	                            tx,
	                            rx,
	                            3,
	                            100);

	    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);

	    value = (((uint16_t)rx[1] << 8) | rx[2]) >> 3;

	    return value;
}


HAL_StatusTypeDef TOUCH_ReadRaw(uint16_t *raw_x,
                                uint16_t *raw_y){
    *raw_x = touch_read_raw(0xD0);
    *raw_y = touch_read_raw(0x90);

    return HAL_OK;
}

HAL_StatusTypeDef TOUCH_Read(uint16_t *x, uint16_t *y ){
	HAL_StatusTypeDef status;
	uint16_t raw_x;
	uint16_t raw_y;


	status = TOUCH_ReadRaw(&raw_x, &raw_y);
	if(status != HAL_OK){
			return status;
	}
	if(raw_x > TOUCH_RAW_X_TOP){
		raw_x = TOUCH_RAW_X_TOP;
	}
	if(raw_x < TOUCH_RAW_X_BOTTOM){
		raw_x = TOUCH_RAW_X_BOTTOM;
	}
	if(raw_y > TOUCH_RAW_Y_LEFT){
		raw_y = TOUCH_RAW_Y_LEFT;
	}
	if(raw_y < TOUCH_RAW_Y_RIGHT){
		raw_y = TOUCH_RAW_Y_RIGHT;
	}

	*x = ((uint32_t)(TOUCH_RAW_Y_LEFT - raw_y) * 319) / (TOUCH_RAW_Y_LEFT - TOUCH_RAW_Y_RIGHT);
	*y = ((uint32_t)(TOUCH_RAW_X_TOP - raw_x) * 239) / (TOUCH_RAW_X_TOP - TOUCH_RAW_X_BOTTOM);

	return HAL_OK;
}

bool TOUCH_IsPressed(void){

	if(HAL_GPIO_ReadPin(TOUCH_IRQ_GPIO_Port, TOUCH_IRQ_Pin) == GPIO_PIN_RESET){
		return true;
	}else {
		return false;
	}
}

static bool touch_was_down = false;


bool TOUCH_WasPressed(void){
	if(TOUCH_IsPressed()){
		if(touch_was_down == false){
			touch_was_down = true;
			return true;
		}
		return false;
	}else
	{
		touch_was_down = false;
		return false;
	}

}


