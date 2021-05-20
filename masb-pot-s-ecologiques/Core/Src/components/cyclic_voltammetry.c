/**
  ******************************************************************************
  * @file		cyclic_voltammetry.c
  * @brief		Gestión de la voltammetría cíclica.
  * @author		Albert Álvarez Carulla
  * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
  ******************************************************************************
  */

#include "components/cyclic_voltammetry.h"
#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include <math.h>
#include "main.h"
#include "components/ad5280_driver.h"
#include "components/i2c_lib.h"
#include "components/mcp4725_driver.h"

// definimos las macros
#define FALSE 			0
#define TRUE 			1

// inicializamos los periféricos
static UART_HandleTypeDef *huart;
static MCP4725_Handle_T hdac;
extern ADC_HandleTypeDef hadc1;
static I2C_HandleTypeDef *hi2c1;
extern TIM_HandleTypeDef htim3;

_Bool mesura_punt = FALSE;
double R_TIA = 10000;

// definimos el sampling period con la fórmula dada
void Mesurant_CV(struct CV_Configuration_S cvConfiguration) {
	uint8_t cycles = cvConfiguration.cycles;
	uint32_t samp_period = cvConfiguration.eStep/cvConfiguration.scanRate;
	int cambio = 1;


	double V_CELL_pre = cvConfiguration.eBegin; // fijamos la tensión de la celda electroquímica a eBegin

	double vObjetivo = cvConfiguration.eVertex1; // establecemos como voltaje objetivo el del vértice 1
	double V_DAC = (1.65 - (V_CELL_pre / 2.0));

	//MCP4725_SetOutputVoltage(hdac, V_DAC); // comentar para probar

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1); // cerramos el relé


	__HAL_TIM_SET_AUTORELOAD(&htim3, samp_period * 10); //fijamos periodo

	__HAL_TIM_SET_COUNTER(&htim3, 0); // reiniciamos el contador del timer a 0

	HAL_TIM_Base_Start_IT(&htim3); // inicializamos el timer


	uint32_t counter = 1; // y el contador
	uint32_t repeticio = 0;
	uint32_t V_ADC = 0;
	HAL_ADC_Start(&hadc1);

	double V_CELL = HAL_ADC_GetValue(&hadc1);

	V_CELL = (double) (1.65 - V_ADC) * 2;
	double I_CELL = (double) V_CELL / R_TIA;

	mesura_punt = FALSE;
	counter = counter + samp_period;
	struct Data_S data;
	data.point = repeticio;
	data.timeMs = counter;
	data.voltage = V_CELL;
	data.current = I_CELL;

	MASB_COMM_S_sendData(data);

	while (counter <= cycles) { // mientras el contador sea más pequeño que el número de ciclos
		if (mesura_punt == TRUE){ // si ha pasado el sampling period

			// medimos V_cell e I_cell
			double V_CELL = (double)(1.65 - V_ADC)*2;
			double I_CELL = (double)V_CELL/R_TIA;
			mesura_punt = FALSE;

			data.point = repeticio;
			data.timeMs = counter;
			data.voltage = V_CELL;
			data.current = I_CELL;

			counter = counter + samp_period;
			repeticio = repeticio + 1;

		// enviamos datos al host
		MASB_COMM_S_sendData(data);

		if (V_CELL == vObjetivo) { // si V_cell es igual a V_objetivo

			if (V_CELL == cvConfiguration.eVertex1){

				vObjetivo = cvConfiguration.eVertex2; // hacemos que el objetivo sea el otro extremo
				cambio = -1;

			} else if (V_CELL == cvConfiguration.eVertex2) { // si es falso

					vObjetivo = cvConfiguration.eBegin; // fijamos el valor de vObjetivo
					cambio = 1;

			} else if (counter == cycles){
					HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0);
					}
			else{
					vObjetivo = cvConfiguration.eVertex1;
					cambio = 1;

				}
			}

		else {
			if (V_CELL + cambio*(cvConfiguration.eStep) > vObjetivo) {
				MASB_COMM_S_sendData(data);
			} else {
				V_CELL = V_CELL +(cvConfiguration.eStep*cambio);
				MASB_COMM_S_sendData(data);
			}
		}
	}
	mesura_punt = FALSE;
	counter = counter + 1;
	V_CELL = V_CELL + cvConfiguration.eStep;
}
	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0); // cerramos el relé

}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) {
	mesura_punt = TRUE;
}



