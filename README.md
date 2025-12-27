# Repositorio para pruebas de placa PZ7020-Starlite

Enlace de compra y fabricante:

- Enlace de fabricante - [PZ7020 Starlite](https://www.en.puzhi.com/Product/AMD-FPGA-Development-Board/Zynq-7000-SoC/PZ7020-StarLite)
- Enlace de compra - [Aliexpress](https://es.aliexpress.com/item/1005006434901972.html)

## Documentación y guías:
 
- Guía de Xilinx para inicio rápido en Zynq7020 - [Zynq 7000 Embedded Design Tutorial (UG1165)](https://docs.amd.com/r/en-US/ug1165-zynq-embedded-design-tutorial/)
- Guía de Drivers para Baremetal - [Baremetal Drivers and Libraries](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841745/Baremetal+Drivers+and+Libraries)
- Repositorio de Xilinx para Drivers - [embeddedsw](https://github.com/Xilinx/embeddedsw/tree/master), ver contenido de ```XilinxProcessorIPLib/drivers```
- Guía de Xilinx para Petalinux - [PetaLinux Tools Documentation: Reference Guide (UG1144)](https://docs.amd.com/r/2023.2-English/ug1144-petalinux-tools-reference-guide/)

## Problemas conocidos

- **Problema con los FSBL:** Los FSBL NO funcionan de forma correcta a no ser que el HW diseñado para él sea tan solo el procesador, sin ningún periférico AXI. Se necesita tener la QSPI, SD y UART0 activas.

- Al construir Petalinux en su revisión 2023, si hay problemas con uid_map, es necesario poner lo siguiente en consola:

    >sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=0

- Al instalar Petalinux, El error package require xsdb FAILED al ejecutar comandos de PetaLinux (como petalinux-config) generalmente indica la falta de bibliotecas de sistema de 32 bits o dependencias de ncurses que las herramientas de AMD/Xilinx requieren para cargar sus scripts de depuración (XSDB). 

    Falta la biblioteca libtinfo5 (Causa más común en Ubuntu)

    Las versiones recientes de Ubuntu (como 24.04 o 22.04) a menudo no incluyen libtinfo5 por defecto, prefiriendo la versión 6. Sin embargo, las herramientas de Xilinx todavía dependen de la versión 5. 

    En Ubuntu 24.04: Esta versión ya no tiene el paquete en sus repositorios oficiales. Debes descargarlo manualmente de los repositorios de seguridad de Ubuntu 22.04:
    > wget http://security.ubuntu.com/ubuntu/pool/universe/n/ncurses/libtinfo5_6.3-2ubuntu0.1_amd64.deb

    > sudo apt install ./libtinfo5_6.3-2ubuntu0.1_amd64.deb
