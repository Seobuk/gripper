#include "control.h"




static bool is_init = false;


static void cliCtrl(cli_args_t *args);

bool controlInit(void)
{
  is_init = true;

  cliAdd("ctrl", cliCtrl);

  return true;
}

void controlUserFunc(void)
{
  // controlUserFunc
}

void controlUserMain(void)
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
