/******************************************************************************
* File Name: optiga_shell.c
*
* Description: This is the source code for shell interface
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



#include "optiga/optiga_crypt.h"
#include "optiga/optiga_util.h"
#include "optiga/common/optiga_lib_logger.h"
#include "optiga/pal/pal_os_event.h"
#include "optiga/pal/pal.h"
#include "optiga/pal/pal_os_timer.h"
#include "optiga_example.h"
#include "optiga/pal/pal_logger.h"
#include "optiga/pal/pal_gpio.h"

#define OPTIGA_SHELL		"optiga --"
#define OPTIGA_SHELL_MODULE "[optiga shell]  : "
#define OPTIGA_SHELL_LOG_MESSAGE(msg) \
	optiga_lib_print_message(msg, OPTIGA_SHELL_MODULE, OPTIGA_LIB_LOGGER_COLOR_LIGHT_GREEN);

void example_optiga_crypt_hash (void);
void example_optiga_crypt_hash_data (void);
void example_optiga_crypt_ecc_generate_keypair(void);
void example_optiga_crypt_ecdsa_sign(void);
void example_optiga_crypt_ecdsa_verify(void);
void example_optiga_crypt_ecdh(void);
void example_optiga_crypt_random(void);
void example_optiga_crypt_tls_prf_sha256(void);
void example_optiga_util_read_data(void);
void example_optiga_util_write_data(void);
void example_optiga_crypt_rsa_generate_keypair(void);
void example_optiga_crypt_rsa_sign(void);
void example_optiga_crypt_rsa_verify(void);
void example_optiga_crypt_rsa_decrypt_and_export(void);
void example_optiga_crypt_rsa_decrypt_and_store(void);
void example_optiga_crypt_rsa_encrypt_message(void);
void example_optiga_crypt_rsa_encrypt_session(void);
void example_optiga_util_update_count(void);
void example_optiga_util_protected_update(void);
void example_read_coprocessor_id(void);
void example_pair_host_and_optiga_using_pre_shared_secret(void);
void example_optiga_util_hibernate_restore(void);
void example_optiga_crypt_symmetric_encrypt_decrypt_ecb(void);
void example_optiga_crypt_symmetric_encrypt_decrypt_cbc(void);
void example_optiga_crypt_symmetric_encrypt_cbcmac(void);
void example_optiga_crypt_hmac(void);
void example_optiga_crypt_hkdf(void);
void example_optiga_crypt_symmetric_generate_key(void);
void example_optiga_hmac_verify_with_authorization_reference(void);
void example_optiga_crypt_clear_auto_state(void);

extern pal_logger_t logger_console;

/**
 * Copy of pal_logger_read with modified format specifier in sprintf to support capture of \n and \r
 */
static pal_status_t optiga_shell_pal_logger_read(void * p_logger_context, uint8_t * p_log_data, uint32_t log_data_length)
{
    char str[4] = "%";
    char str_1[4];
    sprintf(str_1, "%dc", (int)log_data_length);
    strcat(str, str_1);
    scanf(str, p_log_data);

    return PAL_STATUS_SUCCESS;
}

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
static void optiga_util_callback(void * context, optiga_lib_status_t return_status)
{
	optiga_lib_status = return_status;
}

optiga_util_t * me_util = NULL;

typedef struct optiga_example_cmd
{
	const char_t * cmd_description;
	const char_t * cmd_options;
	void (*cmd_handler)();
}optiga_example_cmd_t;

