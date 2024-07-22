/*
 * dac.c
 *
 *  Created on: May 2, 2024
 *      Author: jangho
 */

#include "dac.h"




extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac2;

static bool is_init = false;


bool dacInit(void)
{
  is_init = true;
  
  HAL_DAC_Start(&hdac1, DAC1_CHANNEL_1);
  HAL_DAC_Start(&hdac2, DAC1_CHANNEL_1);

#ifdef _USE_HW_DAC
  cliAdd("dac", cliDac);
#endif

  return true;
}

void dacMain(void)
{
  
}

#ifdef _USE_HW_DAC
void cliDac(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("dac init : %s\n", is_init ? "OK" : "NG");
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("dac info\n");
  }
}
#endif
