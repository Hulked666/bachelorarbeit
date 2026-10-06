/*
 * display.c
 *
 *  Created on: 02.10.2026
 *      Author: marko
 */

#ifndef INC_DISPLAY_H
#define INC_DISPLAY_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

/* Displaygröße */
#define DISPLAY_WIDTH   320U
#define DISPLAY_HEIGHT  240U

/* RGB565 Farben */
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_YELLOW    0xFFE0
#define COLOR_CYAN      0x07FF

/* Öffentliche Displayfunktionen */
HAL_StatusTypeDef display_init(void);

HAL_StatusTypeDef draw_pixel(uint16_t x,
                             uint16_t y,
                             uint16_t color);

HAL_StatusTypeDef fill_screen(uint16_t color);

HAL_StatusTypeDef draw_rectangle(uint16_t x,
                                 uint16_t y,
                                 uint16_t width,
                                 uint16_t height,
                                 uint16_t color);

HAL_StatusTypeDef draw_char(uint16_t x,
                            uint16_t y,
                            char value,
                            uint16_t color,
							uint8_t scale);

HAL_StatusTypeDef draw_text(uint16_t x,
                            uint16_t y,
                            const char *text ,
                            uint16_t color,
							uint8_t scale);

#endif /* INC_DISPLAY_C_ */
