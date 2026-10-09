/*
 * UI.h
 *
 *  Created on: 08.10.2026
 *      Author: marko
 */

#ifndef INC_UI_H_
#define INC_UI_H_

typedef enum{
	CURRENT_VIEW,
	DAILY_VIEW,
	HISTORY_VIEW
} UI_View_t;


HAL_StatusTypeDef UI_DrawCurrent(void);

#endif /* INC_UI_H_ */
