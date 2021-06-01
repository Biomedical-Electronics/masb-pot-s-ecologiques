/**
 ******************************************************************************
 * @file		cyclic_voltammetry.c
 * @brief		Gestión de la voltammetría cíclica.
 * @author		Albert Álvarez Carulla
 * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
 ******************************************************************************
 */

// definimos las macros
#define FALSE 			0
#define TRUE 			1

#include "components/cyclic_voltammetry.h"
#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include <math.h>
#include "main.h"
#include "components/ad5280_driver.h"
#include "components/i2c_lib.h"
#include "components/mcp4725_driver.h"



// inicializamos los periféricos
//static UART_HandleTypeDef *huart;
extern MCP4725_Handle_T hdac;
extern ADC_HandleTypeDef hadc1;
//static I2C_HandleTypeDef *hi2c1;
extern TIM_HandleTypeDef htim3;

extern volatile _Bool mesura_punt;
double R_TIA_volta = 10000;
//extern char estado;

// definimos el sampling period con la fórmula dada
void Mesurant_CV(struct CV_Configuration_S cvConfiguration) {

	uint8_t cycles = cvConfiguration.cycles;
	double samp_period = cvConfiguration.eStep / cvConfiguration.scanRate * 1000.0;

	uint8_t cambio = 1;

	double V_CELL_pre = cvConfiguration.eBegin; // fijamos la tensión de la celda electroquímica a eBegin
	double vObjetivo = cvConfiguration.eVertex1; // establecemos como voltaje objetivo el del vértice 1
	double V_DAC = (1.65 - (V_CELL_pre / 2.0));

	MCP4725_SetOutputVoltage(hdac, V_DAC);

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1); // cerramos el relé

	__HAL_TIM_SET_AUTORELOAD(&htim3, samp_period * 10.0); //fijamos periodo

	__HAL_TIM_SET_COUNTER(&htim3, 0); // reiniciamos el contador del timer a 0

	HAL_TIM_Base_Start_IT(&htim3); // inicializamos el timer

	uint32_t repeticio = 0;
	uint32_t counter = 0; // y el contador
	//uint32_t V_ADC = 0;
	//HAL_ADC_Start(&hadc1);

	//double V_CELL = HAL_ADC_GetValue(&hadc1);

	//V_CELL = (double) (1.65 - V_ADC) * 2;

	//mesura_punt_volta = FALSE;


	while (cycles) { // mientras el contador sea más pequeño que el número de ciclos
		//estado = "CV";
		if (mesura_punt == TRUE) { // si ha pasado el sampling period

			HAL_ADC_Start(&hadc1);

			HAL_ADC_PollForConversion(&hadc1, 100);
			double V_ADC = HAL_ADC_GetValue(&hadc1)*3.3/4095.0; //conversion tenint en compte (voltatge referencia/4096) ja que opera a 12 bits

			// medimos V_cell e I_cell
			double V_CELL = (double) (1.65 - V_ADC) * 2;
			double I_CELL = (double) V_CELL / R_TIA_volta;
			//mesura_punt_volta = FALSE;

			struct Data_S data;
			data.point = repeticio;
			data.timeMs = counter;
//			data.voltage = V_CELL;
			data.voltage = V_CELL_pre;
			data.current = I_CELL;

			// enviamos datos al host
			MASB_COMM_S_sendData(data);

			counter = counter + samp_period;
			repeticio = repeticio + 1;


			if (V_CELL_pre == vObjetivo) { // si V_cell es igual a V_objetivo

				if (V_CELL_pre == cvConfiguration.eVertex1) {

					vObjetivo = cvConfiguration.eVertex2; // hacemos que el objetivo sea el otro extremo
					cambio = -1;

				} else if (V_CELL_pre == cvConfiguration.eVertex2) { // si es falso

					vObjetivo = cvConfiguration.eBegin; // fijamos el valor de vObjetivo
					cambio = 1;

				} else {
					vObjetivo = cvConfiguration.eVertex1;
					cambio = 1;
					cycles--;


				}

			}

			else {
				if (V_CELL_pre + cambio * (cvConfiguration.eStep) > vObjetivo) {
					V_CELL_pre = vObjetivo;
				} else {
					V_CELL_pre = V_CELL_pre + (cvConfiguration.eStep * cambio);
				}
			}

			mesura_punt = FALSE;
		}



	}

	HAL_TIM_Base_Stop_IT(&htim3);
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0); // abrimos el relé

}

