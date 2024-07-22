/*
 * dac.h
 *
 *  Created on: May 2, 2024
 *      Author: jangho
 */

#ifndef COMMON_HW_INCLUDE_DAC_H_
#define COMMON_HW_INCLUDE_DAC_H_


#include "hw_def.h"
#include "cli.h"

bool dacInit(void);
void dacMain(void);
void cliDac(cli_args_t *args);

#endif /* COMMON_HW_INCLUDE_DAC_H_ */
