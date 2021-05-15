/*
 * stm32main.c
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#include "components/stm32main.h"
#include "components/masb_comm_s.h"

struct CV_Configuration_S cvConfiguration;
struct Data_S data;

uint32_t V_ADC = 0; // no hace falta que sea global, es local
float V_CELL = 0;
float I_CELL = 0;
float V_DAC = 0;
const float R_TIA = 10000; // constante porque el valor de la resistencia no varía

void setup(struct Handles_S *handles) {
    MASB_COMM_S_setUart(handles->huart);
    MASB_COMM_S_waitForMessage();
}

void loop(void) {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);

	V_ADC = HAL_ADC_GetValue(&hadc1);

	V_CELL = (1.65 - V_ADC)*2; // la fórmula que sabemos que se corresponde
	I_CELL = V_CELL/R_TIA; // la fórmula para obtener I_CELL

	V_DAC = 1.65 - (V_CELL/2);

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
