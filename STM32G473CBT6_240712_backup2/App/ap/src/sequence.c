
#include "sequence.h"


#define VRMS_MAX 9.99f
#define IRMS_MAX 5.00f

extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim7;
extern DAC_HandleTypeDef  hdac1;
extern DAC_HandleTypeDef  hdac2;
extern ADC_HandleTypeDef  hadc1;


static bool    is_init                = false;
const uint32_t multi_sampling_num     = 3;          // rms 측정시 파형 획득 횟수
const uint8_t  adc_ch_max             = ADC_MAX_CH; // ch1: AIN0(VRMS1), ch2: AIN1(VRMS2), ch11: IPP/IPN(IRMS)
const uint16_t sequence_freq          = 10000;      // 제어 주기 10Khz
const float    vpp_max                = 20.0f;      // vpp
const float    vpeak_max              = 10.0f;      // vpp * 1/2
float          ipeak_max              = 5.0f;       // A
float          vpp_out                = 0.0f;       // vpp (default 5Vpp)
float          adc_offset[ADC_MAX_CH] = {2355.0f, 2355.0f, 2355.0f, 2355.0f, 2837.0f};

uint32_t adc_value[ADC_MAX_CH];                     // VRMS 1 ~ 4, IPP/IPN(IRMS)
int32_t  dac_out_value = 0;                         // 4096 dac output value
uint16_t freq          = 30;                        // dac freq hz (default 30hz)
uint32_t sample_max    = 100;                       // 파형 생성시 1주기를 만들기 위한 샘플 수, sequence_freq/freq, 50(200hz) ~ 10000(1hz).

float vrms[4] = {0.0f, 0.0f, 0.0f, 0.0f};           // V Vrms1 (AIN0), Vrms2(AIN1)
float irms    = 0.0f;                               // mA Irms (IPP/IPN)

const uint32_t  value_num  = 10;
static uint32_t sample_idx = 0;                     // sine wave 에서 생성할 현재 idx 위치.

value_buf_t queue_buffer[BUFFER_SIZE];
qbuffer_t   value_queue;

uint8_t uart3_send_time = 1;                        // ms serial-plot 로 출력할 uart 데이터를 큐에 업로드 하는 주기


//static bool    is_mode_algo             = false;
//static bool    is_mode_cli              = true;
float          algo_vpp_out             = 0.0f;
uint16_t 	   algo_freq          		= 0;


static void vrms1Upadate(void);
static void vrms2Upadate(void);
static void vrms3Upadate(void);
static void vrms4Upadate(void);
static void irms1Upadate(void);
static void sequenceMotorUpdate(void);
static void sequenceDacUpdate(void);
static void sequenceRmsUpdate(void);
static void sequenceTestPinToggle(void);
static void cliSequence(cli_args_t *args);


bool sequenceInit(void)
{
  is_init = true;

  qbufferCreateBySize(&value_queue, (uint8_t *)queue_buffer, sizeof(value_buf_t), value_num);
  
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  
  cliAdd("seq", cliSequence);  

  sequenceCalibration();

  return true;
}

void sequenceInterruptHandler(void)
{
  sequenceRmsUpdate();
  controlUserFunc();
  sequenceDacUpdate();
  sequenceMotorUpdate();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
  if (htim->Instance == htim1.Instance) // 20Khz(50us)
  {        
    if(htim->Instance->CNT >= (htim->Instance->ARR)/2) // 10Khz(100us)
    {     
      sequenceTestPinToggle();
      sequenceInterruptHandler();
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

void sequenceTestPinToggle(void)
{
  static uint32_t toggle_cnt = 0;

  toggle_cnt++;

  if (toggle_cnt >= 10000)
  {
    toggle_cnt = 0;
    HAL_GPIO_TogglePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin); // PB13
  }
}

void sequenceRmsUpdate(void)
{
  vrms1Upadate();
  vrms2Upadate();
  vrms3Upadate();
  vrms4Upadate();
  irms1Upadate();
}

uint32_t dacAvailableForWrite(void)
{
  uint32_t rx_len;
  uint32_t wr_len;

  rx_len = qbufferAvailable(&value_queue);
  wr_len = (value_queue.len - 1) - rx_len;

  return wr_len;
}

void sequenceDacUpdate(void)
{
  sample_max = sequence_freq / freq;

  // dac_out_value = (uint16_t)rint((sinf(((2 * PI) / sample_max) * sample_idx) + 1.0f) * (float)(ADC_RESOLUTION_HALF));

  float radian       = (2.0f * PI) * ((float)sample_idx / (float)sample_max); // 라디안 각도 계산
  float sin_value    = sinf(radian);                                          // 사인 함수 계산
  float shifted_sin  = sin_value + 1.0f;                                      // 사인 값을 [0, 2] 범위로 이동
  float scaled_value = shifted_sin * (float)(ADC_RESOLUTION_HALF);            // ADC 해상도에 맞게 스케일링
  dac_out_value      = (uint16_t)rint(scaled_value);                          // 반올림 및 정수형으로 변환

  dac_out_value = constrain(dac_out_value, 0, ADC_RESOLUTION - 1);
  dac_out_value = dac_out_value * (vpp_out / vpp_max);
  sample_idx    = next_idx(sample_idx, sample_max);

  HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_out_value); // DAC1 OUT
  HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_out_value); // DAC2 OUT


#if 1 // 일정 주기로 버퍼에 UART로 출력할 DAC 데이터를 쌓는 부분.  
  static uint32_t pre_time = 0;
  if (millis() - pre_time >= uart3_send_time)
  {
    pre_time = millis();

    value_buf_t buf;

    buf.frame_start = 0xBBAA;
    
    buf.value[0] = (float)dac_out_value;
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

void sequenceMotorUpdate(void)
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
    // PH: H => OUT1 H, OUT2: L (Forward)
    HAL_GPIO_WritePin(MT_PH_GPIO_Port, MT_PH_Pin, GPIO_PIN_SET);   
  }
  else
  {
    // PH: L => OUT1 L, OUT2: H (Reverse)
    HAL_GPIO_WritePin(MT_PH_GPIO_Port, MT_PH_Pin, GPIO_PIN_RESET); 
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


void vrms1Upadate(void)
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

void vrms2Upadate(void)
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

void vrms3Upadate(void)
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

void vrms4Upadate(void)
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

void irms1Upadate(void)
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

// remove adc offset value ( vrms1~4, Irms1 )
void sequenceCalibration(void)
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

void cliSequence(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    cliPrintf("c init : %s\n", is_init ? "OK" : "NG");
    
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

    freq = constrain(freq_tmp, 1, 200);
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

  // serial-plot 로 출력할 uart 데이터를 큐에 업로드 하는 주기 변경
  if (args->argc == 2 && args->isStr(0, "time") == true)
  {
    uint8_t old_time = uart3_send_time;
    uint8_t new_time     = 0;
    new_time             = args->getData(1);

    if(new_time != old_time)
    {
      uart3_send_time = constrain(new_time, 0, 255);
      cliPrintf("uart3_tx_time : %d -> %d\n", old_time, uart3_send_time);
    }

    ret = true;
  }

  if (ret != true)
  {
    cliPrintf("seq info\n");
    cliPrintf("seq cali\n");
    cliPrintf("seq raw\n");
    cliPrintf("seq time [0-255]\n");    
    cliPrintf("seq wave freq[1-200] vpp[1.0-20.0]\n");
  }
}