static void optiga_shell_init()
{
	optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;
	uint16_t optiga_oid = 0xE0C4;
	uint8_t required_current = 15;

	do
	{
        if (NULL == me_util)
        {
            /*
             * Create an instance of optiga_util to open the application on OPTIGA.
             */
            me_util = optiga_util_create(0, optiga_util_callback, NULL);
            if (NULL == me_util)
            {
                break;
            }
        }
		OPTIGA_EXAMPLE_LOG_MESSAGE("Initializing OPTIGA for example demonstration...\n");
		/**
		 * Open the application on OPTIGA which is a precondition to perform any other operations
		 * using optiga_util_open_application
		 */
		optiga_lib_status = OPTIGA_LIB_BUSY;
		return_status = optiga_util_open_application(me_util, 0);

		if (OPTIGA_LIB_SUCCESS != return_status)
		{
			break;
		}
		while (optiga_lib_status == OPTIGA_LIB_BUSY)
		{
			/*
			 * Wait until the optiga_util_open_application is completed
			 */
		}
		if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
		{
			return_status = optiga_lib_status;
			/*
			 * optiga util open application failed
			 */
			break;
		}

		OPTIGA_SHELL_LOG_MESSAGE("Initializing OPTIGA completed...\n\n");
		OPTIGA_SHELL_LOG_MESSAGE("Begin pairing of host and OPTIGA...");
		/*
		 * Usercase: Generate the pre-shared secret on host and write it to OPTIGA
		 */
		example_pair_host_and_optiga_using_pre_shared_secret();
		OPTIGA_SHELL_LOG_MESSAGE("Pairing of host and OPTIGA completed...");

		/*
		 * Setting current limitation to maximum supported value(15)
		 */

		optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_write_data(me_util,
                                               optiga_oid,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               0, &required_current,
                                               1);

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            /*
             * Wait until the optiga_util_write_data operation is completed
             */
        }
        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            return_status = optiga_lib_status;
            break;
        }
        OPTIGA_SHELL_LOG_MESSAGE("Setting current limitation to maximum...");
        OPTIGA_SHELL_LOG_MESSAGE("Starting OPTIGA example demonstration..\n");
	}while(FALSE);

	OPTIGA_EXAMPLE_LOG_STATUS(return_status);
}

static void optiga_shell_deinit()
{
	optiga_lib_status_t return_status;
	do
	{
		OPTIGA_SHELL_LOG_MESSAGE("Deinitializing OPTIGA for example demonstration...");
		/**
		 * Close the application on OPTIGA after all the operations are executed
		 * using optiga_util_close_application
		 */

		optiga_lib_status = OPTIGA_LIB_BUSY;
		return_status = optiga_util_close_application(me_util, 0);

		if (OPTIGA_LIB_SUCCESS != return_status)
		{
			break;
		}

		while (optiga_lib_status == OPTIGA_LIB_BUSY)
		{
			/*
			 * Wait until the optiga_util_close_application is completed
			 */
		}

		if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
		{
			/*
			 * optiga util close application failed
			 */
			OPTIGA_SHELL_LOG_MESSAGE("OPTIGA util close application failed\n");
			break;
		}

		/*
		 * destroy util and crypt instances if no re-initialisation of optiga trust m is required
		 * optiga_util_destroy(me_util);
		 * me_util = NULL;
		 */

	}while(FALSE);

	OPTIGA_EXAMPLE_LOG_STATUS(optiga_lib_status);
	OPTIGA_SHELL_LOG_MESSAGE("Deinitializing OPTIGA completed");
}

