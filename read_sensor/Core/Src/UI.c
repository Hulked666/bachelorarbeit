#include "UI.h"
#include "display.h"
#include "stats.h"
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

UI_HistoryMetric_t history_metric;
static UI_HistoryMetric_t current_metric = HISTORY_HUM;



HAL_StatusTypeDef UI_DrawTempBox(float temperature);
HAL_StatusTypeDef UI_DrawHumBox(int humidity);
HAL_StatusTypeDef UI_DrawPressBox(int pressure);
static HAL_StatusTypeDef UI_TempValueUpdate(float temperature);
static HAL_StatusTypeDef UI_HumValueUpdate(int humidity);
static HAL_StatusTypeDef UI_PressValueUpdate(int pressure);
HAL_StatusTypeDef UI_DrawDaily(void);

HAL_StatusTypeDef UI_DrawCurrent(void){

	fill_screen(COLOR_BLACK);

	UI_DrawPressBox(current_pressure);
	UI_DrawTempBox(current_temperature);
	UI_DrawHumBox(current_humidity);

	draw_rectangle(0,   209, 105, 30, COLOR_WHITE);
	draw_rectangle(106, 209, 105, 30, COLOR_WHITE);
	draw_rectangle(212, 209, 108, 30, COLOR_WHITE);

	draw_text(20,  218, "AKTUELL", COLOR_BLACK, 1);
	draw_text(147, 218, "TAG",      COLOR_BLACK, 1);
	draw_text(242, 218, "7 TAGE",   COLOR_BLACK, 1);


	return HAL_OK;

}

HAL_StatusTypeDef UI_DrawValues(void){
	UI_TempValueUpdate(current_temperature);
	UI_HumValueUpdate(current_humidity);
	UI_PressValueUpdate(current_pressure);
	return HAL_OK;
}

HAL_StatusTypeDef UI_DrawTempBox(float temperature)
{

    draw_rectangle(0, 0, 105, 208, COLOR_WHITE);
    draw_text(10, 10, "TEMPERATUR", COLOR_BLACK, 1);
    last_temp_color = COLOR_WHITE;
    last_temp_display = -10000;

    return HAL_OK;
}


HAL_StatusTypeDef UI_DrawHumBox(int humidity)
{

    draw_rectangle(107, 0, 104, 208, COLOR_WHITE);

    draw_text(113, 8, "FEUCHTIG", COLOR_BLACK, 2);
    draw_text(137, 25, "KEIT", COLOR_BLACK, 2);
    last_hum_display = -1000;
    last_hum_color = 0xFFFF;

    return HAL_OK;
}


HAL_StatusTypeDef UI_DrawPressBox(int pressure)
{

    draw_rectangle(213, 0, 107, 208, COLOR_WHITE);
    draw_text(215, 10, "LUFTDRUCK", COLOR_BLACK, 2);
    last_press_display = -1000;

    return HAL_OK;
}

HAL_StatusTypeDef UI_TempValueUpdate(float temperature)
{
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
        draw_rectangle(0, 0, 105, 208, new_color);

        draw_text(10, 10, "TEMPERATUR", COLOR_BLACK, 1);

        draw_text(17, 105, temp_read, COLOR_BLACK, 2);

        last_temp_color = new_color;
        last_temp_display = temp_display;
    }
    else if(temp_display != last_temp_display)
    {
        draw_rectangle(10, 103, 92, 22, last_temp_color);

        draw_text(17, 105, temp_read, COLOR_BLACK, 2);

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
        draw_rectangle(107, 0, 104, 208, new_color);

        draw_text(113, 8, "FEUCHTIG", COLOR_BLACK, 2);

        draw_text(137, 25, "KEIT", COLOR_BLACK, 2);

        draw_text(126, 105, hum_read, COLOR_BLACK, 2);

        last_hum_color = new_color;
        last_hum_display = humidity;
    }
    else if(humidity != last_hum_display)
    {
        draw_rectangle(118, 103, 82, 22, last_hum_color);

        draw_text(126, 105, hum_read, COLOR_BLACK, 2);

        last_hum_display = humidity;
    }

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

