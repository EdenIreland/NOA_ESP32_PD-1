/*
  NOA_public.h - NOA public library head file
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef __NOA_PUBLIC_H
#define __NOA_PUBLIC_H

#define ESP32DEBUGGING
#include <ESP32Logger.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIZE_OF_TASK_QUEUE  16
#define SIZE_OF_STACK       2048

#define ARDUINO_RUNNING_CORE xPortGetCoreID()

// Task Queue Mesage struct
extern xQueueHandle NOA_APP_TASKQUEUE;
extern xQueueHandle NOA_RGB_TASKQUEUE;
extern xQueueHandle NOA_NET_TASKQUEUE;

typedef struct {
  uint32_t  message;
  uint32_t  param1;
  uint32_t  param2;
  uint32_t  srcTaskId;
} NOA_PUB_MSG;

void NOA_PUB_ESP32DebugInit(void);
void NOA_PUB_Print_Buf_Hex(uint8_t *buf, uint16_t len);

void NOA_PUB_I2C_Scanner(uint8_t nIndex);
void NOA_PUB_I2C_ReceiveBytes(uint8_t nIndex, uint8_t PD_ADDR, uint8_t addr, uint8_t *data, uint16_t length);
void NOA_PUB_I2C_SendBytes(uint8_t nIndex, uint8_t PD_ADDR, uint8_t addr, uint8_t *data, uint16_t length);

void NOA_PUB_I2C_PD_RreadAllRegs(uint8_t nIndex, uint8_t PD_ADDR);
void NOA_PUB_I2C_PM_RreadAllRegs(uint8_t nIndex, uint8_t PD_ADDR);
void NOA_PUB_I2C_PD_Testing(uint8_t nIndex, uint8_t PD_ADDR);

#ifdef __cplusplus
}
#endif

#endif