static void optiga_shell_util_read_data()
{
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    OPTIGA_SHELL_LOG_MESSAGE("Initializing OPTIGA for example demonstration...");
#endif
	OPTIGA_SHELL_LOG_MESSAGE("Starting Read Data/Metadata Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Read Certificate (~500 bytes)");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Read Certificate Metadata");
#ifndef OPTIGA_INIT_DEINIT_DONE_EXCLUSIVELY
    OPTIGA_SHELL_LOG_MESSAGE("3 Step: Close the application on OPTIGA");
#endif
	example_optiga_util_read_data();
}
static void optiga_shell_util_write_data()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Write Data/Metadata Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Write Sample Certificate in Trust Anchor Data Object (640 bytes)");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Write new Metadata");
	example_optiga_util_write_data();
}
static void optiga_shell_util_read_coprocessor_id(void)
{
    /*
     * OPTIGA_SHELL_LOG_MESSAGE("Initializing OPTIGA for example demonstration...");
     */
    OPTIGA_SHELL_LOG_MESSAGE("Starting reading of Coprocessor ID and displaying it's individual components Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Read Coprocessor UID from OID(0xE0C2) ");
    /*
     * OPTIGA_SHELL_LOG_MESSAGE("2 Step: Close the application on OPTIGA");
     */
    example_read_coprocessor_id();
}
static void optiga_shell_pair_host_optiga()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Pairing of Host and Trust M Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Read and Check existing Metadata for the Binding Secret");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Generate Random for the new Binding Secret");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Write new Binding Secret");
	OPTIGA_SHELL_LOG_MESSAGE("4 Step: Store new Binding Secret on the Host");
	example_pair_host_and_optiga_using_pre_shared_secret();
}
static void optiga_shell_util_hibernate_restore()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Hibernate and Restore Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Open Application on the security chip");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Pair the host and the security chip");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Select Protected I2C Connection");
	OPTIGA_SHELL_LOG_MESSAGE("4 Step: Generate ECC NIST P-256 Key pair and store it in Session Data Object, export the public key");
	OPTIGA_SHELL_LOG_MESSAGE("5 Step: Check Security Event Counter and wait till it reaches 0");
	OPTIGA_SHELL_LOG_MESSAGE("6 Step: Perform Close application with Hibernate parameter set to True");
	OPTIGA_SHELL_LOG_MESSAGE("7 Step: Open Application on the security chip");
	OPTIGA_SHELL_LOG_MESSAGE("8 Step: Sign prepared data with private key stored in Session Data Object");
	OPTIGA_SHELL_LOG_MESSAGE("9 Step: Verify the signature with the public key generated previously");
	OPTIGA_SHELL_LOG_MESSAGE("10 Step: Close Applicaiton on the chip");
	OPTIGA_SHELL_LOG_MESSAGE("Important note: To continue with other examples you need to call the init parameter once again");
	example_optiga_util_hibernate_restore();
}
static void optiga_shell_util_update_count()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Update Counter Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Write Initial Counter Value");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Increase Counter Object");
	example_optiga_util_update_count();
}
static void optiga_shell_util_protected_update()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Protected Update Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Update Metadata for the Object to be updated and the Trust Anchor used to verify the update");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Write Trust Anchor used by the Trust M to verify the update");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Start Protected update with prepared manifest and fragments");
	example_optiga_util_protected_update();
}
static void optiga_shell_crypt_hash()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Hash Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Hash given data with Start, Update and Finalize calls");
	example_optiga_crypt_hash();
}
static void optiga_shell_crypt_hash_data(void)
{
	/*
	 * OPTIGA_SHELL_LOG_MESSAGE("Initializing OPTIGA for example demonstration...");
	 */
    OPTIGA_SHELL_LOG_MESSAGE("Starting generation of digest Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate hash of given user data ");
    /*
     * OPTIGA_SHELL_LOG_MESSAGE("2 Step: Close the application on OPTIGA");
     */
    example_optiga_crypt_hash_data();
}
static void optiga_shell_crypt_tls_prf_sha256()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting TLS PRF SHA256 (Key Deriviation) Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Write prepared Shared Secret into an Arbitrary Data Object");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Update Metadata of the Object to use the Arbitrary Data Object only via Shielded I2C Connection");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Generate Shared Secret using the Shared Secret from the Arbitrary Data Object");
	OPTIGA_SHELL_LOG_MESSAGE("4 Step: Restore Metadata of the Arbitrary Data Object");
	example_optiga_crypt_tls_prf_sha256();
}
static void optiga_shell_crypt_random()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Generate Random Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate 32 bytes random");
	example_optiga_crypt_random();
}
static void optiga_shell_crypt_ecc_generate_keypair()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting generate ECC Key Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate ECC NIST P-256 Key Pair and export the public key");
	example_optiga_crypt_ecc_generate_keypair();
}
static void optiga_shell_crypt_ecdh()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Elliptic-curve Diffie–Hellman (ECDH) Key Agreement Protocol Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Select Protected I2C Connection");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Generate new ECC NIST P-256 Key Pair");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Select Protected I2C Connection");
	OPTIGA_SHELL_LOG_MESSAGE("4 Step: Generate Shared Secret and export it");
	example_optiga_crypt_ecdh();
}
static void optiga_shell_crypt_ecdsa_sign()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting signing example for Elliptic-curve Digital Signature Algorithm (ECDSA)");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Sign prepared Data and export the signature");
	example_optiga_crypt_ecdsa_sign();
}
static void optiga_shell_crypt_ecdsa_verify()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting verification example for Elliptic-curve Digital Signature Algorithm (ECDSA)");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Verify prepared signature, with prepared public key and digest");
	example_optiga_crypt_ecdsa_verify();
}
static void optiga_shell_crypt_rsa_sign()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting signing example for PKCS#1 Ver1.5 SHA256 Signature scheme (RSA)");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Sign prepared Data and export the signature");
	example_optiga_crypt_rsa_sign();
}
static void optiga_shell_crypt_rsa_verify()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting signing example for PKCS#1 Ver1.5 SHA256 Signature scheme (RSA)");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Verify prepared signature, with prepared public key and digest");
	example_optiga_crypt_rsa_verify();
}
static void optiga_shell_rsa_generate_keypair()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting generate RSA Key Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate RSA 1024 Key Pair and export the public key");
	example_optiga_crypt_rsa_generate_keypair();
}
static void optiga_shell_crypt_rsa_decrypt_and_export()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Decrypt and Export Data with RSA Key Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate RSA 1024 Key Pair and export the public key");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Encrypt a message with RSAES PKCS#1 Ver1.5 Scheme");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Select Protected I2C Connection");
	OPTIGA_SHELL_LOG_MESSAGE("4 Step: Decrypt the message with RSAES PKCS#1 Ver1.5 Scheme and export it");
	example_optiga_crypt_rsa_decrypt_and_export();
}
static void optiga_shell_crypt_rsa_decrypt_and_store()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Decrypt and Store Data on the chip with RSA Key Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate RSA 1024 Key Pair and export the public key");
	OPTIGA_SHELL_LOG_MESSAGE("2 Step: Generate 70 bytes RSA Pre master secret which is stored in acquired session OID");
	OPTIGA_SHELL_LOG_MESSAGE("3 Step: Select Protected I2C Connection");
	OPTIGA_SHELL_LOG_MESSAGE("4 Step: Encrypt Session Data with RSA Public Key");
	OPTIGA_SHELL_LOG_MESSAGE("5 Step: Decrypt the message with RSAES PKCS#1 Ver1.5 Scheme and store it on chip");
	example_optiga_crypt_rsa_decrypt_and_store();
}
static void optiga_shell_crypt_rsa_encrypt_message()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Encrypt Data with RSA Key Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Encrypt a message with RSAES PKCS#1 Ver1.5 Scheme");
	example_optiga_crypt_rsa_encrypt_message();
}
static void optiga_shell_crypt_rsa_encrypt_session()
{
	OPTIGA_SHELL_LOG_MESSAGE("Starting Encrypt Data in Session Object on chip with RSA Key Example");
	OPTIGA_SHELL_LOG_MESSAGE("1 Step: Encrypt a message with RSAES PKCS#1 Ver1.5 Scheme stored on chip in Session Object");
	example_optiga_crypt_rsa_encrypt_session();
}
static void optiga_shell_crypt_symmetric_encrypt_decrypt_ecb(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting symmetric Encrypt and Decrypt Data for ECB mode Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate and store the AES 128 Symmetric key in OPTIGA Key store OID(E200)");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Encrypt the plain data with ECB mode");
    OPTIGA_SHELL_LOG_MESSAGE("3 Step: Decrypt the encrypted data from step 2");
    example_optiga_crypt_symmetric_encrypt_decrypt_ecb();
}

