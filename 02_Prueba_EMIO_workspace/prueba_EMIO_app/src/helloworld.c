/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpiops.h"
#include "xparameters.h"
#include "sleep.h"

#define GPIOPS_ID XPAR_XGPIOPS_0_BASEADDR
#define EMIO_LED0 54 // El último FIXED-IO es el 53, de forma que el EMIO0 es el 54
#define EMIO_LED1 55 // El último FIXED-IO es el 53, de forma que el EMIO1 es el 55

#define MS_TO_US 1000U

int main()
{
    init_platform();

    XGpioPs gpiops_inst;
    XGpioPs_Config *gpiops_cfg_ptr;

    // Inicialización de XGpio
    gpiops_cfg_ptr = XGpioPs_LookupConfig(GPIOPS_ID);
    XGpioPs_CfgInitialize(&gpiops_inst, gpiops_cfg_ptr, gpiops_cfg_ptr->BaseAddr);

    // Establecemos pines como salida
    XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_LED0, 1);
    XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_LED1, 1);

    XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_LED0, 1);
    XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_LED1, 1);

    print("Hello World\n\r");
    print("Successfully ran Hello World application\r\n");

    while(TRUE)
    {
        XGpioPs_WritePin(&gpiops_inst, EMIO_LED0, 1);
        XGpioPs_WritePin(&gpiops_inst, EMIO_LED1, 1);
        usleep(500*MS_TO_US);
        XGpioPs_WritePin(&gpiops_inst, EMIO_LED0, 0);
        XGpioPs_WritePin(&gpiops_inst, EMIO_LED1, 0);
        usleep(500*MS_TO_US);
    }

    cleanup_platform();
    return 0;
}
