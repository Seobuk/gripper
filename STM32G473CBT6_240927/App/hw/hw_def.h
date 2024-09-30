#ifndef HW_HW_DEF_H_
#define HW_HW_DEF_H_

#include "bsp.h"


#define _DEF_FIRMWATRE_VERSION    "V240618R3"
#define _DEF_BOARD_NAME           "EXCITER-CTRL-FW"


#define _USE_HW_GPIO
#define _USE_HW_DAC

#define _USE_HW_UART
#define      HW_UART_MAX_CH         2

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    32
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_ADC
#define      HW_ADC_MAX_CH          5

#define _USE_HW_HWTIMER
#define      HW_HWTIMER_MAX_CH      2

#define _USE_HW_SWTIMER
#define      HW_SWTIMER_MAX_CH      8
#define      HW_SWTIMER_PERIOD_MS   1

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       3

#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1
#define      HW_I2C_CH_OLED         _DEF_I2C1

#define _USE_HW_LCD
#define _USE_HW_SSD1306
#define      HW_LCD_WIDTH           128
#define      HW_LCD_HEIGHT          64


//-- BUTTON
//
#define _BTN_LEFT                   0
#define _BTN_RIGHT                  1
#define _BTN_ENTER                  2


//-- CLI
//
#define _USE_CLI_HW_UART            1
#define _USE_CLI_HW_LOG             1
#define _USE_CLI_HW_GPIO            1
#define _USE_CLI_HW_BUTTON          1
#define _USE_CLI_HW_ADC             1


#endif /* HW_HW_DEF_H_ */