HAL_StatusTypeDef UI_DrawDailyStatic(void)
{
    fill_screen(COLOR_BLACK);

    draw_rectangle(0,   0, 105, 209, COLOR_WHITE);
    draw_rectangle(106, 0, 105, 209, COLOR_WHITE);
    draw_rectangle(212, 0, 108, 209, COLOR_WHITE);

    draw_text(10, 28, "TEMPERATUR", COLOR_BLACK, 1);
    draw_text(123, 28, "FEUCHTE", COLOR_BLACK, 1);
    draw_text(231, 28, "LUFTDRUCK", COLOR_BLACK, 1);

    draw_text(27, 55, "MINIMUM", COLOR_BLACK, 1);
    draw_text(133, 55, "MINIMUM", COLOR_BLACK, 1);
    draw_text(240, 55, "MINIMUM", COLOR_BLACK, 1);

    draw_text(27, 105, "MAXIMUM", COLOR_BLACK, 1);
    draw_text(133, 105, "MAXIMUM", COLOR_BLACK, 1);
    draw_text(240, 105, "MAXIMUM", COLOR_BLACK, 1);

    draw_text(32, 155, "MITTEL", COLOR_BLACK, 1);
    draw_text(138, 155, "MITTEL", COLOR_BLACK, 1);
    draw_text(245, 155, "MITTEL", COLOR_BLACK, 1);

    draw_rectangle(0,   210, 105, 30, COLOR_WHITE);
    draw_rectangle(106, 210, 105, 30, COLOR_WHITE);
    draw_rectangle(212, 210, 108, 30, COLOR_WHITE);

    draw_text(20, 219, "AKTUELL", COLOR_BLACK, 1);
    draw_text(147, 219, "TAG", COLOR_BLACK, 1);
    draw_text(242, 219, "7 TAGE", COLOR_BLACK, 1);

    return HAL_OK;
}

static HAL_StatusTypeDef UI_DrawDailyTemp(DailyStats_t *daily)
{
    char value[16];

    if(daily == NULL)
    {
        return HAL_ERROR;
    }
    draw_rectangle(4, 68, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value), "%.1f C", daily->temp_min);
    draw_text(18, 72, value, COLOR_BLACK, 2);
    draw_rectangle(4, 118, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value), "%.1f C", daily->temp_max);
    draw_text(18, 122, value, COLOR_BLACK, 2);
    draw_rectangle(4, 168, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value), "%.1f C",daily->temp_mean);
    draw_text(18, 172, value, COLOR_BLACK, 2);

    return HAL_OK;
}

static HAL_StatusTypeDef UI_DrawDailyHum(DailyStats_t *daily)
{
    char value[16];

    if(daily == NULL)
    {
        return HAL_ERROR;
    }

    draw_rectangle(111, 68, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value), "%.0f %%", daily->hum_min);
    draw_text(132, 72, value, COLOR_BLACK,2);
    draw_rectangle(111, 118, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value), "%.0f %%", daily->hum_max);
    draw_text(132, 122,value, COLOR_BLACK,2);
    draw_rectangle(111, 168, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value),"%.0f %%",daily->hum_mean);
    draw_text(132, 172,value,COLOR_BLACK,2);

    return HAL_OK;
}

static HAL_StatusTypeDef UI_DrawDailyPress(DailyStats_t *daily)
{
    char value[16];

    if(daily == NULL)
    {
        return HAL_ERROR;
    }
    draw_rectangle(218, 68, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value),"%.0f",daily->press_min);
    draw_text(235, 72,value,COLOR_BLACK,2);
    draw_rectangle(218, 118, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value),"%.0f",daily->press_max);
    draw_text(235, 122,value,COLOR_BLACK,2);
    draw_rectangle(218, 168, 98, 25, COLOR_WHITE);
    snprintf(value, sizeof(value),"%.0f",daily->press_mean);
    draw_text(235, 172,value,COLOR_BLACK,2);

    return HAL_OK;
}

HAL_StatusTypeDef UI_DrawDailyValues(void){
	HAL_StatusTypeDef status;
	DailyStats_t daily;
	status = CREATE_Daily(&daily);
	if(status != HAL_OK){
		return status;
	}
	status = UI_DrawDailyTemp(&daily);
	if(status != HAL_OK){
		return status;
	}
	status = UI_DrawDailyHum(&daily);
	if(status != HAL_OK){
		return status;
	}
	status = UI_DrawDailyPress(&daily);
	if(status != HAL_OK){
		return status;
	}


	return HAL_OK;

}


