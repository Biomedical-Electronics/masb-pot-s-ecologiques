/**
 ******************************************************************************
 * @file		chronoamperometry.c
 * @brief		Gestión de la cronoamperometría.
 * @author		Albert Álvarez Carulla
 * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
 ******************************************************************************
 */
// definimos las macros para true y false
#define FALSE  			0
#define TRUE 			1

// incluimos los archivos .h necesarios
#include "components/chronoamperometry.h"
#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include <math.h>
#include "main.h"
#include "components/ad5280_driver.h"
#include "components/i2c_lib.h"
#include "components/mcp4725_driver.h"
#include "components/formulas.h"

// ponemos la inicialización de los periféricos
extern MCP4725_Handle_T hdac;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

volatile _Bool mesura_punt = FALSE; // variable que nos permitirá entrar en el bucle
double R_TIA = 50000; // definimos la resistencia de transimp

void Mesurant_Crono(struct CA_Configuration_S caConfiguration) { //definicion de la funcion como tal

	uint32_t samp_period = caConfiguration.samplingPeriodMs; // definimos el periodo de sampleo
	uint32_t meas_time = caConfiguration.measurementTime * 1000; // y el de medición multiplicado por 1000 para ser ms

	double V_CELL_pre = caConfiguration.eDC; // fijamos la tensión de la celda electroquímica a eDC
	float V_DAC = calculateDacOutputVoltage(V_CELL_pre);

	MCP4725_SetOutputVoltage(hdac, V_DAC); // fijamos el output del DAC

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);   // cerramos relé

	__HAL_TIM_SET_AUTORELOAD(&htim3, samp_period * 10); // Fijamos el periodo.
	// El mutliplicar el samplingPeriodMs por 10 para fijar el periodo solo es valido
	// si se fija una frecuencia de trabajo para el timer de 10 kHz.

	__HAL_TIM_SET_COUNTER(&htim3, 0); // Reiniciamos el contador del timer a 0.
	HAL_TIM_Base_Start_IT(&htim3); // Iniciamos el timer.

	uint32_t counter = 0; // contador
	uint32_t repeticio = 0; // variable que nos indicará el punto de medición
	uint32_t V_ADC = 0;

	HAL_ADC_Start(&hadc1); // iniciamos el ADC
	HAL_ADC_PollForConversion(&hadc1, 100); // realizamos la conversión
	V_ADC = HAL_ADC_GetValue(&hadc1); //conversion teniendo en cuenta (voltaje referencia/4095) yaa que opera a 12 bits

	double V_CELL =calculateVrefVoltage(V_ADC); // voltaje de celda

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,100);
	V_ADC = HAL_ADC_GetValue(&hadc1);

	double I_CELL = calculateIcellCurrent(V_ADC); // corriente de celda

	mesura_punt = FALSE; //Iniciamos la variable en FALSE para que no entre en el while a no ser que haya pasado el tiempo necesario

	counter = counter + samp_period; // Creamos  el contador que nos permitirá controlar el tiempo de medición

	struct Data_S data; // creamos la estructura para la medicion de los datos
	data.point = repeticio;
	data.timeMs = counter;
	data.voltage = V_CELL;
	data.current = I_CELL;

	MASB_COMM_S_sendData(data); // enviamos los datos inciales

	repeticio = repeticio + 1; // aumentamos el punto en una unidad

	while (counter <= meas_time) { // mientras el contador siga siendo menor que el tiempo de medición
		if (mesura_punt == TRUE) { // si ha saltado el callback cambiando el valor de la variable

			HAL_ADC_Start(&hadc1); // iniciamos el ADC
			HAL_ADC_PollForConversion(&hadc1, 100); // realizamos la conversión
			V_ADC = HAL_ADC_GetValue(&hadc1); //conversion teniendo en cuenta (voltaje referencia/4095) yaa que opera a 12 bits

			V_CELL =calculateVrefVoltage(V_ADC); // voltaje de celda

			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1,100);
			V_ADC = HAL_ADC_GetValue(&hadc1);

			I_CELL = calculateIcellCurrent(V_ADC);

			data.point = repeticio;
			data.timeMs = counter;
			data.voltage = V_CELL;
			data.current = I_CELL;

			MASB_COMM_S_sendData(data); // enviamos los datos medidos

			counter = counter + samp_period; // aumentamos el contador en un periodo de sampleo
			repeticio = repeticio + 1; // y la repetición en una unidad

			mesura_punt = FALSE; // salimos del if
		}

	}

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0);
	HAL_TIM_Base_Stop_IT(&htim3); // paramos el timer


}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) { // creamos el callback que nos permitirá entrar
															   // en el if
mesura_punt = TRUE;

}
