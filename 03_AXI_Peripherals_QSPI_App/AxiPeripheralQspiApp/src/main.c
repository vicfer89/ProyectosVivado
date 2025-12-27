#include <stdio.h>
#include <xinterrupt_wrap.h>
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
#define TIMER_CNTR_0       0
#define RESET_VALUE_500MS  (0xFFFFFFFF - 0x17D7840)
#define RESET_VALUE_1000MS (0xFFFFFFFF - 0x2FAF080)
                           

// Variables globales
bool Led1State = false;
bool Led2State = false;

// Instancias de controladores
XScuGic InterruptController;    /* Instance of the Interrupt Controller */
XGpio GPIOInstance_Ptr;         /* Instancia de GPIO */
XTmrCtr Timer500msInstance;  /* Instancia de timer de 500 ms */
XTmrCtr Timer1000msInstance; /* Instancia de timer de 1000 ms */


void timer_500ms_interrupt_handler(void *data, u8 TmrCtrNumber)
{
    (true == Led1State) ? XGpio_DiscreteClear(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK) : XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK);
    Led1State = !Led1State;
    printf("Interrupción de 500 ms alcanzada \r\n");
}

void timer_1000ms_interrupt_handler(void *data, u8 TmrCtrNumber)
{
    (true == Led2State) ? XGpio_DiscreteClear(&GPIOInstance_Ptr, LEDS_CHANNEL, LED2_MASK) : XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED2_MASK);
    Led2State = !Led2State;
    printf("Interrupción de 1000 ms alcanzada \r\n");
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
    Led1State = true;
    XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED2_MASK);
    Led2State = true;

}

static void initTimer500ms()
{
    int Status;

    /* Inicializamos el timer */
    Status = XTmrCtr_Initialize(&Timer500msInstance, XPAR_AXI_TIMER_500MS_BASEADDR);
    if(XST_SUCCESS != Status)
    {
        printf("Error inicializando el timer en %s, linea %u\r\n", __PRETTY_FUNCTION__, __LINE__);
    }

    /* Realizamos SelfTest del timer */
    Status = XTmrCtr_SelfTest(&Timer500msInstance, TIMER_CNTR_0);
    if(XST_SUCCESS == Status)
    {
        printf("Test de timer completado de forma correcta\r\n");
    }
    else
    {
        printf("Error en inicialización del timer en %s, linea %u\r\n", __PRETTY_FUNCTION__, __LINE__);
    }

    /* Activamos las interrupciones del sistema */
    // Asignamos interrupción
    Status = XSetupInterruptSystem(&Timer500msInstance, (XInterruptHandler)XTmrCtr_InterruptHandler, 
        Timer500msInstance.Config.IntrId, Timer500msInstance.Config.IntrParent, XINTERRUPT_DEFAULT_PRIORITY);
    if(XST_SUCCESS != Status)
    {
        printf("Error inicializando la interrupción del timer en %s, linea %u\r\n", __PRETTY_FUNCTION__, __LINE__);
    }

    // Asignamos manejador para interrupción
    XTmrCtr_SetHandler(&Timer500msInstance, timer_500ms_interrupt_handler, &Timer500msInstance);

    /* Configuramos opciones del timer */
    XTmrCtr_SetOptions(&Timer500msInstance, TIMER_CNTR_0,
			   XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

    /* Configuramos valor del timer */
    XTmrCtr_SetResetValue(&Timer500msInstance, TIMER_CNTR_0, RESET_VALUE_500MS);

    /* Comenzamos con la ejecución del timer */
    XTmrCtr_Start(&Timer500msInstance, TIMER_CNTR_0);
}

static void initTimer1000ms()
{
    int Status;

    /* Inicializamos el timer */
    Status = XTmrCtr_Initialize(&Timer1000msInstance, XPAR_AXI_TIMER_1000MS_BASEADDR);
    if(XST_SUCCESS != Status)
    {
        printf("Error inicializando el timer en %s, linea %u\r\n", __PRETTY_FUNCTION__, __LINE__);
    }

    /* Realizamos SelfTest del timer */
    Status = XTmrCtr_SelfTest(&Timer1000msInstance, TIMER_CNTR_0);
    if(XST_SUCCESS == Status)
    {
        printf("Test de timer completado de forma correcta\r\n");
    }
    else
    {
        printf("Error en inicialización del timer en %s, linea %u\r\n", __PRETTY_FUNCTION__, __LINE__);
    }

    /* Activamos las interrupciones del sistema */
    // Asignamos interrupción
    Status = XSetupInterruptSystem(&Timer1000msInstance, (XInterruptHandler)XTmrCtr_InterruptHandler, 
        Timer1000msInstance.Config.IntrId, Timer1000msInstance.Config.IntrParent, XINTERRUPT_DEFAULT_PRIORITY);
    if(XST_SUCCESS != Status)
    {
        printf("Error inicializando la interrupción del timer en %s, linea %u\r\n", __PRETTY_FUNCTION__, __LINE__);
    }

    // Asignamos manejador para interrupción
    XTmrCtr_SetHandler(&Timer1000msInstance, timer_1000ms_interrupt_handler, &Timer1000msInstance);

    /* Configuramos opciones del timer */
    XTmrCtr_SetOptions(&Timer1000msInstance, TIMER_CNTR_0,
			   XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

    /* Configuramos valor del timer */
    XTmrCtr_SetResetValue(&Timer1000msInstance, TIMER_CNTR_0, RESET_VALUE_1000MS);

    /* Comenzamos con la ejecución del timer */
    XTmrCtr_Start(&Timer1000msInstance, TIMER_CNTR_0);
}

int main()
{
    init_platform();

    printf("\n\rAxiPeripheralQspiApp @ %s\n\r", __FILE_NAME__);
    printf("Aplicacion %s - %s\r\n", __DATE__, __TIME__);
    print("Successfully ran Hello World application\r\n");

    GPIOInit();
    initTimer500ms();
    initTimer1000ms();

    while(true)
    {
        /*
        XGpio_DiscreteClear(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK);
        usleep(500*MS_TO_US);
        XGpio_DiscreteSet(&GPIOInstance_Ptr, LEDS_CHANNEL, LED1_MASK);
        usleep(500*MS_TO_US);
        */        
    }

    cleanup_platform();
    return 0;
}
