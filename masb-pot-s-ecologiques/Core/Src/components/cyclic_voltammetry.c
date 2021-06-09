/**
 ******************************************************************************
 * @file		cyclic_voltammetry.c
 * @brief		Gestión de la voltammetría cíclica.
 * @author		Albert Álvarez Carulla
 * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
 ******************************************************************************
 */

// definimos las macros para true y false
#define FALSE 			0
#define TRUE 			1

// incluimos todos los ficheros necesarios
#include "components/cyclic_voltammetry.h"
#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include <math.h>
#include "main.h"
#include "components/ad5280_driver.h"
#include "components/i2c_lib.h"
#include "components/mcp4725_driver.h"
#include "components/formulas.h"

// inicializamos los periféricos
extern MCP4725_Handle_T hdac;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

extern volatile _Bool mesura_punt; // externa y volátil porque ya ha sido definida en cronoamperometría
double R_TIA_volta = 50000; // definimos la resistencia y le damos el valor correspondiente

const double TOL = 0.0001;

_Bool equalsDecimals(double number1, double number2, double tolerance) {

	return fabs(number1 - number2) < tolerance;

}

// creamos la función que nos permitirá realizar la voltametría cíclica
void Mesurant_CV(struct CV_Configuration_S cvConfiguration) {

	uint8_t cycles = cvConfiguration.cycles; // creamos la variable ciclos
	double samp_period = cvConfiguration.eStep / cvConfiguration.scanRate
			* 1000.0; // creamos la variable del sampling period con la fórmula
					  // dada

	uint8_t cambio = 1; // creamos una variable que nos permitirá realizar los incrementos o decrecimientos

	double V_CELL_pre = cvConfiguration.eBegin; // fijamos la tensión de la celda electroquímica a eBegin
	double vObjetivo = cvConfiguration.eVertex1; // establecemos como voltaje objetivo el del vértice 1
	double V_DAC = calculateDacOutputVoltage(V_CELL_pre);

	MCP4725_SetOutputVoltage(hdac, V_DAC); // ponemos como output la tensión correspondiente del DAC

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1); // cerramos el relé

	__HAL_TIM_SET_AUTORELOAD(&htim3, samp_period * 10.0); //fijamos periodo

	__HAL_TIM_SET_COUNTER(&htim3, 0); // reiniciamos el contador del timer a 0

	HAL_TIM_Base_Start_IT(&htim3); // inicializamos el timer

	uint32_t repeticio = 0; // generamos una variable que nos servirá para indicar el punto de medición
	uint32_t counter = 0; // y el contador

	while (cycles) { // mientras el contador sea más pequeño que el número de ciclos

		if (mesura_punt == TRUE) { // si ha pasado el sampling period

			HAL_ADC_Start(&hadc1); // inicializamos el ADC

			HAL_ADC_PollForConversion(&hadc1, 100); // buscamos la conversión

			uint32_t V_ADC = HAL_ADC_GetValue(&hadc1); //conversion teniendo en cuenta (voltaje referencia/4096) ya que opera a 12 bits

			// medimos V_cell e I_cell
			double V_CELL = calculateVrefVoltage(V_ADC);

			HAL_ADC_Start(&hadc1); // inicializamos el ADC

			HAL_ADC_PollForConversion(&hadc1, 100); // buscamos la conversión

			V_ADC = HAL_ADC_GetValue(&hadc1);

			double I_CELL = calculateIcellCurrent(V_ADC);

			// creamos la estructura para los datos de medición
			struct Data_S data;
			data.point = repeticio;
			data.timeMs = counter;
			data.voltage = V_CELL;
			data.current = I_CELL;

			// enviamos datos al host
			MASB_COMM_S_sendData(data);

			// aumentamos el contador en un periodo de sampleo
			counter = counter + samp_period;

			// y aumentamos el número de repeticiones en una unidad
			repeticio = repeticio + 1;

			if (cambio * (V_CELL_pre - vObjetivo) > 0) { // si V_cell es igual a V_objetivo

				if (equalsDecimals(vObjetivo, cvConfiguration.eVertex1, TOL)) { // y esta es igual al voltaje del primer vértice

					vObjetivo = cvConfiguration.eVertex2; // hacemos que el objetivo sea el otro extremo
					cambio = -1; // y cambiamos el signo de los incrementos

				} else if (equalsDecimals(vObjetivo, cvConfiguration.eVertex2,
						TOL)) { // si es falso

					vObjetivo = cvConfiguration.eBegin; // fijamos el valor de vObjetivo
					cambio = 1; // y cambiamos el signo de los incrementos

				} else { // en otro caso
					vObjetivo = cvConfiguration.eVertex1; // fijamos el voltaje objetivo al vértice 1
					cambio = 1; // cambiamos el signo de los incrementos
					cycles--; // le restamos una unidad a los ciclos
				}

			}

			else { // en caso de que V_CELL no sea igual a vObjetivo
				if (equalsDecimals(vObjetivo, cvConfiguration.eVertex1, TOL)) { // si es igual al vértice 1
					if (V_CELL_pre + cambio * (cvConfiguration.eStep)
							> vObjetivo) { // y el valor actual de la tensión + el incremento es mayor que el objetivo
						V_CELL_pre = vObjetivo; // fijamos su valor al objetivo
					} else { // en el caso que el incremento no sea mayor
						V_CELL_pre = V_CELL_pre
								+ (cvConfiguration.eStep * cambio); // realizamos el incremento/decrecimiento
					}
				}

				else if (equalsDecimals(vObjetivo, cvConfiguration.eVertex2,
						TOL)) { // si es igual al vértice 2
					if (V_CELL_pre + cambio * (cvConfiguration.eStep)
							< vObjetivo) { // y el valor actual de la tensión + el incremento es menor que el objetivo
						V_CELL_pre = vObjetivo; // fijamos su valor al objetivo
					} else { // en el caso que el incremento no sea menor
						V_CELL_pre = V_CELL_pre
								+ (cvConfiguration.eStep * cambio); // realizamos el incremento/decrecimiento
					}
				} else { // en el caso de que el voltaje sea igual al de inicio
					if (V_CELL_pre + cambio * (cvConfiguration.eStep)
							> vObjetivo) { // y si el aumento hace que sea mayor que el objetivo
						V_CELL_pre = vObjetivo; // hacemos que el voltaje sea igual al objetivo
					} else { // en el caso de que sea menor
						V_CELL_pre = V_CELL_pre
								+ (cvConfiguration.eStep * cambio); // realizamos el incremento/decrecimiento
					}
				}

			}
			MCP4725_SetOutputVoltage(hdac,
					calculateDacOutputVoltage(V_CELL_pre));
			mesura_punt = FALSE; // salimos del if
		}

	}

	HAL_TIM_Base_Stop_IT(&htim3); // paramos el timer
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0); // abrimos el relé

}

