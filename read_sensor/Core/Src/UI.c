#include "UI.h"
#include "display.h"
#include <stdio.h>
#include <stdint.h>



static UI_View_t current_state = CURRENT_VIEW;

HAL_StatusTypeDef UI_DrawTempBox(float temperature);
HAL_StatusTypeDef UI_DrawHumBox(int humidity);
HAL_StatusTypeDef UI_DrawPressBox(int pressure);

HAL_StatusTypeDef UI_DrawCurrent(void){

	fill_screen(COLOR_WHITE);
	draw_rectangle(0,0,320,20,COLOR_BLUE);
	draw_rectangle(0,210,106,30,COLOR_CYAN);
	draw_rectangle(106,210,106,30,COLOR_CYAN);
	draw_rectangle(212,210,108,30,COLOR_CYAN);
	UI_DrawPressBox();
	UI_DrawTempBox();
	UI_DrawHumBox();
	draw_text(12,213,"AKTUELL",COLOR_WHITE,2);
	draw_text(141,213,"TAG",COLOR_WHITE,2);
	draw_text(237,213,"7 TAGE",COLOR_WHITE,2);


	return HAL_OK;

}

HAL_StatusTypeDef UI_DrawTempBox(float temperature){
	char temp_read[20];
	snprintf(temp_read, sizeof(temp_read),"%.1f \xB0""C", temperature);
	if(temperature > 32){
		draw_rectangle(0,20,106,190,COLOR_RED);
	}else if((temperature > 25) && (temperature <= 32)){
		draw_rectangle(0,20,106,190,COLOR_ORANGE);
	}else if((temperature > 18 ) && (temperature <= 25)){
		draw_rectangle(0,20,106,190,COLOR_GREEN);
	}else if((temperature > 10) && (temperature <= 18)){
		draw_rectangle(0,20,106,190,COLOR_CYAN);
	}else{
		draw_rectangle(0,20,106,190,COLOR_BLUE);
	}

	draw_text(19,105,"TEMPERATUR", COLOR_WHITE,2);
	draw_text(17,117,temp_read,COLOR_WHITE,2);

	return HAL_OK;

}

HAL_StatusTypeDef UI_DrawHumBox(int humidity){
	char hum_read[20];
	snprintf(hum_read,sizeof(hum_read),"%d %%", humidity);
	if(humidity > 70){
		draw_rectangle(106,20,106,190,COLOR_RED);
	}else if((humidity <= 70) && (humidity > 45)){
		draw_rectangle(106,20,106,190,COLOR_GREEN);
	}else{
		draw_rectangle(106,20,106,190,COLOR_YELLOW);
	}

	draw_text(112,98,"FEUCHTIG",COLOR_WHITE,2);
	draw_text(136,106,"KEIT",COLOR_WHITE,2);
	draw_text(136,117,hum_read,COLOR_WHITE,2);

	return HAL_OK;
}

HAL_StatusTypeDef UI_DrawPressBox(int pressure){
	char press_read[20];
	snprintf(press_read,sizeof(press_read),"%d hPa",pressure);
	draw_rectangle(212,20,108,190,COLOR_GREEN);
	draw_text(214,105,"LUFTDRUCK",COLOR_WHITE,2);
	draw_text(224,117,press_read,COLOR_WHITE,2);

	return HAL_OK;
}
