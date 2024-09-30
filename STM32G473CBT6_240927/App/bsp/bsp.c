/*
 * bsp.c
 *
 *  Created on: May 2, 2024
 *      Author: user
 */


#include "bsp.h"
#include "hw_def.h"


bool bspInit(void)
{
  return true;
}

void delay(uint32_t ms)
{
  HAL_Delay(ms);
}

uint32_t millis(void)
{
  return HAL_GetTick();
}


