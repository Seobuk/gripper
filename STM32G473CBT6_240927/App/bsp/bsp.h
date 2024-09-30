/*
 * bsp.h
 *
 *  Created on: May 2, 2024
 *      Author: user
 */

#ifndef BSP_H_
#define BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"

#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"




bool bspInit(void);

void logPrintf(const char *fmt, ...);
void delay(uint32_t time_ms);
uint32_t millis(void);
uint32_t micros(void);


#ifdef __cplusplus
}
#endif

#endif

