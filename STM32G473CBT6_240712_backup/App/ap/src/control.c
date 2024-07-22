#include "control.h"




static bool is_init = false;


static void cliCtrl(cli_args_t *args);

bool controlInit(void)
{
  is_init = true;

  cliAdd("ctrl", cliCtrl);

  return true;
}

void controlUserFunc(void) //  10Khz(100us)
{
  // controlUserFunc


  static uint32_t toggle_cnt = 0;

  toggle_cnt++;

  if (toggle_cnt >= 10000)
  {
	toggle_cnt = 0;
	HAL_GPIO_TogglePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin); // PB13
  }
}

void controlUserMain(void)  // apmain : 비주기성
{
  // controlUserMain
}

void cliCtrl(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("ctrl init : %s\n", is_init ? "OK" : "NG");

    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("ctrl info\n");
  }
}
