/******************************************************************************
* File Name:   example_optiga_crypt_ecdsa_verify.c
*
* Description: This file provides the example for ECDSA verify operation using 
*              #optiga_crypt_ecdsa_verify.
*
*
* Related Document: See README.md
*
*
* ******************************************************************************
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

#ifdef OPTIGA_CRYPT_ECDSA_VERIFY_ENABLED

#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
extern void example_optiga_init(void);
extern void example_optiga_deinit(void);
#endif

void example_util_encode_ecc_public_key_in_bit_string_format(const uint8_t * q_buffer,
                                                        uint8_t q_length,
                                                        uint8_t * pub_key_buffer,
                                                        uint16_t * pub_key_length);

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

static const uint8_t ecc_public_key_component [] =
{
    /* NIST-256 Public Key */
    0x8b,0x88,0x9c,0x1d,0xd6,0x07,0x58,0x2e,
    0xd6,0xf8,0x2c,0xc2,0xd9,0xbe,0xd0,0xfe,
    0x64,0xf3,0x24,0x5e,0x94,0x7d,0x54,0xcd,
    0x20,0xdc,0x58,0x98,0xcf,0x51,0x31,0x44,
    0x22,0xea,0x01,0xd4,0x0b,0x23,0xb2,0x45,
    0x7c,0x42,0xdf,0x3c,0xfb,0x0d,0x33,0x10,
    0xb8,0x49,0xb7,0xaa,0x0a,0x85,0xde,0xe7,
    0x6a,0xf1,0xac,0x31,0x31,0x1e,0x8c,0x4b
};

/* SHA-256 Digest */
static const uint8_t digest [] =
{
    0xE9,0x5F,0xB3,0xB1,0x9F,0xA4,0xDD,0x27,0xFE,0xAE,0xB3,0x33,0x40,0x80,0xCE,0x35,
    0xDF,0x3E,0x08,0xF1,0x6F,0x36,0xF3,0x24,0x0E,0xB0,0xB3,0x2F,0xAB,0xD0,0x90,0xCA,
};

/* ECDSA Signature */
static const uint8_t signature [] =
{
    /* DER INTEGER Format */
    0x02,
    /* Length */
    0x20,
    0x39,0xA4,0x70,0xE9,0x32,0x30,0xF5,0x5F,
    0xA4,0xDF,0x8A,0x07,0x36,0x58,0x65,0xC6,
    0xE6,0x1B,0x07,0x51,0xFB,0xC6,0x16,0x05,
    0xEB,0xDF,0x56,0x6D,0xA9,0x50,0x3B,0x24,
    /* DER INTEGER Format */
    0x02,
    /* Length */
    0x1E,
    0x49,0x33,0x6C,0x07,0x2B,0xD0,0x40,0x20,
    0x0F,0xD4,0xE0,0x7E,0x67,0x66,0xC4,0xF5,
    0x7F,0x98,0xEC,0x38,0xB8,0xEF,0x44,0x8F,
    0x6A,0xE1,0xFD,0x1E,0x92,0xB4,
};

uint8_t ecc_public_key[70] = {0x00};
uint16_t ecc_public_key_length = 0;

/**
 * The below example demonstrates the verification of signature using
 * the public key provided by host.
 *
 * Example for #optiga_crypt_ecdsa_verify
 *
 */
void example_optiga_crypt_ecdsa_verify(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;

    optiga_crypt_t * me = NULL;
    uint32_t time_taken = 0;
    example_util_encode_ecc_public_key_in_bit_string_format(ecc_public_key_component,
                                                        sizeof(ecc_public_key_component),
                                                        ecc_public_key,
                                                        &ecc_public_key_length);
    
    public_key_from_host_t public_key_details = {
                                                 ecc_public_key,
                                                 ecc_public_key_length,
                                                 (uint8_t)OPTIGA_ECC_CURVE_NIST_P_256
                                                };


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

        START_PERFORMANCE_MEASUREMENT(time_taken);
        
        /**
         * 2. Verify ECDSA signature using public key from host
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_crypt_ecdsa_verify (me,
                                                   digest,
                                                   sizeof(digest),
                                                   signature,
                                                   sizeof(signature),
                                                   OPTIGA_CRYPT_HOST_DATA,
                                                   &public_key_details);

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

#endif  /* OPTIGA_CRYPT_ECDSA_VERIFY_ENABLED */
