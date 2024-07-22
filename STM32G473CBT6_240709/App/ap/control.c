/*
 * control.c
 *
 *  Created on: May 10, 2024
 *      Author: user
 */


#include "control.h"
#include "hw_def.h"
#include "cli.h"
#include "adc.h"
#include "dac.h"
#include "lcd.h"
#include "uart.h"
#include "qbuffer.h"


#define VRMS_MAX  9.99f
#define IRMS_MAX  5.00f


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim7;
extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac2;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_tx;


static bool    is_init                = false;
const uint32_t multi_sampling_num     = 3;          // rms 측정시 파형 획득 횟수
const uint32_t lcd_update_period      = 500;        // ms
const uint8_t  adc_ch_max             = ADC_MAX_CH; // ch1: AIN0(VRMS1), ch2: AIN1(VRMS2), ch11: IPP/IPN(IRMS)
const uint16_t control_freq           = 10000;      // 제어 주기 10Khz
const float    vpp_max                = 20.0f;      // vpp
const float    vpeak_max              = 10.0f;      // vpp * 1/2
float          ipeak_max              = 5.0f;       // [A]
float          vpp_out                = 0.0f;       // vpp (default 5Vpp)
float          adc_offset[ADC_MAX_CH] = {2355.0f, 2355.0f, 2355.0f, 2355.0f, 2837.0f};

uint32_t adc_value[ADC_MAX_CH];                     // VRMS 1 ~ 4, IPP/IPN(IRMS)
int32_t  dac_out_value = 0;                         // 4096 dac output value
uint16_t freq          = 30;                        // dac freq hz (default 30hz)
uint32_t sample_max    = 100;                       // 파형 생성시 1주기를 만들기 위한 샘플 수, control_freq/freq, 50(200hz) ~ 10000(1hz).

float vrms[4] = {0.0f, 0.0f, 0.0f, 0.0f};           // [V] Vrms1 (AIN0), Vrms2(AIN1)
float irms    = 0.0f;                               // [mA] Irms (IPP/IPN)

const uint32_t value_num = 10;

typedef struct
{
  uint16_t frame_start;
  float value[4];
} __attribute__((__packed__)) value_buf_t;

value_buf_t queue_buffer[10];
qbuffer_t value_queue;




void controlUartUpdate(void);
void adcVrms1UpadateProc(void);
void adcVrms2UpadateProc(void);
void adcVrms3UpadateProc(void);
void adcVrms4UpadateProc(void);
void adcIrms1UpadateProc(void);


bool controlInit(void)
{
  is_init = true;

  qbufferCreateBySize(&value_queue, (uint8_t *)queue_buffer, sizeof(value_buf_t), value_num);
  
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  
  cliAdd("ctrl", cliControl);  

  return true;
}

void lcdUpdate(void)
{
  static uint32_t pre_time = 0;

  if(millis() - pre_time >= lcd_update_period)
  {
    pre_time = millis();

    lcdClearBuffer(black);

    if(lcdGetFont() == LCD_FONT_HAN)
    {
      lcdPrintf(0, 16 * 0, white, "%3dHz %2.1fVpp", freq, vpp_out);
      lcdPrintf(0, 16 * 1, white, "Vrms %1.2f %1.2f", vrms[ADC_CH_VRMS_1], vrms[ADC_CH_VRMS_2]);
      lcdPrintf(40, 16 * 2, white, "%1.2f %1.2f", vrms[ADC_CH_VRMS_3], vrms[ADC_CH_VRMS_4]);
      lcdPrintf(0, 16 * 3, white, "Irms %1.2f", irms);
    }
    else
    {
      lcdPrintf(28, 16*0, white, "%3dHz %2.1fVpp", freq, vpp_out);
      lcdPrintf(0, 16 * 1, white, "V [1]%1.2f [2]%1.2f", vrms[ADC_CH_VRMS_1], vrms[ADC_CH_VRMS_2]);
      lcdPrintf(14, 16 * 2, white, "[3]%1.2f [4]%1.2f", vrms[ADC_CH_VRMS_3], vrms[ADC_CH_VRMS_4]);
      lcdPrintf(0, 16 * 3, white, "I [1]%1.2f", irms);
    }      

    lcdRequestDraw();
  }
}

uint32_t dacAvailableForWrite(void)
{
  uint32_t rx_len;
  uint32_t wr_len;

  rx_len = qbufferAvailable(&value_queue);
  wr_len = (value_queue.len - 1) - rx_len;

  return wr_len;
}

void controlMain(void)
{
  controlUartUpdate();
}

void controlUserFunc(void)
{
  // controlUserFunc
}

