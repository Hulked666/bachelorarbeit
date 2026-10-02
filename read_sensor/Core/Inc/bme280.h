#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32l4xx_hal.h"


#ifndef INC_BME280_H_
#define INC_BME280_H_

typedef struct {
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t dig_H6;

} calibrationData_t;



HAL_StatusTypeDef BME280_ReadCalibration(calibrationData_t *data);



#endif /* INC_BME280_H_ */
