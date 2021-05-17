/*
 * stm32main.c
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#include "components/stm32main.h"
#include "components/masb_comm_s.h"

struct CV_Configuration_S cvConfiguration;
struct CA_Configuration_S caConfiguration;
struct Data_S data;

void setup(struct Handles_S *handles) {
    MASB_COMM_S_setUart(handles->huart);
    MASB_COMM_S_waitForMessage();
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
 			case START_CA_MEAS:

 				caConfiguration = MASB_COMM_S_getCaConfiguration():

				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);  //cerramos relé

 				V_CELL = caConfiguration.eDC; // fijamos la tensión de la celda electroquímica a eDC

				//HAL_TIM_Base_Start_IT(&htim3);  //se inicia timer

				// Se supone que detenemos el timer justo al finalizar la medicion anterior. Por
				// eso esta comentada.
				// HAL_TIM_Base_Stop_IT(&htim3);

				__HAL_TIM_SET_AUTORELOAD(&htim3, samplingPeriodMs * 10); // Fijamos el periodo.
				// El mutliplicar el samplingPeriodMs por 10 para fijar el periodo solo es valido
				// si se fija una frecuencia de trabajo para el timer de 10 kHz.

				__HAL_TIM_SET_COUNTER(&htim3, 0); // Reiniciamos el contador del timer a 0.
				HAL_TIM_Base_Start_IT(&htim3); // Iniciamos el timer.
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


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) {

	//contador con measuring time
	//if true mesuramos
	V_CELL=(1.65 - V_ADC)*2
	I_CELL=((V_ADC - 1.65))
	//enviamos host

	//abrimos relé
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0);
}