HAL_StatusTypeDef UI_DrawHistoryStatic(void)
{
    fill_screen(COLOR_BLACK);

    draw_rectangle(0,   0, 106, 28, COLOR_WHITE);
    draw_rectangle(107, 0, 106, 28, COLOR_WHITE);
    draw_rectangle(214, 0, 106, 28, COLOR_WHITE);

    draw_text(11, 6, "FEUCHTE", COLOR_BLACK, 2);
    draw_text(136, 6, "TEMP", COLOR_BLACK, 2);
    draw_text(237, 6, "DRUCK", COLOR_BLACK, 2);

    draw_rectangle(0,   29, 44, 179, COLOR_WHITE);
    draw_rectangle(46,  29, 44, 179, COLOR_WHITE);
    draw_rectangle(92,  29, 44, 179, COLOR_WHITE);
    draw_rectangle(138, 29, 44, 179, COLOR_WHITE);
    draw_rectangle(184, 29, 44, 179, COLOR_WHITE);
    draw_rectangle(230, 29, 44, 179, COLOR_WHITE);
    draw_rectangle(276, 29, 44, 179, COLOR_WHITE);

    draw_rectangle(0,   209, 106, 31, COLOR_WHITE);
    draw_rectangle(107, 209, 106, 31, COLOR_WHITE);
    draw_rectangle(214, 209, 106, 31, COLOR_WHITE);

    draw_text(32, 220, "AKTUELL", COLOR_BLACK, 1);
    draw_text(151, 220, "TAG", COLOR_BLACK, 1);
    draw_text(249, 220, "7 TAGE", COLOR_BLACK, 1);

    return HAL_OK;
}


static HAL_StatusTypeDef UI_DrawHistoryMetric(
        DailyStats_t *history,
        uint8_t count,
        UI_HistoryMetric_t metric)
{
    char date_read[8];
    char max_read[10];
    char mean_read[10];
    char min_read[10];

    uint16_t x_pos[7] = {0, 46, 92, 138, 184, 230, 276};

    if(history == NULL)
    {
        return HAL_ERROR;
    }

    if(count > 7)
    {
        count = 7;
    }

    for(uint8_t i = 0; i < count; i++)
    {
        float max_value;
        float mean_value;
        float min_value;

        uint16_t x = x_pos[i];

        switch(metric)
        {
            case HISTORY_TEMP:

                max_value = history[i].temp_max;
                mean_value = history[i].temp_mean;
                min_value = history[i].temp_min;

                snprintf(max_read, sizeof(max_read), "%.1f", max_value);
                snprintf(mean_read, sizeof(mean_read), "%.1f", mean_value);
                snprintf(min_read, sizeof(min_read), "%.1f", min_value);

                break;


            case HISTORY_HUM:

                max_value = history[i].hum_max;
                mean_value = history[i].hum_mean;
                min_value = history[i].hum_min;

                snprintf(max_read, sizeof(max_read), "%.0f", max_value);
                snprintf(mean_read, sizeof(mean_read), "%.0f", mean_value);
                snprintf(min_read, sizeof(min_read), "%.0f", min_value);

                break;


            case HISTORY_PRESS:

                max_value = history[i].press_max;
                mean_value = history[i].press_mean;
                min_value = history[i].press_min;

                snprintf(max_read, sizeof(max_read), "%.0f", max_value);
                snprintf(mean_read, sizeof(mean_read), "%.0f", mean_value);
                snprintf(min_read, sizeof(min_read), "%.0f", min_value);

                break;


            default:
                return HAL_ERROR;
        }

        snprintf(date_read,
                 sizeof(date_read),
                 "%02u.%02u",
                 history[i].day,
                 history[i].month);

        draw_rectangle(x, 32, 44, 175, COLOR_WHITE);

        draw_text(x + 4, 38, date_read, COLOR_BLACK, 1);

        draw_text(x + 10, 66, "MAX", COLOR_BLACK, 1);
        draw_text(x + 6, 80, max_read, COLOR_BLACK, 1);

        draw_text(x + 10, 110, "AVG", COLOR_BLACK, 1);
        draw_text(x + 6, 124, mean_read, COLOR_BLACK, 1);

        draw_text(x + 10, 154, "MIN", COLOR_BLACK, 1);
        draw_text(x + 6, 168, min_read, COLOR_BLACK, 1);
    }

    return HAL_OK;
}

HAL_StatusTypeDef UI_DrawHistoryValues(UI_HistoryMetric_t metric)
{
    DailyStats_t history[7];
    uint8_t count;

    HAL_StatusTypeDef status;

    status = STATS_GetHistory(history,&count);

    if(status != HAL_OK)
    {
        return status;
    }

    return UI_DrawHistoryMetric(history,count,metric);
}
