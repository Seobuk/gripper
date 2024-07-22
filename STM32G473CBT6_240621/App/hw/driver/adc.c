/*
 * adc.c
 *
 *  Created on: May 2, 2024
 *      Author: jangho
 */

#include "adc.h"




extern TIM_HandleTypeDef htim8;
extern ADC_HandleTypeDef hadc1;


static bool is_init = false;

bool adcInit(void)
{
  is_init = true;

  HAL_TIM_Base_Start(&htim8); //ADC TIM8
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADC_Start_IT(&hadc1);

#ifdef _USE_HW_ADC
 cliAdd("adc", cliAdc);
#endif

  return true;
}

void adcMain(void)
{

}

#ifdef _USE_HW_ADC
void cliAdc(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("adc init : %s\n", is_init ? "OK" : "NG");
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("adc info\n");
  }
}
#endif
