/******************************************************************************
* File Name:   example_optiga_crypt_hash.c
*
* Description: This file provides the example for hashing operations using
*              #optiga_crypt_hash_start, #optiga_crypt_hash_update and
*              #optiga_crypt_hash_finalize.
*
*
* Related Document: See README.md
*
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

#include "optiga/optiga_crypt.h"
#include "optiga_example.h"

#ifdef OPTIGA_CRYPT_HASH_ENABLED

#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
extern void example_optiga_init(void);
extern void example_optiga_deinit(void);
#endif

/**
 * Callback when optiga_crypt_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
/* lint --e{818} suppress "argument "context" is not used in the sample provided" */
static void optiga_crypt_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        /* callback to upper layer here */
    }
}

/**
 * Prepare the hash context
 */
#define OPTIGA_HASH_CONTEXT_INIT(hash_context,p_context_buffer,context_buffer_size,hash_type) \
{                                                               \
    hash_context.context_buffer = p_context_buffer;             \
    hash_context.context_buffer_length = context_buffer_size;   \
    hash_context.hash_algo = hash_type;                         \
}

/**
 * The below example demonstrates the generation of digest using
 * optiga_crypt_hash_xxxx operations.
 *
 * Example for #optiga_crypt_hash_start, #optiga_crypt_hash_update,
 * #optiga_crypt_hash_finalize
 *
 */
void example_optiga_crypt_hash(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;

    /* Note : The size of the hash context provided by OPTIGA is 209 bytes for SHA-256 */
    uint8_t hash_context_buffer [OPTIGA_HASH_CONTEXT_LENGTH_SHA_256];
    optiga_hash_context_t hash_context;

    uint8_t data_to_hash [] = {"OPTIGA, Infineon Technologies AG"};
    hash_data_from_host_t hash_data_host;

    uint8_t digest [32];
    uint32_t time_taken = 0;

    optiga_crypt_t * me = NULL;

    do
    {

#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        /**
         * Open the application on OPTIGA which is a precondition to perform any other operations
         * using optiga_util_open_application
         */
        example_optiga_init();
#endif /* OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY */
        
        OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
        /**
         * 1. Create OPTIGA Crypt Instance
         */
        me = optiga_crypt_create(0, optiga_crypt_callback, NULL);
        if (NULL == me)
        {
            break;
        }

        /**
         * 2. Initialize the Hash context
         */
        
        OPTIGA_HASH_CONTEXT_INIT(hash_context,hash_context_buffer,  \
                                 sizeof(hash_context_buffer),(uint8_t)OPTIGA_HASH_TYPE_SHA_256);

        optiga_lib_status = OPTIGA_LIB_BUSY;

        /**
         * 3. Initialize the hashing context at OPTIGA
         */
        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = optiga_crypt_hash_start(me, &hash_context);
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 4. Continue hashing the data
         */
        hash_data_host.buffer = data_to_hash;
        hash_data_host.length = sizeof(data_to_hash);

        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_hash_update(me,
                                                 &hash_context,
                                                 OPTIGA_CRYPT_HOST_DATA,
                                                 &hash_data_host);
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        /**
         * 5. Finalize the hash
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_hash_finalize(me,
                                                   &hash_context,
                                                   digest);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        READ_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = OPTIGA_LIB_SUCCESS;

    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Close the application on OPTIGA after all the operations are executed
     * using optiga_util_close_application
     */
    example_optiga_deinit();
#endif /* OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY */
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken, return_status); 
    
    if (me)
    {
        /* Destroy the instance after the completion of usecase if not required. */
        return_status = optiga_crypt_destroy(me);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            /* lint --e{774} suppress This is a generic macro */
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
}

/**
 * The below example demonstrates the generation of digest using
 * optiga_crypt_hash operations.
 *
 * Example for #optiga_crypt_hash
 *
 */
void example_optiga_crypt_hash_data(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;

    uint8_t data_to_hash [] = {"OPTIGA, Infineon Technologies AG"};
    hash_data_from_host_t hash_data_host;

    uint8_t digest [32];
    uint32_t time_taken = 0;

    optiga_crypt_t * me = NULL;

    do
    {
        
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        /**
         * Open the application on OPTIGA which is a precondition to perform any other operations
         * using optiga_util_open_application
         */
        example_optiga_init();
#endif /* OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY */
        
        OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);

        /**
         * 1. Create OPTIGA Crypt Instance
         */
        me = optiga_crypt_create(0, optiga_crypt_callback, NULL);
        if (NULL == me)
        {
            break;
        }

        /**
         * 2. Initialize the hashing context at OPTIGA
         */
        hash_data_host.buffer = data_to_hash;
        hash_data_host.length = sizeof(data_to_hash);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        
        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = optiga_crypt_hash(me, OPTIGA_HASH_TYPE_SHA_256, OPTIGA_CRYPT_HOST_DATA, &hash_data_host, digest);
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        READ_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = OPTIGA_LIB_SUCCESS;

    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Close the application on OPTIGA after all the operations are executed
     * using optiga_util_close_application
     */
    example_optiga_deinit();
#endif /* OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY  */
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken, return_status);
    
    if (me)
    {
        /* Destroy the instance after the completion of usecase if not required. */
        return_status = optiga_crypt_destroy(me);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            /* lint --e{774} suppress This is a generic macro */
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    
}

#endif  /* OPTIGA_CRYPT_HASH_ENABLED */