void controlAdcUpdate(void)
{
  adcVrms1UpadateProc();
  adcVrms2UpadateProc();
  adcVrms3UpadateProc();
  adcVrms4UpadateProc();
  adcIrms1UpadateProc();
}

void controlDacUpdate(void)
{
  static uint32_t sample_idx = 0; // sine wave 에서 생성할 현재 idx 위치.
  sample_max = control_freq / freq;


  dac_out_value = (uint16_t)rint((sinf(((2 * PI) / sample_max) * sample_idx) + 1.0f) * (float)(ADC_RESOLUTION_HALF));

  dac_out_value = constrain(dac_out_value, 0, ADC_RESOLUTION-1);
  dac_out_value = dac_out_value * (vpp_out / vpp_max);
  sample_idx    = next_idx(sample_idx, sample_max);

  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_out_value); // DAC1 OUT
  HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_out_value); // DAC2 OUT


#if 1
  // 일정 주기로 버퍼에 UART로 출력할 DAC 데이터를 쌓는 부분.
  static uint32_t pre_time = 0;
  if (millis() - pre_time >= 1)
  {
    pre_time = millis();

    value_buf_t buf;

    buf.frame_start = 0xBBAA;

    buf.value[0] = dac_out_value;
    buf.value[1] = 34.5f;
    buf.value[2] = 56.7f;
    buf.value[3] = 78.9f;

    if (dacAvailableForWrite() > 0)
    {
      qbufferWrite(&value_queue, (uint8_t *)&buf, 1);
    }
  }
#endif
}

void controlMotorUpdate(void)
{
  float    arr       = (float)(__HAL_TIM_GET_AUTORELOAD(&htim1));
  float    pwm_ratio = 0.0f;
  uint32_t pwm_out   = 0;
  int32_t  dac_value = dac_out_value;
  float    vpp_ratio = (vpp_out / vpp_max);
  int32_t dac_offset = 0;


  dac_offset = (int32_t)(vpp_ratio * (float)ADC_RESOLUTION_HALF);

  if (dac_value >= dac_offset)
  {
    HAL_GPIO_WritePin(MT_PH_GPIO_Port, MT_PH_Pin, GPIO_PIN_SET);   // PH: H => OUT1 H, OUT2: L (Forward)
  }
  else
  {
    HAL_GPIO_WritePin(MT_PH_GPIO_Port, MT_PH_Pin, GPIO_PIN_RESET); // PH: L => OUT1 L, OUT2: H (Reverse)
  }

  dac_value -= dac_offset;

  if(dac_value >= 0)
  {
    dac_value++;
  }
  else
  {
    dac_value *= -1;
  }

  dac_value = constrain(dac_value, 1, ADC_RESOLUTION_HALF);

  pwm_ratio =  (float)dac_value / (float)ADC_RESOLUTION_HALF;

  pwm_out   = (uint32_t)(pwm_ratio * arr);

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_out);
}

void controlUartUpdate(void)
{
  value_buf_t buf;

  if (qbufferAvailable(&value_queue) > 0)
  {
    qbufferRead(&value_queue, (uint8_t *)&buf, 1);

    HAL_UART_Transmit(&huart3, (uint8_t *)&buf, sizeof(value_buf_t), 10);
  }
}

void controlAdcCalibration(void)
{
  uint32_t pre_time = 0;
  uint32_t adc_time = 0;
  uint32_t cnt      = 0;

  uint32_t sum[ADC_MAX_CH] = { 0, 0, 0, 0, 0 };
  uint32_t avg[ADC_MAX_CH] = { 0, 0, 0, 0, 0 };


  pre_time = millis();

  while (millis() - pre_time < 1000)
  {
    if (millis() - adc_time >= 1)
    {
      adc_time = millis();

      if (cnt < 500)
      {
        for (uint8_t ch = 0; ch < adc_ch_max; ch++)
        {
          sum[ch] += adc_value[ch];
        }
        cnt++;
      }
      else
      {
        for (uint8_t ch = 0; ch < adc_ch_max; ch++)
        {
          avg[ch]        = sum[ch] / (cnt + 1);
          adc_offset[ch] = avg[ch];
        }
        break;
      }
    }
  }
}

void controlInterruptHandler(void)
{
  controlAdcUpdate();
  controlUserFunc();
  controlDacUpdate();
  controlMotorUpdate();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim1.Instance) // 20Khz(50us)
  {        
    if(htim->Instance->CNT >= (htim->Instance->ARR)/2) // 10Khz(100us)
    {
      controlInterruptHandler();
    }  
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == hadc1.Instance) //  10Khz(100us)
  {    
    static uint8_t i = 0;

    adc_value[i] = HAL_ADC_GetValue(&hadc1);

    i = next_idx(i, adc_ch_max);    
  }
}

