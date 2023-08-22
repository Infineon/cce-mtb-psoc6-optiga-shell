/******************************************************************************
* File Name:   example_optiga_crypt_symmetric_generate_key.c
*
* Description: This file provides the example for generation of symmetric key
*              using optiga_crypt_symmetric_generate_key.
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


#include "optiga/optiga_util.h"
#include "optiga/optiga_crypt.h"
#include "optiga_example.h"

#ifdef OPTIGA_CRYPT_SYM_GENERATE_KEY_ENABLED

#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
extern void example_optiga_init(void);
extern void example_optiga_deinit(void);
#endif

uint32_t time_taken_to_generate_key = 0;

#define METADATA_TAG_KEY_ALGO_ID     (0xE0)
extern optiga_lib_status_t example_check_tag_in_metadata(const uint8_t * buffer, 
                                                         const uint8_t buffer_length, 
                                                         const uint8_t tag,
                                                         bool_t * tag_available);

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

/* lint --e{818} suppress "argument "context" is not used in the sample provided" */
static void optiga_util_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        /* callback to upper layer here */
    }
}

/**
 * Sample metadata of 0xE200 
 */
const uint8_t E200_metadata[] = { 0x20, 0x06, 0xD0, 0x01, 0x00, 0xD3, 0x01, 0x00 };

/**
 * The below generates symmetric Key using optiga_crypt_symmetric_generate_key.
 *
 */
optiga_lib_status_t generate_symmetric_key(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    optiga_key_id_t symmetric_key;
    uint16_t optiga_oid, bytes_to_read;
    uint8_t read_data_buffer[100];
    bool_t  is_tag = FALSE;

    optiga_crypt_t * crypt_me = NULL;
    optiga_util_t * util_me = NULL;    

    do
    {
       /**
         * 1. Create OPTIGA Crypt Instance
         */
        crypt_me = optiga_crypt_create(0, optiga_crypt_callback, NULL);
        if (NULL == crypt_me)
        {
            break;
        }

        util_me = optiga_util_create(0, optiga_util_callback, NULL);
        if (NULL == util_me)
        {
            break;
        }
        
        /**
         * Read metadata of a data object (e.g. key data object 0xE200)
         * using optiga_util_read_metadata.
         */
        optiga_oid = 0xE200;
        bytes_to_read = sizeof(read_data_buffer);
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_read_metadata(util_me,
                                                  optiga_oid,
                                                  read_data_buffer,
                                                  &bytes_to_read);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        return_status = example_check_tag_in_metadata(read_data_buffer,
                                                      bytes_to_read,
                                                      METADATA_TAG_KEY_ALGO_ID,
                                                      &is_tag);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            return_status = OPTIGA_LIB_SUCCESS;
            break;
        }
        
        optiga_lib_status = OPTIGA_LIB_BUSY;
        optiga_oid = 0xE200;
        return_status = optiga_util_write_metadata(util_me,
                                                   optiga_oid,
                                                   E200_metadata,
                                                   sizeof(E200_metadata));

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        /**
         * 2. Generate symmetric key
         *       - Use AES-128 key type
         *       - OPTIGA_KEY_USAGE_ENCRYPTION as a Key Usage
         *       - Store the Symmetric key in OPTIGA Key store OID(E200)
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        symmetric_key = OPTIGA_KEY_ID_SECRET_BASED;
        
        START_PERFORMANCE_MEASUREMENT(time_taken_to_generate_key);
        
        return_status = optiga_crypt_symmetric_generate_key(crypt_me,
                                                            OPTIGA_SYMMETRIC_AES_128,
                                                            (uint8_t)OPTIGA_KEY_USAGE_ENCRYPTION,
                                                            FALSE,
                                                            &symmetric_key);
        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        READ_PERFORMANCE_MEASUREMENT(time_taken_to_generate_key);
        
        return_status = OPTIGA_LIB_SUCCESS;
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);

    if (crypt_me)
    {
        /* Destroy the instance after the completion of usecase if not required. */
        return_status = optiga_crypt_destroy(crypt_me);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            /* lint --e{774} suppress This is a generic macro */
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    if (util_me)
    {
        /* Destroy the instance after the completion of usecase if not required. */
        return_status = optiga_util_destroy(util_me);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            /* lint --e{774} suppress This is a generic macro */
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
    return return_status;
}

/**
 * The below example demonstrates the generation of
 * symmetric Key using optiga_crypt_symmetric_generate_key.
 *
 */
void example_optiga_crypt_symmetric_generate_key(void)
{
    optiga_lib_status_t return_status;
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
        /**
         * Open the application on OPTIGA which is a precondition to perform any other operations
         * using optiga_util_open_application
         */
        example_optiga_init();
#endif /* OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY */

    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);
        
    return_status = generate_symmetric_key();
    
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    /**
     * Close the application on OPTIGA after all the operations are executed
     * using optiga_util_close_application
     */
    example_optiga_deinit();
#endif /* OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY */
    OPTIGA_EXAMPLE_LOG_PERFORMANCE_VALUE(time_taken_to_generate_key, return_status);
}

#endif  /* OPTIGA_CRYPT_SYM_GENERATE_KEY_ENABLED */
/**
* @}
*/
