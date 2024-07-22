/*
 * adc.h
 *
 *  Created on: May 2, 2024
 *      Author: jangho
 */

#ifndef COMMON_HW_INCLUDE_ADC_H_
#define COMMON_HW_INCLUDE_ADC_H_

#include "cli.h"
#include "hw_def.h"


#define ADC_MAX_CH          HW_ADC_MAX_CH
#define ADC_RESOLUTION      4096
#define ADC_RESOLUTION_HALF 2048

typedef enum
{
  ADC_CH_VRMS_1,   // ADC1_CH0
  ADC_CH_VRMS_2,   // ADC1_CH1
  ADC_CH_VRMS_3,   // ADC1_CH2
  ADC_CH_VRMS_4,   // ADC1_CH3
  ADC_CH_IRMS_1,   // ADC1_CH11
} AdcCh_t;


bool adcInit(void);
void adcMain(void);
void cliAdc(cli_args_t *args);

#endif /* COMMON_HW_INCLUDE_ADC_H_ */