static void optiga_shell_crypt_symmetric_encrypt_decrypt_cbc(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting symmetric Encrypt and Decrypt Data for CBC mode Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate and store the AES 128 Symmetric key in OPTIGA Key store OID(E200)");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Encrypt the plain data with CBC mode");
    OPTIGA_SHELL_LOG_MESSAGE("3 Step: Decrypt the encrypted data from step 2");
    example_optiga_crypt_symmetric_encrypt_decrypt_cbc();
}

static void optiga_shell_crypt_symmetric_encrypt_cbcmac(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting symmetric Encrypt Data for CBCMAC mode Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate and store the AES 128 Symmetric key in OPTIGA Key store OID(E200)");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Encrypt the plain data with CBCMAC mode");
    example_optiga_crypt_symmetric_encrypt_cbcmac();
}

static void optiga_shell_crypt_hmac(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting HMAC-SHA256 generation Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Change metadata for OID(0xF1D0) as Execute access condition = Always and Data object type  =  Pre-shared secret");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Generate HMAC");
    example_optiga_crypt_hmac();
}

static void optiga_shell_crypt_hkdf(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting HKDF-SHA256 key derivation Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Write the shared secret to the Arbitrary data object F1D0");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Change metadata of OID(0xF1D0) Data object type  =  Pre-shared secret");
    OPTIGA_SHELL_LOG_MESSAGE("3 Step: Derive HKDF");
    example_optiga_crypt_hkdf();
}

