#ifndef DEF_H_
#define DEF_H_



#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arm_math.h"

#include "err_code.h"

#define _CFG_DEBUG_LOG      1    // 1: On, 0: Off   "setup.h"
#define _CFG_LCD_DIRECT     1    // 1: FOWARD, 0: REVERSE

#if _CFG_DEBUG_LOG
#define logDebug logPrintf
#else
#define logDebug
#endif

#define _DEF_CH1              0
#define _DEF_CH2              1
#define _DEF_CH3              2
#define _DEF_CH4              3

#define _DEF_LED1             0
#define _DEF_LED2             1
#define _DEF_LED3             2

#define _DEF_UART1            0
#define _DEF_UART2            1

#define _DEF_BUTTON1          0
#define _DEF_BUTTON2          1
#define _DEF_BUTTON3          2

#define _DEF_I2C1             0

#define _DEF_LOW              0
#define _DEF_HIGH             1

#define _DEF_INPUT            0
#define _DEF_INPUT_PULLUP     1
#define _DEF_INPUT_PULLDOWN   2
#define _DEF_OUTPUT           3
#define _DEF_OUTPUT_PULLUP    4
#define _DEF_OUTPUT_PULLDOWN  5

#define square(x) ((x) * (x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define next_idx(curr, max) (curr + 1 + max) % max

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef cmax
#define cmax(a,b) (((a) > (b)) ? (a) : (b))
#define cmin(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef map
#define map(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#ifndef cmap
#define cmap(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#endif
