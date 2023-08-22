# OPTIGA™ Shell Interface using PSoC™ 6

_Disclaimer: This is a community code example (CCE) released for the benefit of the community users. These projects have only been tested for the listed BSPs, tools versions, and toolchains documented in this readme. They are intended to demonstrate how a solution / concept / use-case can be achieved on a particular device. For official code examples, please click [here](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software)._

This example uses an OPTIGA™ Trust M V3 security solution on a PSoC&trade; 6 MCU to execute example code to perform available crypto operations on the secure element. This example outputs the result and the time taken to perform the crypto operations in a UART terminal.



## Requirements

- [ModusToolbox™ software](https://www.cypress.com/products/modustoolbox) v2.4 

   **Note**: This code example version requires ModusToolbox™ software version 2.4 and may not be compatible with other versions.
- Board support package (BSP) minimum required version: 2.0.0
- Programming language: C
- Associated parts:
  - All [PSoC™ 6 MCU](http://www.cypress.com/PSoC6) parts
  - [OPTIGA™ Trust M V3](http://www.infineon.com/optiga-trust-m)


## Supported kits (make variable 'TARGET')

- [OPTIGA™ Trust M IoT Security Development Kit](https://www.infineon.com/cms/en/product/evaluation-boards/optiga-trust-m-iot-kit/) (`CYSBSYSKIT-DEV-01`) - Default value of `TARGET`
- [PSoC&trade; 62S2 evaluation kit](https://www.cypress.com/CY8CEVAL-062S2) (`CY8CEVAL-062S2`, `CY8CEVAL-062S2-CYW943439M2IPA1`, `CY8CEVAL-062S2-LAI-4373M2`)


## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.


## Software setup

Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).


## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade; software</b></summary>

1. Click the **New application** link in the **quick panel** (or, use **File** > **New** > **ModusToolbox&trade; application**). This launches the [project creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcabbd31e5) tool.

2. Pick a kit supported by the code example from the list shown in the **Project creator - Choose board support package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [library manager](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Library_Manager_User_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99ab34b831ce) to choose the BSP for the supported kit. You can use the library manager to select or update the BSP and firmware libraries used in this application. To access the library manager, click the link from the **quick panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work. Refer to the following [blog](https://community.infineon.com/t5/Blogs/Interfacing-OPTIGA-Trust-M-with-a-PSoC-6-device/ba-p/361647) for detailed steps on how to import an OPTIGA™ Trust M application for various kits.

3. In the **Project creator - Select application** dialog, choose the example from **Community Code Examples** by enabling the checkbox.

4. (Optional) Change the suggested **New application name**.

5. The **Application(s) root path** defaults to the Eclipse workspace which is usually the desired location for the application. If you want to store the application in a different location, you can change the *Application(s) root path* value. Applications that share libraries should be in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632) (locally available at *{ModusToolbox&trade; software install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; software provides the project creator as both a GUI tool and the command line tool, "project-creator-cli". The CLI tool can be used to create applications from a CLI terminal or from within batch files or shell scripts. This tool is available in the *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/* directory.

Use a CLI terminal to invoke the "project-creator-cli" tool. On Windows, use the command line "modus-shell" program provided in the ModusToolbox&trade; software installation instead of a standard Windows command-line application. This shell provides access to all ModusToolbox&trade; software tools. You can access it by typing `modus-shell` in the search box in the Windows menu. In Linux and macOS, you can use any terminal application.

This tool has the following arguments:

Argument | Description | Required/optional
---------|-------------|-----------
`--board-id` | Defined in the `<id>` field of the [BSP](https://github.com/Infineon?q=bsp-manifest&type=&language=&sort=) manifest | Required
`--app-id`   | Defined in the `<id>` field of the [CE](https://github.com/Infineon?q=ce-manifest&type=&language=&sort=) manifest | Required
`--target-dir`| Specify the directory in which the application is to be created if you prefer not to use the default current working directory | Optional
`--user-app-name`| Specify the name of the application if you prefer to have a name other than the example's default name | Optional

<br>

The following example will clone the "[OPTIGA™ Shell Interface using PSoC™ 6](https://github.com/Infineon/cce-mtb-psoc6-optiga-shell)" application with the desired name "OptigaShell" configured for the *CYSBSYSKIT-DEV-01* BSP into the specified working directory, *C:/mtb_projects*:

   ```
   project-creator-cli --board-id CYSBSYSKIT-DEV-01 --app-id cce-mtb-psoc6-optiga-shell --user-app-name OptigaShell --target-dir "C:/mtb_projects"
   ```

**Note:** The project-creator-cli tool uses the `git clone` and `make getlibs` commands to fetch the repository and import the required libraries. For details, see the "Project creator tools" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In third-party IDEs</b></summary>

Use one of the following options:

- **Use the standalone [project creator](https://www.infineon.com/dgdl/Infineon-ModusToolbox_Project_Creator_Guide_3-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7d718a49017d99bcabbd31e5) tool:**

   1. Launch the project creator from the Windows Start menu or from *{ModusToolbox&trade; software install directory}/tools_{version}/project-creator/project-creator.exe*.

   2. In the initial **Choose board support package** screen, select the BSP, and click **Next**.

   3. In the **Select application** screen, select the appropriate IDE from the **Target IDE** drop-down menu.

   4. Click **Create** and follow the instructions printed in the bottom pane to import or open the exported project in the respective IDE.

<br>

- **Use command-line interface (CLI):**

   1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and then import the libraries using the `make getlibs` command.

   2. Export the application to a supported IDE using the `make <ide>` command.

   3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632) (locally available at *{ModusToolbox&trade; software install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>


## Operation

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud. Enable the local echo to see the input commands.

3. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade; software</b></summary>

      1. Select the application project in the project explorer.

      2. In the **quick panel**, scroll down, and click **\<Application name> Program (KitProg3_MiniProg4)**.
   </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. The default toolchain and target are specified in the application's Makefile but you can override those values manually:
      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:
      ```
      make program TARGET=CYSBSYSKIT-DEV-01 TOOLCHAIN=GCC_ARM
      ```

4. After programming, the application starts automatically. A prompt appears to begin the shell operation.

   **Figure 1. Terminal output on program startup**

   ![](images/prompt_begin.png)

5. Press ENTER to see the list of supported commands, command usage and their corresponding functions.

   **Figure 2. List of shell commands**

   ![](images/commands_list.png)

6. Always begin with OPTIGA™ Trust device initialization, by typing the command ***optiga --init*** and press ENTER.

   **Figure 3. Initalization of OPTIGA™ Trust device**

   ![](images/optiga_init.png)

7. After successful initialization, any function from the list of commands can be executed by typing the corresponding command.<br>
   E.g. For hashing of data, type the command ***optiga --hash*** and press ENTER.

   **Figure 4. Hashing of data**

   ![](images/optiga_hash.png)

8. In case a wrong command is given, an error message is shown.<br>
   E.g. Type a random command which is not in the list of commands, ***optiga --hello*** and press ENTER.

   **Figure 5. Wrong command error**

   ![](images/optiga_error_cmd.png)


## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application name> Debug (KitProg3_MiniProg4)** configuration in the **quick panel**. For more details, see the "Program and debug" section in the [Eclipse IDE for ModusToolbox&trade; software user guide](https://www.infineon.com/dgdl/Infineon-ModusToolbox_2.4_User_Guide-UserManual-v01_00-EN.pdf?fileId=8ac78c8c7e7124d1017ed97e72563632).

**Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice - once before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.cypress.com/docs/DOC-21143) to learn about this and for the workaround.


## Design and implementation

This example initializes the OPTIGA™ Trust device by calling a dedicated function from the platform abstraction layer (PAL). This function initializes all hardware and system-relevant components. Afterward, the example sequentially calls isolated sub-examples to showcase various API combinations.

All examples follow the same pattern `init` function, `do_crypto` function, and an optional `deinit` function. The following snippets provide an overview:

```c
void example_main(void)
{
   uint8_t random_data_buffer [32];
   optiga_crypt_t * me_crypt_instance = NULL;
   optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;

   do
   {
      /*
      * Step 1. Initialize the OPTIGA device
      */
      example_optiga_init();

      /*
      * Step 2. Create an instance of optiga_crypt to perform any crypto operation on the OPTIGA device.
      */
      me_crypt_instance = optiga_crypt_create(0, optiga_lib_callback, NULL);
      if (NULL == me_crypt_instance)
      {
            break;
      }

      /*
      * Step 3..n-1. Perform any operation with the OPTIGA device; e.g., get a true random number.
      * random_data_buffer should contain the resulting random number.
      */
      optiga_lib_status = OPTIGA_LIB_BUSY;
      return_status = optiga_crypt_random(me_crypt_instance,
                                          OPTIGA_RNG_TYPE_TRNG,
                                          random_data_buffer,
                                          sizeof(random_data_buffer));

      WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

      /* Step n. De-initialize the OPTIGA device. This step is optional in many cases, because some resources on MCU-based systems cannot be really de-initialized.
      */
      example_optiga_deinit();
   } while (FALSE);
} 
```

OPTIGA™ `init` and `deinit` functions simply allocate a new command context and send an `OpenApplication`/`CloseApplication` command to the chip. The `while` loop is required to synchronize the state machine. The application is free to implement this differently and check the status occasionally; the rest might be in an idle state.

```c
#include "optiga/optiga_crypt.h"
#include "optiga/optiga_util.h"
#include "optiga/optiga_lib_config.h"


optiga_util_t * me_util_instance = NULL;
optiga_crypt_t * me_crypt_instance = NULL;

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_lib_callback(void * context, optiga_lib_status_t return_status)
{
   optiga_lib_status = return_status;
   if (NULL != context)
   {
      // callback to upper layer here
   }
}

// Check return status
#define WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status)\
                            if (OPTIGA_LIB_SUCCESS != return_status)\
                            {\
                                break;\
                            }\
                            while (OPTIGA_LIB_BUSY == optiga_lib_status)\
                            { }\
                            if (OPTIGA_LIB_SUCCESS != optiga_lib_status)\
                            {\
                                return_status = optiga_lib_status;\
                                break;\
                            }

void example_optiga_init(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;

    do
    {
        if (NULL == me_util_instance)
        {
           /*
           Create an instance of optiga_util to open the application on the OPTIGA device.
           */
           me_util_instance = optiga_util_create(0, optiga_lib_callback, NULL);
           if (NULL == me_util_instance)
           {
               break;
           }
        }

        /**
         * Open the application on the OPTIGA device which is a precondition to perform any other operations
         * using the optiga_util_open_application.
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_open_application(me_util_instance, 0);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

    }while(FALSE);
}

void example_optiga_deinit(void)
{
    optiga_lib_status_t return_status = !OPTIGA_LIB_SUCCESS;

    do
    {
        /**
         * Close the application on the OPTIGA device after all the operations are executed
         * using optiga_util_close_application.
         */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_close_application(me_util_instance, 0);

        WAIT_AND_CHECK_STATUS(return_status, optiga_lib_status);

        // Destroy util and crypt instances
        optiga_util_destroy(me_util_instance);
        me_util_instance = NULL;
    }while(FALSE);
}
```

<br />

### Resources and settings

There are several settings which can be controlled:

| OPTIGA™ Trust M pins | Assigned GPIOs by default | Notes |
| ------ | ------ | ------ |
| I2C SDA (I/O) | CYBSP_TRUSTM_I2C_SDA | Any GPIO connected to the I2C SDA line can be used. |
| I2C SCL (Clock) | CYBSP_TRUSTM_I2C_SCL | Any GPIO connected to the I2C SDA line can be used. |
| RST (Reset) | CYBSP_TRUSTM_RST | An optional control pin if defined in optiga_lib_config_mtb.h |
| VDD (Power control) | (Optional) CYBSP_TRUSTM_VDD | An optional control pin if defined in optiga_lib_config_mtb.h |

| optiga_lib_config_mtb.h macros | Meaning | Default value |
| ------ | ------ | ------ |
| `OPTIGA_CRYPT_XXXX` | Controls whether to enable/disable selected crypto support on the host library side | All are enabled |
| `OPTIGA_COMMS_SHIELDED_CONNECTION` and `OPTIGA_COMMS_DEFAULT_PROTECTION_LEVEL` | Together define whether to use and the extent of use of the shielded connection (encrypted and integrity-protected I2C communication) | Defined `OPTIGA_COMMS_SHIELDED_CONNECTION` |
| `OPTIGA_COMMS_DEFAULT_RESET_TYPE` | The reset type if VDD or RST pins are defined. Choose 1 or 2 depending on the combination used. VDD can be used in certain cases as a reset line, but it is recommended to use them separately. | 2 |
| `OPTIGA_CMD_MAX_REGISTRATIONS` | Controls the number of crypt/util registrations allowed. In a very basic scenario, this can be reduced to 2 (one registration each for crypt and util) | 6 |
| `OPTIGA_MAX_COMMS_BUFFER_SIZE` | Maximum buffer size that the command layer should be able to store intermediately | 0x615 |
| `OPTIGA_LIB_ENABLE_LOGGING` | Controls whether logging can be enabled in general | Defined |
| `OPTIGA_LIB_ENABLE_UTIL_LOGGING` | If defined together with `OPTIGA_LIB_ENABLE_LOGGING`, outputs util API-relevant messages | Undefined |
| `OPTIGA_LIB_ENABLE_CRYPT_LOGGING` | If defined together with `OPTIGA_LIB_ENABLE_LOGGING`, outputs crypt API-relevant messages | Undefined |
| `OPTIGA_LIB_ENABLE_CMD_LOGGING` | If defined together with `OPTIGA_LIB_ENABLE_LOGGING`, outputs APDU sent to the OPTIGA™ Trust M external interface (See the solution reference manual) | Undefined |
| `OPTIGA_LIB_ENABLE_COMMS_LOGGING` | If defined together with `OPTIGA_LIB_ENABLE_LOGGING`, prints out I2C frames | Undefined |


<br />
<br />

## Related resources

| Resources | Links |
| ------ | ------ |
| Application notes | [AN228571](https://www.infineon.com/dgdl/Infineon-AN228571_Getting_started_with_PSoC_6_MCU_on_ModusToolbox_software-ApplicationNotes-v06_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d36de1f66d1&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-an_vanitylink) – Getting started with PSoC™ 6 MCU on ModusToolbox™ software <br />[AN221774](https://www.infineon.com/dgdl/Infineon-AN221774_Getting_Started_with_PSoC_6_MCU_on_PSoC_Creator-ApplicationNotes-v07_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d357e356627&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-an_vanitylink) – Getting started with PSoC™ 6 MCU on PSoC™ Creator <br />[AN210781](https://www.infineon.com/dgdl/Infineon-AN210781_Getting_Started_with_PSoC_6_MCU_with_Bluetooth_Low_Energy_(BLE)_Connectivity_on_PSoC_Creator-ApplicationNotes-v05_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d311f536528&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-an_vanitylink) – Getting started with PSoC™ 6 MCU with Bluetooth® Low Energy connectivity on PSoC™ Creator <br /> [AN215656](https://www.infineon.com/dgdl/Infineon-AN215656_PSoC_6_MCU_Dual-CPU_System_Design-ApplicationNotes-v09_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d3180c4655f&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-an_vanitylink) – PSoC™ 6 MCU: Dual-CPU system design |
| Code examples on GitHub | [Using ModusToolbox™ software](https://github.com/Infineon/Code-Examples-for-ModusToolbox-Software) |
| Device documentation | [PSoC™ 6 MCU datasheets](https://documentation.infineon.com/html/psoc6/bnm1651211483724.html) <br />[PSoC™ 6 technical reference manuals](https://www.infineon.com/cms/en/product/microcontroller/32-bit-psoc-arm-cortex-microcontroller/psoc-6-32-bit-arm-cortex-m4-mcu/?utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-doc_search#!details) <br /> [OPTIGA™ Trust M datasheet](https://www.infineon.com/cms/en/product/security-smart-card-solutions/optiga-embedded-security-solutions/optiga-trust/optiga-trust-m-sls32aia/#!?fileId=5546d4626c1f3dc3016c853c271a7e4a) |
| Development kits | Visit [www.infineon.com/design-support](https://www.infineon.com/cms/en/design-support/) and use the **Evaluation Boards & Kits** section to filter kits by Product family or Features. |
| Libraries on GitHub | [mtb-pdl-cat1](https://github.com/Infineon/mtb-pdl-cat1) – PSoC™ 6 peripheral driver library (PDL) <br /> [mtb-hal-cat1](https://github.com/Infineon/mtb-hal-cat1) – Hardware abstraction layer (HAL) library <br /> [retarget-io](https://github.com/Infineon/retarget-io) – Utility library to retarget STDIO messages to a UART port |
| Middleware on GitHub | [optiga-trust-m](https://github.com/Infineon/optiga-trust-m) – OPTIGA™ Trust M library and documents <br /> [capsense](https://github.com/Infineon/capsense) – CAPSENSE™ library and documents <br /> [psoc6-middleware](https://github.com/Infineon/modustoolbox-software#psoc-6-middleware-libraries) – Links to all PSoC™ 6 MCU middleware |
| Tools | [Eclipse IDE for ModusToolbox™ software](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/) – ModusToolbox™ software is a collection of easy-to-use software and tools enabling rapid development with Infineon MCUs, covering applications from embedded sense and control to wireless and cloud-connected systems using AIROC™ Wi-Fi and Bluetooth® connectivity devices. |

<br />

## Other resources

Infineon provides a wealth of data at [www.infineon.com](https://www.infineon.com/) to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC™ 6 MCU devices, see [How to design with PSoC™ 6 MCU - KBA223067](https://community.infineon.com/t5/Knowledge-Base-Articles/How-to-Design-with-PSoC-6-MCU-KBA223067/ta-p/248857) in the Cypress community.

<br />

## Document History

Document title: CCE237273 - OPTIGA™ Shell Interface using PSoC™ 6

| Version | Description of change |
| ------ | ------ |
| 1.0.0 | New community code example |

## 

© Cypress Semiconductor Corporation, 2016-2021. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates (“Cypress”). This document, including any software or firmware included or referenced in this document (“Software”), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide. Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights. If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products. Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.

TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. No computing device can be absolutely secure. Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, “Security Breach”). Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach. In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes. It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product. “High-Risk Device” means any device or system whose failure could cause personal injury, death, or property damage. Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices. “Critical Component” means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness. Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.

Cypress, the Cypress logo, and combinations thereof, PSoC, CapSense, EZ-USB, F-RAM, Traveo, WICED, and ModusToolbox are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit cypress.com. Other names and brands may be claimed as property of their respective owners.
