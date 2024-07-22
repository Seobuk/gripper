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
  
  monitorInit();
  sequenceInit();
  controlInit();
}

void apMain(void)
{
  while (1)
  {
    cliMain();
    monitorMain();
    controlUserMain();

     if (millis() - pre_time > 500)
     {
       pre_time = millis();
       HAL_GPIO_TogglePin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin); // ON BOARD LED toggle 500ms
     }
  }
}
