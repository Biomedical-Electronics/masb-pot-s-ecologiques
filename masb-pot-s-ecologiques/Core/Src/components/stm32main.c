/*
 * stm32main.c
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include "chronoamperometry.h"
#include "cyclic_voltammetry.h"

struct CV_Configuration_S cvConfiguration;
struct CA_Configuration_S caConfiguration;
struct Data_S data;

static TIM_HandleTypeDef *timer;

void setup(struct Handles_S *handles) {
    MASB_COMM_S_setUart(handles->huart);
    MASB_COMM_S_waitForMessage();

    HAL_TIM_Base_Start_IT(&htim3); // inicializamos el timer
}

void loop(void) {
    if (MASB_COMM_S_dataReceived()) { // Si se ha recibido un mensaje...

 		HAL_TIM_PeriodElapsedCallback(&htim3);

       // Una vez procesado los comando, esperamos el siguiente
 		MASB_COMM_S_waitForMessage();

 	}


}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	switch(MASB_COMM_S_command()) { // Miramos que comando hemos recibido

	 			case START_CV_MEAS: // Si hemos recibido START_CV_MEAS

	                 // Leemos la configuracion que se nos ha enviado en el mensaje y
	                 // la guardamos en la variable cvConfiguration
					cvConfiguration = MASB_COMM_S_getCvConfiguration();


	 				__NOP(); // Esta instruccion no hace nada y solo sirve para poder anadir un breakpoint

	 				// Aqui iria todo el codigo de gestion de la medicion que hareis en el proyecto
	                // si no quereis implementar el comando de stop.

	 				break;

	 			case START_CA_MEAS: // Si hemos recibido START_CA_MEAS

					 // Leemos la configuracion que se nos ha enviado en el mensaje y
					 // la guardamos en la variable caConfiguration
					caConfiguration = MASB_COMM_S_getCaConfiguration();


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
}