void adcVrms1UpadateProc(void)
{
  const uint8_t ch = ADC_CH_VRMS_1;

  float adc_max    = 4090.0f;
  float adc_min    = 0.0f;
  float adc_range  = 0.0f;
  float value      = 0.0f;

  static float    sum = 0.0f;
  static float    avg = 0.0f;
  static uint32_t cnt = 0;
  uint32_t sample_cnt = 0;

  sample_cnt = sample_max * multi_sampling_num;
  adc_range  = adc_max - adc_offset[ch];
  adc_min    = adc_offset[ch] - adc_range;

  value = adc_value[ch];
  value = constrain(value, adc_min, adc_max);
  value = abs(value - adc_offset[ch]);
  value /= adc_range;
  sum += value;

  cnt++;
  if(cnt >= sample_cnt)
  {
    avg = sum / cnt;
    vrms[ch] = avg * vpeak_max;
    vrms[ch] = constrain(vrms[ch], 0, VRMS_MAX);

    sum = 0.0f;
    cnt = 0;
  }
}

void adcVrms2UpadateProc(void)
{
  const uint8_t ch = ADC_CH_VRMS_2;

  float adc_max    = 4090.0f;
  float adc_min    = 0.0f;
  float adc_range  = 0.0f;
  float value      = 0.0f;  

  static float    sum = 0.0f;
  static float    avg = 0.0f;
  static uint32_t cnt = 0;
  uint32_t sample_cnt = 0;

  sample_cnt = sample_max * multi_sampling_num;
  adc_range  = adc_max - adc_offset[ch];
  adc_min    = adc_offset[ch] - adc_range;

  value = adc_value[ch];
  value = constrain(value, adc_min, adc_max);
  value = abs(value - adc_offset[ch]);
  value /= adc_range;
  sum += value;

  cnt++;
  if(cnt >= sample_cnt)
  {
    avg = sum / cnt;
    vrms[ch] = avg * vpeak_max;
    vrms[ch] = constrain(vrms[ch], 0, VRMS_MAX);

    sum = 0.0f;
    cnt = 0;
  }
}

void adcVrms3UpadateProc(void)
{
  const uint8_t ch = ADC_CH_VRMS_3;

  float adc_max    = 4090.0f;
  float adc_min    = 0.0f;
  float adc_range  = 0.0f;
  float value      = 0.0f;

  static float    sum = 0.0f;
  static float    avg = 0.0f;
  static uint32_t cnt = 0;
  uint32_t sample_cnt = 0;

  sample_cnt = sample_max * multi_sampling_num;
  adc_range  = adc_max - adc_offset[ch];
  adc_min    = adc_offset[ch] - adc_range;

  value = adc_value[ch];
  value = constrain(value, adc_min, adc_max);
  value = abs(value - adc_offset[ch]);
  value /= adc_range;
  sum += value;

  cnt++;
  if(cnt >= sample_cnt)
  {
    avg = sum / cnt;
    vrms[ch] = avg * vpeak_max;
    vrms[ch] = constrain(vrms[ch], 0, VRMS_MAX);

    sum = 0.0f;
    cnt = 0;
  }
}

void adcVrms4UpadateProc(void)
{
  const uint8_t ch = ADC_CH_VRMS_4;

  float adc_max    = 4090.0f;
  float adc_min    = 0.0f;
  float adc_range  = 0.0f;
  float value      = 0.0f;

  static float    sum = 0.0f;
  static float    avg = 0.0f;
  static uint32_t cnt = 0;
  uint32_t sample_cnt = 0;

  sample_cnt = sample_max * multi_sampling_num;
  adc_range  = adc_max - adc_offset[ch];
  adc_min    = adc_offset[ch] - adc_range;

  value = adc_value[ch];
  value = constrain(value, adc_min, adc_max);
  value = abs(value - adc_offset[ch]);
  value /= adc_range;
  sum += value;

  cnt++;
  if(cnt >= sample_cnt)
  {
    avg = sum / cnt;
    vrms[ch] = avg * vpeak_max;
    vrms[ch] = constrain(vrms[ch], 0, VRMS_MAX);

    sum = 0.0f;
    cnt = 0;
  }
}

