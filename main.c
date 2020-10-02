/**********************************************************
 *                                                        *
 *      Cairo University Shell Eco-Racing Team            *
 *      Shell Eco Marathon 2021 Autonomous System         *
 *      Embedded Autonomous Movement Control Sub-team     *
 *                                                        *
 **********************************************************/

 /******************************************************************************
 *
 * File Name:   main.c
 *
 * Description: main source file to initialize and start the Autonomous System
 *
 * Date:        10/2/2020
 *
 ******************************************************************************/

/* FreeRTOS Includes */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>


/* CCS Default Includes */
#include <stdint.h>
#include <stdbool.h>

/* Tiva-Ware Macros/Defines Includes */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_uart.h"

/* Tiva-Ware Drivers Includes */
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "utils/ustdlib.h"

/* Configurations Includes */
#include "USB_tasks.h"
#include "PID_TASKS.h"
#include "State_Decode.h"
#include "STEPPER_TASKS.h"
#include "UART_TASK.h"
#include "UART.h"

/* Queues handles declarations */
QueueHandle_t Queue_Feedback_Orientation;
QueueHandle_t Queue_Desired_Orientation;
QueueHandle_t Queue_steering;
QueueHandle_t Queue_Current_Orientation;


/******************************************************************************
 *
 * Function Name: tx_app
 *
 * Description: USB Transmit Callback function. The function is triggered when
 *              the USB module transmit data on the transmit buffer
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void tx_app (void)
{
    /* Transmit code here */
}


/******************************************************************************
 *
 * Function Name: rx_app
 *
 * Description: USB Receive Callback function. The function is triggered when
 *              the USB module receives data on the receive buffer.
 *
 * Arguments:   void
 * Return:      void
 *
 *****************************************************************************/
void rx_app (void)
{
   uint8_t read_char;
   USBBufferRead((tUSBBuffer *)&g_sRxBuffer,&read_char,1);
   State_Decoding (read_char) ;
   USBBufferWrite((tUSBBuffer *)&g_sTxBuffer,&read_char,1);

}


int main(void)
{
    /* Enable lazy stacking for interrupt handlers. This allows floating-point
     * instructions to be used within interrupt handlers, but at the expense of
     * extra stack usage. */
    MAP_FPULazyStackingEnable();
    MAP_FPUEnable();

    MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN); /* Set the clocking to run at 50 MHz from the PLL. */
    MAP_IntMasterEnable();                                                                      /* Enable Global Interrupt-bit */

    /* Creating the Queues and storing their addresses in their handles */
    Queue_Feedback_Orientation = xQueueCreate(1,4);
    Queue_Current_Orientation = xQueueCreate(1,4);
    Queue_Desired_Orientation = xQueueCreate(1,4);
    Queue_steering = xQueueCreate(1,4);


    /* Initializing System's modules */
    vInit_Steppers_Tasks();
    vInit_PID();
    vInit_USBTasks (tx_app,rx_app);
    UART1_Init(9600);
    vInit_UART();

        /* Prototype for xTaskCreate:
        *
        *  BaseType_t xTaskCreate( TaskFunction_t pvTaskCode,
        *                          const char * const pcName,
        *                          uint16_t usStackDepth,
        *                          void *pvParameters,
        *                          UBaseType_t uxPriority,
        *                          TaskHandle_t *pvCreatedTask);
        */

    vTaskStartScheduler();  /* Run the Kernel's Scheduler */

    /* UNREACHABLE CODE */
    while(1);

}
