#ifndef AP_SEQUENCE_H_
#define AP_SEQUENCE_H_

#include "hw.h"
#include "control.h"

#define BUFFER_SIZE   200

typedef struct
{
  uint16_t frame_start;
  float    value[4];
} __attribute__((__packed__)) value_buf_t;

bool sequenceInit(void);
void sequenceCalibration(void);

#endif
