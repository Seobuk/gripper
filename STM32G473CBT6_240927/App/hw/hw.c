/*
 * hw.c
 *
 *  Created on: May 2, 2024
 *      Author: jangho
 */


#include "hw.h"

void hwInit(void)
{
  bspInit();

  cliInit();
  logInit();
  uartInit();
  i2cInit();
  lcdInit();
  dacInit();
  adcInit();

  for (int i=0; i<HW_UART_MAX_CH; i++)
  {
    uartOpen(i, 115200);
  }
  uartOpen(_DEF_CH2, 256000);

  logOpen(HW_LOG_CH, 115200);
  logDebug("\r\n[ Firmware Begin... ]\r\n");
  logDebug("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
  logDebug("Booting..Ver  \t\t: %s\r\n", _DEF_FIRMWATRE_VERSION);
  logDebug("Booting..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq() / 1000000);
  logDebug("\n");
}
