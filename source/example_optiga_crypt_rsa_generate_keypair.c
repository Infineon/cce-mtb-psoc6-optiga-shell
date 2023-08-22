/******************************************************************************
* File Name:   example_optiga_crypt_rsa_generate_keypair.c
*
* Description: This file provides the example for generation of RSA keypair
*              using #optiga_crypt_rsa_generate_keypair.
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


#include "optiga/optiga_util.h"
#include "optiga/optiga_crypt.h"
#include "optiga_example.h"

#ifdef OPTIGA_CRYPT_RSA_GENERATE_KEYPAIR_ENABLED

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
 * Sample metadata of 0xE0F1 
 */
const uint8_t E0FC_metadata[] = { 0x20, 0x06, 0xD0, 0x01, 0x00, 0xD3, 0x01, 0x00 };

/**
 * The below example demonstrates the generation of
 * RSA Key pair using #optiga_crypt_rsa_generate_keypair.
 *
 */
void example_optiga_crypt_rsa_generate_keypair(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
    optiga_key_id_t optiga_key_id;
    uint32_t time_taken = 0;
    uint16_t optiga_oid;
    
    /* To store the generated public key as part of Generate key pair
    *
    *
    * Give a buffer of minimum size required to store the exported public key.
    * E.g
    * For 1024 bit key : modulus(128 bytes) + public expoenent(3 bytes) + Encoding bytes length(approximately 15 bytes)
    * For 2048 bit key : modulus(256 bytes) + public expoenent(3 bytes) + Encoding bytes length(approximately 15 bytes)
    */
    uint8_t public_key [150];
    uint16_t public_key_length = sizeof(public_key);

    optiga_crypt_t * crypt_me = NULL;
    optiga_util_t * util_me = NULL;    

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

        optiga_lib_status = OPTIGA_LIB_BUSY;
        optiga_oid = 0xE0FC;
        return_status = optiga_util_write_metadata(util_me,
                                                   optiga_oid,
                                                   E0FC_metadata,
                                                   sizeof(E0FC_metadata));

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);
        
        /**
         * 2. Generate RSA Key pair
         *       - Use 1024 or 2048 bit RSA key
         *       - Specify the Key Usage
         *       - Store the Private key in OPTIGA Key store
         *              (When exporting the private key, provide buffer of sufficient length (key size in bytes +
         *               encoding length))
         *       - Export Public Key
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        optiga_key_id = OPTIGA_KEY_ID_E0FC;
        
        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        return_status = optiga_crypt_rsa_generate_keypair(crypt_me,
                                                          OPTIGA_RSA_KEY_1024_BIT_EXPONENTIAL,
                                                          (uint8_t)OPTIGA_KEY_USAGE_SIGN,
                                                          FALSE,
                                                          &optiga_key_id,
                                                          public_key,
                                                          &public_key_length);
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
    
}

#endif  /* OPTIGA_CRYPT_RSA_GENERATE_KEYPAIR_ENABLED */
