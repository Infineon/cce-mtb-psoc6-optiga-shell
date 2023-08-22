/******************************************************************************
* File Name:   example_optiga_init_deinit.c
*
* Description: This file provides an example for initializing and deinitializing
* 			   OPTIGA™
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


#include "optiga_example.h"
#include "optiga/optiga_util.h"

#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
extern optiga_lib_status_t pair_host_and_optiga_using_pre_shared_secret(void);
#endif

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
/* lint --e{818} suppress "argument "context" is not used in the sample provided" */
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        /* callback to upper layer here */
    }
}

optiga_util_t * me_util_instance = NULL;

void example_optiga_init(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
    static uint8_t host_optiga_pairing_completed = FALSE;
#endif
    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);

    do
    {
        if (NULL == me_util_instance)
        {
            /* Create an instance of optiga_util to open the application on OPTIGA. */
            me_util_instance = optiga_util_create(0, optiga_lib_callback, NULL);
            if (NULL == me_util_instance)
            {
                break;
            }
        }
        
        /**
         * Open the application on OPTIGA which is a precondition to perform any other operations
         * using optiga_util_open_application
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_open_application(me_util_instance, 0);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);         
        
#ifdef OPTIGA_COMMS_SHIELDED_CONNECTION
        if(FALSE == host_optiga_pairing_completed)
        {
            OPTIGA_EXAMPLE_LOG_MESSAGE("pair_host_and_optiga_using_pre_shared_secret");
            /* Generate the pre-shared secret on host and write it to OPTIGA */
            return_status = pair_host_and_optiga_using_pre_shared_secret();              
            if (OPTIGA_LIB_SUCCESS != return_status)
            {
                /* pairing of host and optiga failed */
                break;
            }
            host_optiga_pairing_completed = TRUE;
        }
#endif         
    }while(FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
}

void example_optiga_deinit(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
    
    do
    {
        /**
         * Close the application on OPTIGA after all the operations are executed
         * using optiga_util_close_application
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_close_application(me_util_instance, 0);
            
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /* destroy util and crypt instances */
        /* lint --e{534} suppress "Error handling is not required so return value is not checked" */
        optiga_util_destroy(me_util_instance);
        me_util_instance = NULL;
    }while(FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
}

/**
* @}
*/