static void optiga_shell_crypt_symmetric_generate_key(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting generation of symmetric AES-128 key");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Generate symmetric AES-128 key and store it in OID(E200)");
    example_optiga_crypt_symmetric_generate_key();
}

static void optiga_shell_crypt_hmac_verify_with_authorization_reference(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting HMAC verify with authorization reference Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Get the User Secret and store it in OID(0xF1D0)");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Set the metadata of 0xF1E0 to Auto with 0xF1D0");
    OPTIGA_SHELL_LOG_MESSAGE("3 Step: Generate authorization code with optional data");
    OPTIGA_SHELL_LOG_MESSAGE("4 Step: Calculate HMAC on host using mbedtls");
    OPTIGA_SHELL_LOG_MESSAGE("5 Step: Perform HMAC verification");
    example_optiga_hmac_verify_with_authorization_reference();
}

static void optiga_shell_crypt_clear_auto_state(void)
{
    OPTIGA_SHELL_LOG_MESSAGE("Starting clear auto state Example");
    OPTIGA_SHELL_LOG_MESSAGE("1 Step: Change metadata of OID(0xF1D0) Data object type  =  Pre-shared secret");
    OPTIGA_SHELL_LOG_MESSAGE("2 Step: Get the User Secret and store it in OID(0xF1D0)");
    OPTIGA_SHELL_LOG_MESSAGE("3 Step: Generate auth code with optional data");
    OPTIGA_SHELL_LOG_MESSAGE("4 Step: Calculate HMAC on host using mbedtls");
    OPTIGA_SHELL_LOG_MESSAGE("5 Step: Perform HMAC verification");
    OPTIGA_SHELL_LOG_MESSAGE("6 Step: Perform clear auto state");
    example_optiga_crypt_clear_auto_state();
}

#define PRINT_PERFORMANCE_RESULTS(TESTCASE) \
		timestamp = pal_os_timer_get_time_in_milliseconds(); \
		TESTCASE(); \
		sprintf(buffer_string, "Example with pre/post steps takes %d msec", (int) pal_os_timer_get_time_in_milliseconds() - timestamp);\
		OPTIGA_SHELL_LOG_MESSAGE(buffer_string); \
		optiga_lib_print_string_with_newline(""); \
		pal_os_timer_delay_in_milliseconds(2000);

static void optiga_shell_selftest()
{
	char buffer_string[60];
	int timestamp = pal_os_timer_get_time_in_milliseconds();

	PRINT_PERFORMANCE_RESULTS(optiga_shell_init);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_util_read_data);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_util_write_data);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_util_read_coprocessor_id);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_pair_host_optiga);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_util_update_count);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_util_protected_update);

	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_hash);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_hash_data);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_tls_prf_sha256);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_random);

	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_ecc_generate_keypair);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_ecdsa_sign);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_ecdsa_verify);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_ecdh);

	PRINT_PERFORMANCE_RESULTS(optiga_shell_rsa_generate_keypair);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_rsa_sign);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_rsa_verify);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_rsa_encrypt_message);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_rsa_encrypt_session);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_rsa_decrypt_and_store);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_rsa_decrypt_and_export);

	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_symmetric_encrypt_decrypt_ecb);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_symmetric_encrypt_decrypt_cbc);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_symmetric_encrypt_cbcmac);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_hmac);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_hkdf);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_symmetric_generate_key);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_clear_auto_state);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_crypt_hmac_verify_with_authorization_reference);
	PRINT_PERFORMANCE_RESULTS(optiga_shell_deinit);
}


static void optiga_shell_show_usage();


