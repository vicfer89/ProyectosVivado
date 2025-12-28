/** NOTA DE APLICACION 
*  https://docs.amd.com/v/u/en-US/xapp1079-amp-bare-metal-cortex-a9
*
*/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xil_mmu.h"
#include "xil_exception.h"
#include "xpseudo_asm.h"
#include "xscugic.h"

#define sev() __asm__ __volatile__ ("sev" : : : "memory")

#define CPU1_COPY_ADDR 0xfffffff0
#define CPU1_START_ADDR 0x200000

int main()
{

    // Deshabilita el atributo de Caché para la memoria de copia de la CPU1.
    // Esto fuerza a que el valor se actualice cuando se hace la operación Xil_Out32 con el valor de la dirección
    // de arranque de la CPU1. Es decir, garantizar la coherencia de datos y el determinismo en escenarios específicos.
    Xil_SetTlbAttributes(CPU1_COPY_ADDR, 0x14de2); // S=b1 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0

    Xil_Out32(CPU1_COPY_ADDR, CPU1_START_ADDR);
    dmb();
    sev();

    init_platform();




    print("Hello World - CPU 0\n\r");
    cleanup_platform();
    return 0;
}