void adcIrms1UpadateProc(void)
{
  const uint8_t ch = ADC_CH_IRMS_1;

  float adc_max    = 3797.0f;
  float adc_min    = 0.0f;
  float adc_range  = 0.0f;
  float value      = 0.0f;  

  static float    sum = 0.0f;
  static float    avg = 0.0f;
  static uint32_t cnt = 0;
  uint32_t sample_cnt = 0;

  sample_cnt = sample_max * multi_sampling_num;
  adc_range  = adc_max - adc_offset[ch];
  adc_min    = adc_offset[ch] - adc_range;

  value = adc_value[ch];
  value = constrain(value, adc_min, adc_max);
  value = abs(value - adc_offset[ch]);
  value /= adc_range;
  sum += value;

  cnt++;
  if(cnt >= sample_cnt)
  {
    avg = sum / cnt;
    irms = avg * ipeak_max;
    irms = constrain(irms, 0, IRMS_MAX);

    sum = 0.0f;
    cnt = 0;
  }
}

void cliControl(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("ctrl init : %s\n", is_init ? "OK" : "NG");
    
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "wave") == true)
  {
    uint16_t freq_tmp, prev_freq;
    float    vpp_tmp, prev_vpp;

    freq_tmp = args->getData(1);
    vpp_tmp  = args->getFloat(2);

    prev_freq = freq;
    prev_vpp = vpp_out;

    freq    = constrain(freq_tmp, 1, 200);
    vpp_out = constrain(vpp_tmp, 0.0f, 20.0f);

    cliPrintf("wave update.\nfreq: %3d -> %3d hz\nvpp : %2.1f -> %2.1f vpp\n", prev_freq, freq, prev_vpp, vpp_out);

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "raw") == true)
  {
    uint32_t pre_time = 0;

    while (cliKeepLoop())
    {
      if (millis() - pre_time >= 100)
      {
        pre_time = millis();

        cliPrintf("adc raw : %d %d %d %d %d\n",
                  adc_value[ADC_CH_VRMS_1],
                  adc_value[ADC_CH_VRMS_2],
                  adc_value[ADC_CH_VRMS_3],
                  adc_value[ADC_CH_VRMS_4],
                  adc_value[ADC_CH_IRMS_1]);
      }
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "cali") == true) // Calibration
  {
    uint16_t percent  = 0;
    uint32_t pre_time = 0;
    uint32_t adc_time = 0;
    uint32_t sum[ADC_MAX_CH]   = {0,};
    uint32_t avg[ADC_MAX_CH]   = {0,};
    uint32_t old[ADC_MAX_CH]   = {0,};
    uint32_t cnt      = 0;

    bool is_cali_done = false;

    volatile uint32_t start_time = 0;
    volatile uint32_t diff_time = 0;

    cliPrintf("Offset Calibration start...\n");
    cliPrintf("Disconnect the ADC terminal.\n");

    for (uint8_t ch = 0; ch < adc_ch_max; ch++)
    {
      old[ch] = adc_offset[ch];
    }

    start_time = millis();

    while (cliKeepLoop())
    {
      if (millis() - adc_time >= 1)
      {
        adc_time = millis();

        if (cnt < 1000)
        {
          for (uint8_t ch = 0; ch < adc_ch_max; ch++)
          {
            sum[ch] += adc_value[ch];
          }
          cnt++;
        }
        else if(is_cali_done == false)
        {
          diff_time = millis() - start_time;
          cliPrintf("Calculated Time: %dms\n", diff_time);
          for (uint8_t ch = 0; ch < adc_ch_max; ch++)
          {
            avg[ch]        = sum[ch] / (cnt + 1);
            adc_offset[ch] = avg[ch];
          }

          is_cali_done = true;
        }
      }

      if (millis() - pre_time > 150)
      {
        pre_time = millis();
        if (lcdDrawAvailable())
        {
          lcdClearBuffer(black);
          lcdPrintf(96, 0, white, "%3d%%", percent);
          lcdDrawRect(0, 16, 128, 16, white);
          lcdDrawFillRect(2, 19, percent * 124 / 100, 10, white);
          lcdRequestDraw();
          percent += 10;
        }
      }

      if (is_cali_done && (percent > 100))
      {        
        cliPrintf("sample_cnt: %d\n", cnt);
        cliPrintf("ch[0]: %d -> %d\n", old[0], avg[0]);
        cliPrintf("ch[1]: %d -> %d\n", old[1], avg[1]);
        cliPrintf("ch[2]: %d -> %d\n", old[2], avg[2]);
        cliPrintf("ch[3]: %d -> %d\n", old[3], avg[3]);
        cliPrintf("ch[4]: %d -> %d\n", old[4], avg[4]);

        cliPrintf("Offset Calibration Done!!\n");
        delay(1000);
        break;
      }
    }

    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("ctrl info\n");
    cliPrintf("ctrl cali\n");
    cliPrintf("ctrl raw\n");
    cliPrintf("ctrl wave freq[1-200] vpp[1.0-20.0]\n");
  }
}
