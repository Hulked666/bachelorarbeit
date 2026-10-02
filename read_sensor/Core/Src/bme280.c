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
uint8_t reg_status = 0xf3;
uint8_t reg_raw_data = 0xf7;


static uint32_t raw_temperature;
static uint32_t raw_pressure;
static uint16_t raw_humidity;


static uint32_t t_fine;




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
//checks if sensor is still measuring and saves it in a variable
static HAL_StatusTypeDef is_mesuring(bool *is_active){
	HAL_StatusTypeDef status;
	uint8_t status_reg;
	status = read_data(reg_status, &status_reg ,1);
	if(status != HAL_OK){
		return status;
	}else{
		if((status_reg & (1U << 3)) == 0){

		*is_active = false;
	}else{
		*is_active = true;
	}
}
	return HAL_OK;
}


static HAL_StatusTypeDef wait_for_measurement(uint32_t time){
	HAL_StatusTypeDef status;
	bool measuring;
	uint32_t start_time;

	start_time = HAL_GetTick();

	while(1){
		status = is_mesuring(&measuring);
		if(status != HAL_OK){
			return status;
		}

		if(measuring == false){
			return HAL_OK;
		}

		if((HAL_GetTick() - start_time) >= time ){
			return HAL_TIMEOUT;
		}

		HAL_Delay(1);
	}


}
// reading sensor raw data before compensation
static HAL_StatusTypeDef read_raw_data(void){
	HAL_StatusTypeDef status;
	uint8_t buffer[8];
	status = read_data(reg_raw_data, buffer,8);

	if(status != HAL_OK){
		return status;
	}

	raw_pressure = (((uint32_t)buffer[0] << 12) | (buffer[1] << 4) | (buffer[2] >> 4));
	raw_temperature = (((uint32_t)buffer[3] << 12) | (buffer[4] << 4) | (buffer[5] >> 4));
	raw_humidity = ((uint16_t)(buffer[6] << 8) | (buffer[7]));

	return HAL_OK;

}

static uint32_t compensate_temp(uint32_t adc_T){
    int32_t var1;
    int32_t var2;
    int32_t temperature;

    var1 = (((int32_t)adc_T >> 3)
            - ((int32_t)calibrationData.dig_T1 << 1));

    var1 = (var1 * (int32_t)calibrationData.dig_T2) >> 11;


    var2 = ((int32_t)adc_T >> 4)
            - (int32_t)calibrationData.dig_T1;

    var2 = (((var2 * var2) >> 12)
            * (int32_t)calibrationData.dig_T3) >> 14;

    t_fine = var1 + var2;

    temperature = (t_fine * 5 + 128) >> 8;

    return temperature;
}

static uint32_t compensate_pressure(uint32_t adc_P)
{
    int64_t var1;
    int64_t var2;
    int64_t pressure;

    var1 = ((int64_t)t_fine) - 128000;

    var2 = var1 * var1 * (int64_t)calibrationData.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calibrationData.dig_P5) << 17);
    var2 = var2 + (((int64_t)calibrationData.dig_P4) << 35);

    var1 = ((var1 * var1 * (int64_t)calibrationData.dig_P3) >> 8)
         + ((var1 * (int64_t)calibrationData.dig_P2) << 12);

    var1 = (((((int64_t)1) << 47) + var1)
           * (int64_t)calibrationData.dig_P1) >> 33;

    if (var1 == 0)
    {
        return 0;
    }

    pressure = 1048576 - (int64_t)adc_P;

    pressure = (((pressure << 31) - var2) * 3125) / var1;

    var1 = ((int64_t)calibrationData.dig_P9
           * (pressure >> 13)
           * (pressure >> 13)) >> 25;

    var2 = ((int64_t)calibrationData.dig_P8
           * pressure) >> 19;

    pressure = ((pressure + var1 + var2) >> 8)
             + (((int64_t)calibrationData.dig_P7) << 4);

    return (uint32_t)pressure;
}


static uint32_t compensate_humidity(uint16_t adc_H)
{
    int32_t value;

    value = t_fine - 76800;

    value = (((((int32_t)adc_H << 14)
              - ((int32_t)calibrationData.dig_H4 << 20)
              - ((int32_t)calibrationData.dig_H5 * value))
              + 16384) >> 15)
            *
            (((((((value * (int32_t)calibrationData.dig_H6) >> 10)
                * (((value * (int32_t)calibrationData.dig_H3) >> 11)
                + 32768)) >> 10)
                + 2097152)
                * (int32_t)calibrationData.dig_H2
                + 8192) >> 14);

    value = value -
            (((((value >> 15) * (value >> 15)) >> 7)
            * (int32_t)calibrationData.dig_H1) >> 4);

    if (value < 0)
    {
        value = 0;
    }

    if (value > 419430400)
    {
        value = 419430400;
    }

    return (uint32_t)(value >> 12);
}

HAL_StatusTypeDef BME280ReadMesurements(float *temperature, *pressure, *humidity){
	HAL_StatusTypeDef status;
	status = trigger_forced_mes();
	if(status != HAL_OK){
		return status;
	}

	status = wait_for_measurement(100);
		if(status != HAL_OK){
			return status;
		}

	status = read_raw_data();
		if(status != HAL_OK){
			return status;
		}


	*temperature = compensate_temp(raw_temp) / 100.f;
	*pressure = compensate_pressure(raw_pressure) / 25600.0f;
	*humidity = compensate_humidity(raw_humidity) / 1024.0f;
}

