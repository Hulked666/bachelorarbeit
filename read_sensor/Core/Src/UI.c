#include "UI.h"
#include "display.h"
#include <stdio.h>
#include <stdint.h>



static float current_temperature;
static int current_humidity;
static int current_pressure;

static int last_temp_display;
static uint16_t last_temp_color;

static int last_hum_display;
static uint16_t last_hum_color;

static int last_press_display;



static UI_View_t current_state = CURRENT_VIEW;

HAL_StatusTypeDef UI_DrawTempBox(float temperature);
HAL_StatusTypeDef UI_DrawHumBox(int humidity);
HAL_StatusTypeDef UI_DrawPressBox(int pressure);
static HAL_StatusTypeDef UI_TempValueUpdate(float temperature);
static HAL_StatusTypeDef UI_HumValueUpdate(int humidity);
static HAL_StatusTypeDef UI_PressValueUpdate(int pressure);

HAL_StatusTypeDef UI_DrawCurrent(void){

	fill_screen(COLOR_WHITE);
	draw_rectangle(0,0,320,20,COLOR_BLUE);
	draw_rectangle(0,210,106,30,COLOR_CYAN);
	draw_rectangle(106,210,106,30,COLOR_CYAN);
	draw_rectangle(212,210,108,30,COLOR_CYAN);
	draw_text(12,213,"AKTUELL",COLOR_WHITE,2);
	draw_text(141,213,"TAG",COLOR_WHITE,2);
	draw_text(237,213,"7 TAGE",COLOR_WHITE,2);
	UI_DrawPressBox(current_pressure);
	UI_DrawTempBox(current_temperature);
	UI_DrawHumBox(current_humidity);


	return HAL_OK;

}

HAL_StatusTypeDef UI_DrawValues(void){
	UI_TempValueUpdate(current_temperature);
	UI_HumValueUpdate(current_humidity);
	UI_PressValueUpdate(current_pressure);
	return HAL_OK;
}

HAL_StatusTypeDef UI_DrawTempBox(float temperature){
	draw_rectangle(0,20,106,190,COLOR_WHITE);
    draw_text(2,85,"TEMPERATUR", COLOR_BLACK,1);
	last_temp_color= COLOR_WHITE;
	last_temp_display = (int) - 10000;
	return HAL_OK;
}

HAL_StatusTypeDef UI_DrawHumBox(int humidity){

	draw_rectangle(106,20,106,190,COLOR_WHITE);
	draw_text(112,69,"FEUCHTIG",COLOR_BLACK,2);
	draw_text(136,85,"KEIT",COLOR_BLACK,2);
	last_hum_display = -1000;
	last_hum_color = 0xFFFF;


	return HAL_OK;
}

HAL_StatusTypeDef UI_TempValueUpdate(float temperature){
    char temp_read[20];
    uint16_t new_color;
    int temp_display;

    temp_display = (int)(temperature * 10.0f);

    if(temperature > 32.0f)
    {
        new_color = COLOR_RED;
    }
    else if(temperature > 25.0f)
    {
        new_color = COLOR_ORANGE;
    }
    else if(temperature > 18.0f)
    {
        new_color = COLOR_GREEN;
    }
    else if(temperature > 10.0f)
    {
        new_color = COLOR_CYAN;
    }
    else
    {
        new_color = COLOR_BLUE;
    }

    snprintf(temp_read, sizeof(temp_read), "%.1f \xB0""C", temperature);

    if(new_color != last_temp_color)
    {
        draw_rectangle(0, 20, 106, 190, new_color);

        draw_text(2,85,"TEMPERATUR",COLOR_BLACK,1);

        draw_text(17, 117, temp_read, COLOR_BLACK,2);

        last_temp_color = new_color;
        last_temp_display = temp_display;
    }
    else if(temp_display != last_temp_display)
    {

        draw_rectangle(10,115,92,20,last_temp_color);

        draw_text(17,117,temp_read,COLOR_BLACK,2);

        last_temp_display = temp_display;
    }

    return HAL_OK;
}

static HAL_StatusTypeDef UI_HumValueUpdate(int humidity)
{
    char hum_read[20];
    uint16_t new_color;

    if(humidity > 70)
    {
        new_color = COLOR_RED;
    }
    else if(humidity > 45)
    {
        new_color = COLOR_GREEN;
    }
    else
    {
        new_color = COLOR_YELLOW;
    }

    snprintf(hum_read, sizeof(hum_read), "%d %%", humidity);

    if(new_color != last_hum_color)
    {
        draw_rectangle(106,20,106,190,new_color);

        draw_text(112,69, "FEUCHTIG", COLOR_BLACK,2);

        draw_text(136,85,"KEIT",COLOR_BLACK,2);

        draw_text(128,120,hum_read,COLOR_BLACK,2);

        last_hum_color = new_color;
        last_hum_display = humidity;
    }
    else if(humidity != last_hum_display)
    {
        draw_rectangle(120, 118, 80,20,last_hum_color);

        draw_text(128,120,hum_read,COLOR_BLACK,2);

        last_hum_display = humidity;
    }

    return HAL_OK;
}

HAL_StatusTypeDef UI_DrawPressBox(int pressure){
	draw_rectangle(212,20,108,190,COLOR_WHITE);
	draw_text(214,85,"LUFTDRUCK",COLOR_BLACK,2);

	last_press_display = -1000;

	return HAL_OK;
}

static HAL_StatusTypeDef UI_PressValueUpdate(int pressure)
{
    char press_read[20];

    if(pressure != last_press_display)
    {
        snprintf(press_read,sizeof(press_read),"%d hPa",pressure);


        draw_rectangle(218,115,98,22,COLOR_WHITE);
        draw_text(244,117,press_read,COLOR_BLACK,2);

        last_press_display = pressure;
    }

    return HAL_OK;
}

void UI_SetMeasurement(float temperature, int humidity, int pressure){
	current_temperature = temperature;
	current_humidity = humidity;
	current_pressure = pressure;
}
