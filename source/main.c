/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSoC™ 6 MCU and OPTIGA™ Trust M
*              for ModusToolbox.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2022, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "optiga_example.h"
#include "optiga/common/optiga_lib_logger.h"
#include "optiga/pal/pal.h"

/*******************************************************************************
* Macros
*******************************************************************************/

#define EXAMPLE_MODULE "[optiga example]  : "

#define MESSAGE(msg) \
        optiga_lib_print_message(msg, EXAMPLE_MODULE, \
        OPTIGA_LIB_LOGGER_COLOR_LIGHT_GREEN);
                            
#define ERROR(msg) \
        optiga_lib_print_message(msg, OPTIGA_SHELL_MODULE, \
        OPTIGA_LIB_LOGGER_COLOR_YELLOW);

#define TAB "                    "

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

extern void optiga_shell_wait_for_user(void);
extern void optiga_shell_begin(void);


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It initialises all optiga relevant 
* routines and executes dedicated cryptography relevant examples.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    
    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();


    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** OPTIGA: Cryptography Example ****************** \r\n\n");

    /* 
      PAL stands for Pltaform Abstraction Layer in OPTIGA Host library 
      Here all the target system relevant function started
    */
    pal_init();

    optiga_shell_wait_for_user();
    optiga_shell_begin();

}

/* [] END OF FILE */
