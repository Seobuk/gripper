/*
 * control.h
 *
 *  Created on: May 10, 2024
 *      Author: user
 */

#ifndef AP_CONTROL_H_
#define AP_CONTROL_H_

#include "hw_def.h"
#include "cli.h"

bool controlInit(void);
void controlUserFunc(void);
void lcdUpdate(void);
void cliControl(cli_args_t *args);
void controlMain(void);
void controlAdcCalibration(void);

#endif /* AP_CONTROL_H_ */
