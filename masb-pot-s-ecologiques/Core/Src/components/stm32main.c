/*
 * stm32main.c
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#include "components/stm32main.h"
#include "components/i2c_lib.h"
#include "components/ad5280_driver.h"
#include "main.h"
#include "components/masb_comm_s.h"

struct CV_Configuration_S cvConfiguration;
struct CA_Configuration_S caConfiguration;
struct Data_S data;

void setup(struct Handles_S *handles) {
	HAL_GPIO_Write_Oin(EN_GPIO_Port, EN_Pin,GPIO_PIN_RESET);

	AD5280_Handle_T hpot = NULL;
	hpot = AD5280_Init();

	AD5280_ConfigSlaveAddress(hpot, 0x2C);
	AD5280_ConfigNominalResistorValue(hpot, 50e3f);
	AD5280_ConfigWriteFunction(hpot, I2C_Write);
}

void loop(void) {
    if (MASB_COMM_S_dataReceived()) { // Si se ha recibido un mensaje...

 		switch(MASB_COMM_S_command()) { // Miramos que comando hemos recibido

 			case START_CV_MEAS: // Si hemos recibido START_CV_MEAS

                 // Leemos la configuracion que se nos ha enviado en el mensaje y
                 // la guardamos en la variable cvConfiguration
				cvConfiguration = MASB_COMM_S_getCvConfiguration();


 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

 				// Aqui iria tod el codigo de gestion de la medicion que hareis en el proyecto
                // si no quereis implementar el comando de stop.

 				break;
 			case START_CA_MEAS:

 				caConfiguration = MASB_COMM_S_getCaConfiguration();

 				__NOP();

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