optiga_example_cmd_t optiga_cmds [] =
{
		{"",                                        	    "help",				optiga_shell_show_usage},
		{"    initialize optiga                        : "OPTIGA_SHELL,"init",			optiga_shell_init},
		{"    de-initialize optiga                     : "OPTIGA_SHELL,"deinit",		optiga_shell_deinit},
		{"    run all tests at once                    : "OPTIGA_SHELL,"selftest",		optiga_shell_selftest},
		{"    read data                                : "OPTIGA_SHELL,"readdata",		optiga_shell_util_read_data},
		{"    write data                               : "OPTIGA_SHELL,"writedata",	   	optiga_shell_util_write_data},
		{"    read coprocessor id                      : "OPTIGA_SHELL,"coprocid",		optiga_shell_util_read_coprocessor_id},

		{"    binding host with optiga                 : "OPTIGA_SHELL,"bind",			optiga_shell_pair_host_optiga},
		{"    hibernate and restore                    : "OPTIGA_SHELL,"hibernate",		optiga_shell_util_hibernate_restore},
		{"    update counter                           : "OPTIGA_SHELL,"counter",		optiga_shell_util_update_count},
		{"    protected update                         : "OPTIGA_SHELL,"protected",		optiga_shell_util_protected_update},

		{"    hashing of data                          : "OPTIGA_SHELL,"hash",			optiga_shell_crypt_hash},
		{"    hash single function                     : "OPTIGA_SHELL,"hashsha256",		optiga_shell_crypt_hash_data},
		{"    tls pfr sha256                           : "OPTIGA_SHELL,"prf",			optiga_shell_crypt_tls_prf_sha256},
		{"    random number generation                 : "OPTIGA_SHELL,"random",		optiga_shell_crypt_random},

		{"    ecc key pair generation                  : "OPTIGA_SHELL,"ecckeygen",		optiga_shell_crypt_ecc_generate_keypair},
		{"    ecdsa sign                               : "OPTIGA_SHELL,"ecdsasign",		optiga_shell_crypt_ecdsa_sign},
		{"    ecdsa verify sign                        : "OPTIGA_SHELL,"ecdsaverify",		optiga_shell_crypt_ecdsa_verify},
		{"    ecc diffie hellman                       : "OPTIGA_SHELL,"ecdh",			optiga_shell_crypt_ecdh},

		{"    rsa key pair generation                  : "OPTIGA_SHELL,"rsakeygen",		optiga_shell_rsa_generate_keypair},
		{"    rsa sign                                 : "OPTIGA_SHELL,"rsasign",		optiga_shell_crypt_rsa_sign},
		{"    rsa verify sign                          : "OPTIGA_SHELL,"rsaverify",		optiga_shell_crypt_rsa_verify},
		{"    rsa encrypt message                      : "OPTIGA_SHELL,"rsaencmsg",		optiga_shell_crypt_rsa_encrypt_message},
		{"    rsa encrypt session                      : "OPTIGA_SHELL,"rsaencsession",		optiga_shell_crypt_rsa_encrypt_session},
		{"    rsa decrypt and store                    : "OPTIGA_SHELL,"rsadecstore",		optiga_shell_crypt_rsa_decrypt_and_store},
		{"    rsa decrypt and export                   : "OPTIGA_SHELL,"rsadecexp",		optiga_shell_crypt_rsa_decrypt_and_export},

		{"    symmetric ecb encrypt and decrypt        : "OPTIGA_SHELL,"ecbencdec",	 	optiga_shell_crypt_symmetric_encrypt_decrypt_ecb},
		{"    symmetric cbc encrypt and decrypt        : "OPTIGA_SHELL,"cbcencdec",		optiga_shell_crypt_symmetric_encrypt_decrypt_cbc},
		{"    symmetric cbcmac encrypt                 : "OPTIGA_SHELL,"cbcmacenc",		optiga_shell_crypt_symmetric_encrypt_cbcmac},
		{"    hmac-sha256 generation                   : "OPTIGA_SHELL,"hmac",			optiga_shell_crypt_hmac},
		{"    hkdf-sha256 key derivation               : "OPTIGA_SHELL,"hkdf",			optiga_shell_crypt_hkdf},
		{"    generate symmetric aes-128 key           : "OPTIGA_SHELL,"aeskeygen",		optiga_shell_crypt_symmetric_generate_key},
		{"    clear auto state                         : "OPTIGA_SHELL,"clrautostate",		optiga_shell_crypt_clear_auto_state},
		{"    hmac verify                              : "OPTIGA_SHELL,"hmacverify",		 optiga_shell_crypt_hmac_verify_with_authorization_reference},
};

#define OPTIGA_SIZE_OF_CMDS			(sizeof(optiga_cmds)/sizeof(optiga_example_cmd_t))

