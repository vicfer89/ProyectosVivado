#include <stdio.h>
#include <xstatus.h>
#include "stdbool.h"
#include "platform.h"
#include "xil_printf.h"
#include "xil_types.h"
#include "xgpio.h"
#include "xparameters.h"
#include "xtmrctr.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "sleep.h"

// Definiciones para leds
#define LEDS_CHANNEL       0x01
#define LEDS_DIRECTIONMASK 0x00
#define LED1_MASK          0b01
#define LED2_MASK          0b10
#define MS_TO_US           1000

// Variables globales
bool fTimer500ms = false;
bool fTimer1000ms = false;

// Instancias de controladores
XScuGic InterruptController;    /* Instance of the Interrupt Controller */
XGpio GPIOInstance_Ptr;         /* Instancia de GPIO */
XTmrCtr Timer500msInstancePtr;  /* Instancia de timer de 500 ms */
XTmrCtr Timer1000msInstancePtr; /* Instancia de timer de 1000 ms */


void timer_500ms_interrupt_handler(void *data, u8 TmrCtrNumber)
{
    fTimer500ms = true;
}

void timer_1000ms_interrupt_handler(void *data, u8 TmrCtrNumber)
{
    fTimer1000ms = true;
}

static void GPIOInit()
{
    /* 1 - Inicializamos el periférico AXI de GPIO */
    int xStatus;
    xStatus = XGpio_Initialize(&GPIOInstance_Ptr, XPAR_AXI_GPIO_0_BASEADDR);
    if(XST_SUCCESS != xStatus)
    {
        printf("Error en inicialización de GPIO\r\n");
    }
    /* 2 - Seleccionamos dirección de GPIO del periférico */
    XGpio_SetDataDirection(&GPIOInstance_Ptr, LEDS_CHANNEL, LEDS_DIRECTIONMASK);
    /* 3 - Escribimos el valor inicial del pin */
    XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK);
    XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED2_MASK);

}

int main()
{
    init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application\r\n");

    GPIOInit();

    while(true)
    {
        XGpio_DiscreteClear(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK);
        usleep(500*MS_TO_US);
        XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK);
        usleep(500*MS_TO_US);
    }

    cleanup_platform();
    return 0;
}
