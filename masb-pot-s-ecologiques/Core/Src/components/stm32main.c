/*
 * stm32main.c
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#include "components/stm32main.h"
#include "components/chronoamperometry.h"
#include "components/i2c_lib.h"
#include "components/ad5280_driver.h"
#include "main.h"
#include "components/masb_comm_s.h"
#include "components/mcp4725_driver.h"

// creamos las estructuras que necesitaremos
struct CV_Configuration_S cvConfiguration;
struct CA_Configuration_S caConfiguration;
struct Data_S data;
MCP4725_Handle_T hdac;

void setup(struct Handles_S *handles) { // el tramo que solo se repetirá una vez
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET); //
	HAL_Delay(500);

	I2C_init(handles->hi2c);


	AD5280_Handle_T hpot = NULL;
	hpot = AD5280_Init();

	AD5280_ConfigSlaveAddress(hpot, 0x2C);
	AD5280_ConfigNominalResistorValue(hpot, 50e3f);
	AD5280_ConfigWriteFunction(hpot, I2C_write);
	AD5280_SetWBResistance(hpot, 50e3f);

	hdac = MCP4725_Init();

	// Configuramos la direccion I2C de esclavo, su tension de referencia (que es la
	// de alimentacion) e indicamos que funcion queremos que se encargue de la
	// escritura a traves del I2C. Utilizaremos la funcion I2C_Write de la libreria
	// i2c_lib.

	MCP4725_ConfigSlaveAddress(hdac, 0x66);
	MCP4725_ConfigVoltageReference(hdac, 4.0f);
	MCP4725_ConfigWriteFunction(hdac, I2C_write);

	// Esto lo ejecutamos cada vez que queremos generar una tension nueva. ---------
	MCP4725_SetOutputVoltage(hdac, 0.0f);

	MASB_COMM_S_waitForMessage(); // esperamos a recibir un mensaje
}

void loop(void) { // seccion que se repetirá en bucle

    if (MASB_COMM_S_dataReceived()) { // Si se ha recibido un mensaje...

 		switch(MASB_COMM_S_command()) { // Miramos que comando hemos recibido

 			case START_CV_MEAS: // Si hemos recibido START_CV_MEAS

                 // Leemos la configuracion que se nos ha enviado en el mensaje y
                 // la guardamos en la variable cvConfiguration
				cvConfiguration = MASB_COMM_S_getCvConfiguration();

				Mesurant_CV(cvConfiguration); // función que permite medir la voltametria cíclica

 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

 				break;

 			case START_CA_MEAS: // si hemos recibido la instrucción de la cronoamperometria

 				caConfiguration = MASB_COMM_S_getCaConfiguration(); // Leemos la configuracion que se nos ha enviado y la guardamos

 				Mesurant_Crono(caConfiguration); // aplicamos la configuracion a la función que permite medir la cronoamperometría

 				__NOP();

 				break;

			case STOP_MEAS: // Si hemos recibido STOP_MEAS


 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint


 				break;

 			default: // En el caso que se envia un comando que no exista


 				break;

 		}

       // Una vez procesado los comando, esperamos el siguiente
 		MASB_COMM_S_waitForMessage();

 	}


}


