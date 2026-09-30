#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bme280.h"

uint8_t buff[1];
uint16_t dev_adr = 0x76 << 1;
uint16_t id_adr = 0xD0;
uint8_t ref_adr = 0x60;
uint8_t resetReg = 0xE0;
uint8_t calibReg = 0x88;
uint8_t calibReg2 = 0xE1;

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


calibrationData_t calibrationData;


  static HAL_StatusTypeDef read_data(uint8_t regAdress,
			  	  	  	  	  	  	  	 uint8_t *pBuffer,
										 uint16_t byteSize){

		  HAL_StatusTypeDef status;
		  status = HAL_I2C_Mem_Read(&hi2c1,
		  	  	  	  	  	  	  	dev_adr,
									regAdress,
									I2C_MEMADD_SIZE_8BIT,
									pBuffer,
									byteSize,
									100);
		  return status;
	  };


  static bool read_chip_id(void){
	  HAL_StatusTypeDef status;
	  uint8_t chip_id = 0;
	  status = read_data(id_adr,&chip_id,1);
	  if(status == HAL_OK){
		if(ref_adr == chip_id){
			return 1;
		}else{
			return 0;
		}
		}else{
			return 0;
	  }
  }


  static HAL_StatusTypeDef write_register(uint8_t regAdress,
		  	  	  	  	  	  	  	  	  uint8_t *pData,
										  uint16_t byteSize){
	  HAL_StatusTypeDef status;
	  status = HAL_I2C_Mem_Write(&hi2c1,
		  	  	  	  	  	  	 dev_adr,
								 regAdress,
								 I2C_MEMADD_SIZE_8BIT,
								 pData,
								 byteSize,
								 100);
	  return status;
  }

  static HAL_StatusTypeDef soft_reset(void){
	  HAL_StatusTypeDef status;
	  uint8_t reset_value = 0xB6;
	  status = write_register(resetReg, &reset_value, 1);
	  return status;


  }


  static HAL_StatusTypeDef im_update(bool *is_active){
	  HAL_StatusTypeDef status;
	  uint8_t status_reg = 0XF3;
	  uint8_t status_value = 0;

	  status = read_data(status_reg, &status_value,1);
	  if((status != HAL_OK) ){
		return status;
	  }else{
		  if((status_value & (1U << 0)) != 0){
			  *is_active = true;
		  } else{
			  *is_active = false;
		  }
		  return status;
	  }

  }


  static HAL_StatusTypeDef read_calibration_1(void) {
	  HAL_StatusTypeDef status;
	  uint8_t dataBuffer[26];
	  status = read_data(calibReg, dataBuffer,26);

	  if(status != HAL_OK){
		  return status;
	  }else{
		  calibrationData.dig_T1 = ((uint16_t)dataBuffer[1] << 8) | dataBuffer[0];
		  calibrationData.dig_T2 = (int16_t)(((uint16_t)dataBuffer[3] << 8) | dataBuffer[2]);
		  calibrationData.dig_T3 = (int16_t)(((uint16_t)dataBuffer[5] << 8) | dataBuffer[4]);
		  calibrationData.dig_P1 = ((uint16_t)dataBuffer[7] << 8) | dataBuffer[6];
		  calibrationData.dig_P2 = (int16_t)(((uint16_t)dataBuffer[9] << 8) | dataBuffer[8]);
		  calibrationData.dig_P3 = (int16_t)(((uint16_t)dataBuffer[11] << 8) | dataBuffer[10]);
		  calibrationData.dig_P4 = (int16_t)(((uint16_t)dataBuffer[13] << 8) | dataBuffer[12]);
		  calibrationData.dig_P5 = (int16_t)(((uint16_t)dataBuffer[15] << 8) | dataBuffer[14]);
		  calibrationData.dig_P6 = (int16_t)(((uint16_t)dataBuffer[17] << 8) | dataBuffer[16]);
		  calibrationData.dig_P7 = (int16_t)(((uint16_t)dataBuffer[19] << 8) | dataBuffer[18]);
		  calibrationData.dig_P8 = (int16_t)(((uint16_t)dataBuffer[21] << 8) | dataBuffer[20]);
		  calibrationData.dig_P9 = (int16_t)(((uint16_t)dataBuffer[23] << 8) | dataBuffer[22]);
		  calibrationData.dig_H1 = ((uint8_t)dataBuffer[25]);
		  return status;
	  }
  }

  static HAL_StatusTypeDef read_calibration_2(void){
	  HAL_StatusTypeDef status;
	  uint8_t dataBuffer[7];
	  status  = read_data(calibReg2,dataBuffer,7);
	  if(status != HAL_OK){
		  return false;

	  }

	 calibrationData.dig_H2 = (int16_t)(((uint16_t)dataBuffer[1] << 8 ) | dataBuffer[0]);
	 calibrationData.dig_H3 = ((uint8_t) dataBuffer[2]);
	 calibrationData.dig_H4 = ((uint8_t) dataBuffer[3]);
	 calibrationData.dig_H5 = ((uint8_t) dataBuffer[3]);
	 calibrationData.dig_H6 = ((uint8_t) dataBuffer[3]);


  }