static void optiga_shell_show_usage()
{
	uint8_t number_of_cmds = OPTIGA_SIZE_OF_CMDS;
	uint8_t index = 0;
	optiga_example_cmd_t * current_cmd;
	optiga_lib_print_string_with_newline("");
	optiga_lib_print_string_with_newline("    USAGE : optiga -<cmd>");
	optiga_lib_print_string_with_newline("");
	for(index = 0; index < number_of_cmds; index++)
	{
		current_cmd = &optiga_cmds[index];
		if(0 != strcmp("help",current_cmd->cmd_options))
		{
			optiga_lib_print_string(current_cmd->cmd_description);
			optiga_lib_print_string_with_newline(current_cmd->cmd_options);
		}
	}
	optiga_lib_print_string_with_newline("");
	optiga_lib_print_string_with_newline("Initialize OPTIGA before the 1st crypto functionality (not required for self-test)");

}

static void optiga_shell_trim_cmd(char_t * user_cmd)
{
	char_t* i = user_cmd;
	char_t* j = user_cmd;
	while(*j != 0)
	{
		*i = *j++;
		if(*i != ' ')
			i++;
	}
	*i = 0;
	if(strlen(user_cmd)>strlen(OPTIGA_SHELL))
	{
		strcpy(user_cmd,user_cmd+strlen(OPTIGA_SHELL)-1);
	}
}

#include <stdio.h>
static void optiga_shell_execute_example(char_t * user_cmd)
{
	uint8_t number_of_cmds = OPTIGA_SIZE_OF_CMDS;
	uint8_t index = 0,cmd_found = 0;
	optiga_example_cmd_t * current_cmd;

	do
	{
		if (0 != strncmp(user_cmd,OPTIGA_SHELL,9))
		{
		    break;
		}
		optiga_shell_trim_cmd(user_cmd);

		for(index = 0; index < number_of_cmds; index++)
		{
			current_cmd = &optiga_cmds[index];
			if((0 == strcmp(user_cmd,current_cmd->cmd_options)))
			{
				if(NULL != current_cmd->cmd_handler)
				{
					optiga_lib_print_string_with_newline("");
					current_cmd->cmd_handler();
					optiga_lib_print_string_with_newline("");
					cmd_found = 1;
					break;
				}
				else
				{
					optiga_lib_print_string_with_newline("No example exists for this request");
					break;
				}
			}
		}

	}while(FALSE);

	if(cmd_found == 0)
	{
		optiga_lib_print_string_with_newline("");
        optiga_lib_print_message("Wrong command issued!", "[error] : ", OPTIGA_LIB_LOGGER_COLOR_LIGHT_RED);
		optiga_lib_print_string_with_newline("Please try again or type help for list of commands");
		optiga_lib_print_string_with_newline("");
	}

}

static void optiga_shell_show_prompt()
{
	optiga_lib_print_string_with_newline(">>>");
}

void optiga_shell_begin(void)
{
	uint8_t ch = 0;
	char_t user_cmd[50];
	uint8_t index = 0;

	optiga_shell_show_usage();
	optiga_lib_print_string_with_newline("");
	optiga_shell_show_prompt();

	while(TRUE)
	{
		if (0 == optiga_shell_pal_logger_read(&logger_console,&ch,1))
		{
			/*
			 * Check if carriage return \r or line feed \n is received, indicating the end of command input
			 * */
			if(ch == (uint8_t)'\r' || ch == (uint8_t)'\n')
			{
				if(index != 0)
				{
					user_cmd[index++] = 0;
					index = 0;
					optiga_shell_show_prompt();
					/*
					 * start cmd parsing
					 */
					optiga_shell_execute_example((char_t * )&user_cmd);
					optiga_lib_print_string_with_newline("");
					optiga_shell_show_prompt();
				}
				else
				{
					/* If index is 0, it means the serial console (E.g. Tera Term) sends both \r and \n
					 * In this case, ignore the second character received and move the command pointer back to beginning
					 * as we only need to break at \r or \n
					 */
					user_cmd[index] = 0;
				}
			}
			else
			{
				/*
				 * keep adding
				 */
				pal_logger_write(&logger_console, &ch, 1);
				user_cmd[index++] = ch;
			}
		}
	}
}

void optiga_shell_wait_for_user(void)
{
	pal_status_t pal = PAL_STATUS_FAILURE;
	uint8_t ch = 0;

	while(1U)
	{
		optiga_lib_print_string_with_newline(" Please press ENTER key to start optiga mini shell");
		pal_os_timer_delay_in_milliseconds(2000);

		pal = optiga_shell_pal_logger_read(&logger_console,&ch,1);

		if(pal == PAL_STATUS_SUCCESS)
		{
			break;
		}
	}
}
