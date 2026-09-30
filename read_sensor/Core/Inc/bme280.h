#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef INC_BME280_H_
#define INC_BME280_H_

HAL_StatusTypeDef read_data(uint8_t regAdress, uint8_t *pBuffer, uint16_t byteSize);

#endif /* INC_BME280_H_ */
