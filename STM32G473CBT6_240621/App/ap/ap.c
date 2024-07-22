/*
 * ap.c
 *
 *  Created on: May 2, 2024
 *      Author: jangho
 */


#include "ap.h"

static uint32_t pre_time = 0;

void apInit(void)
{
  cliOpen(_DEF_UART1, 115200);
  
  controlInit();

  lcdSetFont(LCD_FONT_HAN);    

  for (int i = 0; i < 40; i += 1)
  {
    lcdClearBuffer(black);
    lcdPrintf(0, 64 - i, white, " 한국기계연구원");
    lcdDrawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);
    lcdUpdateDraw();
    delay(5);
  }
  delay(500);  
  lcdClear(black);    

  lcdSetFont(LCD_FONT_07x10);  
}

void apMain(void)
{
  while (1)
  {
    cliMain();
    lcdUpdate();
    controlMain();

    if (millis() - pre_time > 500)
    {
      pre_time = millis();
      HAL_GPIO_TogglePin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin); // ON BOARD LED toggle 500ms
    }
  }
}
