#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bme280.h"
#include <stdbool.h>
extern I2C_HandleTypeDef hi2c1;

uint8_t buff[1];
uint16_t dev_adr = 0x76 << 1;
uint16_t id_adr = 0xD0;
uint8_t ref_adr = 0x60;
uint8_t resetReg = 0xE0;
uint8_t calibReg = 0x88;
uint8_t calibReg2 = 0xE1;
uint8_t reg_ctrl_hum = 0xF2;
uint8_t reg_sctrl_meas =  0xf4;




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
// does a sensor soft reset
  static HAL_StatusTypeDef soft_reset(void){
	  HAL_StatusTypeDef status;
	  uint8_t reset_value = 0xB6;
	  status = write_register(resetReg, &reset_value, 1);
	  return status;


  }

// checks if is in update and saves it in a variable
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

  // reads calibration data in burst mode for temp, press, and hum from t1 - h1 and saves them
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
// reads calibration data in burst mode for humidity from h2 - h6 and saves them
  static HAL_StatusTypeDef read_calibration_2(void){
	  HAL_StatusTypeDef status;
	  uint8_t dataBuffer[7];
	  status  = read_data(calibReg2,dataBuffer,7);
	  if(status != HAL_OK){
		  return status;

	  }

	 calibrationData.dig_H2 = (int16_t)(((uint16_t)dataBuffer[1] << 8 ) | dataBuffer[0]);
	 calibrationData.dig_H3 = ((uint8_t) dataBuffer[2]);
	 	 	 	 	 	 	 uint16_t h4_raw = (dataBuffer[3] << 4) | (dataBuffer[4] & 0x0F);
	 	 	 	 	 	 	 	if((h4_raw) & 0x0800 ){
	 	 	 	 	 	 	 		h4_raw |= 0xF000;
	 	 	 	 	 	 	 	}
	 calibrationData.dig_H4 = (int16_t)h4_raw;
	 	 	 	 	 	 	uint16_t h5_raw = (dataBuffer[5] << 4) | (dataBuffer[4] >> 4);
	 	 	 	 	 	 	if((h5_raw) & 0x0800 ){
	 	 	 	 	 	 		h5_raw |= 0xF000;
	 	 	 	 	 	 	}
	 calibrationData.dig_H5 = (int16_t)h5_raw;
	 calibrationData.dig_H6 = ((int8_t) dataBuffer[6]);
	 return status;
  }
// checks calibration data status
  static HAL_StatusTypeDef read_calibration_data(void){
	  HAL_StatusTypeDef status;
	  status = read_calibration_1();
	  if(status != HAL_OK){
		  return status;
	  }

	  status = read_calibration_2();
	  if (status != HAL_OK){
		  return status;
	  }


	  return HAL_OK;
  }

// Reads raw data for calibration and saves them in strict
  HAL_StatusTypeDef BME280_ReadCalibration(calibrationData_t *data)
  {
      HAL_StatusTypeDef status;

      status = read_calibration_data();

      if (status != HAL_OK)
      {
          return status;
      }

      *data = calibrationData;

      return HAL_OK;
  }

// sets oversampling rate for humidity on 1x
static HAL_StatusTypeDef set_hum_oversampling(void){
	HAL_StatusTypeDef status;
	uint8_t rate = 0x01;
	status = write_register(reg_ctlr_hum, &rate,1);
	return status;
}

//sets oversampampling for temp and humidity and triggers forced mode
static HAL_StatusTypeDef trigger_forced_mes(void){
	HAL_StatusTypeDef status;
	uint8_t rate = 0x25;
	status = write_register(reg_ctrl_meas, &rate, 1);
	return status;
}
