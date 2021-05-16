/*
 * stm32main.c
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include "main.h"
#include "components/ad5280_driver.h"
#include "components/mcp4725_driver.h"
#include "components/i2c_lib.h"

struct CV_Configuration_S cvConfiguration;
struct CA_Configuration_S caConfiguration;   //esto es para la chronoamperimetry
struct Data_S data;

void setup(struct Handles_S *handles) {
    MASB_COMM_S_setUart(handles->huart);
    MASB_COMM_S_waitForMessage();

    //=========================== Potenciometro ====================================
    // Solo debemos de ejecutar este codigo una unica vez al inicio del programa.
    // Fijaremos una resistencia fija para siempre y no la modificaremos.

    // Creamos el handle de la libreria.
    AD5280_Handle_T hpot = NULL;

    hpot = AD5280_Init();

    // Configuramos su direccion I2C de esclavo, su resistencia total (hay
    // diferentes modelos; este tiene 50kohms) e indicamos que funcion queremos que
    // se encargue de la escritura a traves del I2C. Utilizaremos la funcion
    // I2C_Write de la libreria i2c_lib.
    AD5280_ConfigSlaveAddress(hpot, 0x2C);
    AD5280_ConfigNominalResistorValue(hpot, 50e3f);
    AD5280_ConfigWriteFunction(hpot, I2C_Write);

    // Fijamos la resistencia de, por ejemplo, 12kohms.
    AD5280_SetWBResistance(hpot, 12e3f));


    //================================== DAC =======================================
    // Esto lo realizaremos una unica vez al inicio del programa. ------------------

    // Creamos el handle de la libreria.
    MCP4725_Handle_T hdac = NULL;

    hdac = MCP4725_Init();

    // Configuramos la direccion I2C de esclavo, su tension de referencia (que es la
    // de alimentacion) e indicamos que funcion queremos que se encargue de la
    // escritura a traves del I2C. Utilizaremos la funcion I2C_Write de la libreria
    // i2c_lib.
    MCP4725_ConfigSlaveAddress(hdac, 0x66);
    MCP4725_ConfigVoltageReference(hdac, 4.0f);
    MCP4725_ConfigWriteFunction(hdac, I2C_Write);

}

void loop(void) {
    if (MASB_COMM_S_dataReceived()) { // Si se ha recibido un mensaje...

 		switch(MASB_COMM_S_command()) { // Miramos que comando hemos recibido

 			case START_CV_MEAS: // Si hemos recibido START_CV_MEAS

                 // Leemos la configuracion que se nos ha enviado en el mensaje y
                 // la guardamos en la variable cvConfiguration
				cvConfiguration = MASB_COMM_S_getCvConfiguration();


 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

 				// Aqui iria todo el codigo de gestion de la medicion que hareis en el proyecto
                // si no quereis implementar el comando de stop.

 				break;

			case STOP_MEAS: // Si hemos recibido STOP_MEAS

 				/*
 				 * Debemos de enviar esto desde CoolTerm:
 				 * 020300
 				 */
 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

 				// Aqui iria el codigo para tener la medicion si implementais el comando stop.

 				break;

 			default: // En el caso que se envia un comando que no exista


 				break;

 		}

       // Una vez procesado los comando, esperamos el siguiente
 		MASB_COMM_S_waitForMessage();

 	}


}
