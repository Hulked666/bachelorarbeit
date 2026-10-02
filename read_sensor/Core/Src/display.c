#include "display.h"
#include "main.h"




extern SPI_HandleTypeDef hspi1;

static const uint8_t reset_val = 0x01;
static const uint8_t sleep_out = 0x11;
static const uint8_t pixel_format = 0x3A;
static const uint8_t bit_p_pixel = 0x55;
static const uint8_t display_on = 0x29;
static const uint8_t mem_acc_ctrl = 0x36;
static const uint8_t disp_orient = 0x28;
static const uint8_t set_window_x = 0x2A;
static const uint8_t set_window_y = 0x2B;
static const uint8_t mem_write = 0x2C;

static HAL_StatusTypeDef write_command(uint8_t command){
	HAL_StatusTypeDef status;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
	status = HAL_SPI_Transmit(&hspi1, &command,1,100);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
	return status;
}



static HAL_StatusTypeDef write_data(uint8_t data){
	HAL_StatusTypeDef status;
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TFT_DC_GPIO_Port,TFT_DC_Pin,GPIO_PIN_SET);
	status = HAL_SPI_Transmit(&hspi1, &data,1,100);
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_SET);
	return status;
}



 static HAL_StatusTypeDef write_data_buffer(uint8_t *data, uint16_t size){
		HAL_StatusTypeDef status;
		HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(TFT_DC_GPIO_Port,TFT_DC_Pin,GPIO_PIN_SET);
		status = HAL_SPI_Transmit(&hspi1, data,size,100);
		HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_SET);
		return status;
 }

 static void hardware_reset(void){
	 HAL_GPIO_WritePin(TFT_RST_GPIO_Port,TFT_RST_Pin,GPIO_PIN_RESET);
	 HAL_Delay(100);
	 HAL_GPIO_WritePin(TFT_RST_GPIO_Port,TFT_RST_Pin,GPIO_PIN_SET);
	 HAL_Delay(100);
 }

 HAL_StatusTypeDef display_init(void){
	 HAL_StatusTypeDef status;
	 hardware_reset();
	 status = write_command(reset_val);
	 if(status != HAL_OK){
		 return status;
	 }
	 HAL_Delay(50);
	 status = write_command(sleep_out);
	 if(status != HAL_OK){
		 return status;
	 }

	 HAL_Delay(120);

	 status = write_command(pixel_format);
	 if(status != HAL_OK){
		 return status;
	 }

	 status = write_data(bit_p_pixel);
	 if(status != HAL_OK){
		 return status;
	 }

	 status = write_command(mem_acc_ctrl);
	 if(status != HAL_OK){
		 return status;
	 }

	 status = write_data(disp_orient);
	 if(status != HAL_OK){
		 return status;
	 }

	 status = write_command(display_on);
	 if(status != HAL_OK){
		 return status;
	 }

	 HAL_Delay(10);

	 return HAL_OK;
 }


 HAL_StatusTypeDef set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	 HAL_StatusTypeDef status;
	 status = write_command(set_window_x);
	 if(status != HAL_OK){
		 return status;
	 }
	 uint8_t buff[4];
	 buff[0] = (x1 >> 8);
	 buff[1] = (x1 & 0xFF);
	 buff[2] = (x2 >> 8);
	 buff[3] = (x2 & 0xFF);
	 status = write_data_buffer(buff,4);
	 if(status != HAL_OK){
		 return status;
	 }

	 status = write_command(set_window_y);
	 if(status != HAL_OK){
		 return status;
	 }

	 buff[0] = (y1 >> 8);
	 buff[1] = (y1 & 0xFF);
	 buff[2] = (y2 >> 8);
	 buff[3] = (y2 & 0xFF);
	 status = write_data_buffer(buff,4);
	 if(status != HAL_OK){
		 return status;
	 }

	 status = write_command(mem_write);
	 if(status != HAL_OK){
		 return status;
	 }
	 return HAL_OK;
 }
