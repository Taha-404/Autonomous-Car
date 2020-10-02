/*
 * State_Decode.h
 *
 *  Created on: ???/???/????
 *      Author: Tefa
 */

#ifndef STATE_DECODE_H_
#define STATE_DECODE_H_


#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include <stdint.h>


#define Numbers_Ascii_Base 0x30
#define Numbers_Ascii_Offset 9


extern QueueHandle_t Queue_steering;
extern QueueHandle_t Queue_Feedback_Orientation;
extern QueueHandle_t Queue_Desired_Orientation;
extern QueueHandle_t Queue_steering;


extern void State_Decoding (char USB_Received_Char);

#endif /* STATE_DECODE_H_ */