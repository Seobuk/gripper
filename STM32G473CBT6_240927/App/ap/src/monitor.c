
#include "monitor.h"
#include "sequence.h"


extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef  hdma_usart3_tx;
extern uint16_t freq;
extern float    vpp_out;
extern float    vrms[4];
extern float    irms;
extern qbuffer_t value_queue;

static bool    is_init           = false;
const uint32_t lcd_update_period = 500; // ms
const uint32_t cli_update_period = 500; // ms


static void cliMonitor(cli_args_t *args);

bool monitorInit(void)
{
  is_init = true;

  cliAdd("mon", cliMonitor);  

  #if 0
  lcdSetFont(LCD_FONT_HAN);

  for (int i = 0; i < 40; i += 1)
  {
    lcdClearBuffer(black);
    lcdPrintf(0, 64 - i, white, " 한국기계연구원");
    lcdDrawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);
    lcdUpdateDraw();
    delay(5);
  }
  delay(500);
  lcdClear(black);
  #endif

  lcdSetFont(LCD_FONT_07x10);

  return true;
}

void monitorLcdUpdate(void)
{
  static uint32_t pre_time = 0;

  if(millis() - pre_time >= lcd_update_period)
  {
    pre_time = millis();

    lcdClearBuffer(black);

    lcdPrintf(28, 16 * 0, white, "%3dHz %2.1fVpp", freq, vpp_out);
    lcdPrintf(0, 16 * 1, white, "V [1]%1.2f [2]%1.2f", vrms[ADC_CH_VRMS_1], vrms[ADC_CH_VRMS_2]);
    lcdPrintf(14, 16 * 2, white, "[3]%1.2f [4]%1.2f", vrms[ADC_CH_VRMS_3], vrms[ADC_CH_VRMS_4]);
    lcdPrintf(0, 16 * 3, white, "I [1]%1.2f", irms);

    lcdRequestDraw();
  }
}

// void monitorCliUpdate(void)
// {
//   static uint32_t pre_time = 0;

//   if(millis() - pre_time >= cli_update_period)
//   {
//     pre_time = millis();

//     lcdClearBuffer(black);

//     lcdPrintf(28, 16 * 0, white, "%3dHz %2.1fVpp", freq, vpp_out);
//     lcdPrintf(0, 16 * 1, white, "V [1]%1.2f [2]%1.2f", vrms[ADC_CH_VRMS_1], vrms[ADC_CH_VRMS_2]);
//     lcdPrintf(14, 16 * 2, white, "[3]%1.2f [4]%1.2f", vrms[ADC_CH_VRMS_3], vrms[ADC_CH_VRMS_4]);
//     lcdPrintf(0, 16 * 3, white, "I [1]%1.2f", irms);

//     lcdRequestDraw();
//   }
// }

void monitorUartUpdate(void)
{
  value_buf_t buf;
  if (qbufferAvailable(&value_queue) > 0)
  {
    qbufferRead(&value_queue, (uint8_t *)&buf, 1);
    HAL_UART_Transmit(&huart3, (uint8_t *)&buf, sizeof(value_buf_t), 10);
  }
}

void monitorMain(void)
{  
  monitorUartUpdate();
}

void cliMonitor(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("mon init : %s\n", is_init ? "OK" : "NG");

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "view"))
  {
    uint32_t pre_time = 0;

    cliShowCursor(false);
    while (cliKeepLoop())
    {
      monitorUartUpdate();
      if (millis() - pre_time >= cli_update_period)
      {
        pre_time = millis();

        cliPrintf("Freq  : %dHz\n", freq);
        cliPrintf("Volt  : %2.1fVpp\n", vpp_out);
        cliPrintf("Vrms1 : %1.2fV\n", vrms[ADC_CH_VRMS_1]);
        cliPrintf("Vrms2 : %1.2fV\n", vrms[ADC_CH_VRMS_2]);
        cliPrintf("Vrms3 : %1.2fV\n", vrms[ADC_CH_VRMS_3]);
        cliPrintf("Vrms4 : %1.2fV\n", vrms[ADC_CH_VRMS_4]);        
        cliPrintf("Irms1 : %1.2fA\n", irms);        
        cliMoveUp(7);        
      }
    }
    cliMoveDown(7);
    cliShowCursor(true);
    
    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("mon info\n");
    cliPrintf("mon view\n");
  }
}
