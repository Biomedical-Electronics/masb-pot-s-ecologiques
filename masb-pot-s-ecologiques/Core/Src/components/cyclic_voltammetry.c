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

#define FALSE 			0
#define TRUE 			1

ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim3;


uint32_t samp_period = cvConfiguration.eStep/cvConfiguration.scanRate;
double vObjetivo = 0; //inicializamos la variable vObjetivo

V_CELL = cvConfiguration.eBegin; // fijamos la tensión de la celda electroquímica a eBegin

vObjetivo = cvConfiguration.eVertex1; // establecemos como voltaje objetivo el del vértice 1

// cerramos el relé
HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);

__HAL_TIM_SET_AUTORELOAD(&htim3, samp_period * 10);

__HAL_TIM_SET_COUNTER(&htim3, 0);

HAL_TIM_Base_Start_IT(&htim3);

_Bool mesura_punt = FALSE;

uint32_t counter = 0;

while (counter <= cvConfiguration.cycles) {
	if (mesura_punt == TRUE){
		double V_CELL = (double)(1.65 - V_ADC)*2;
		double I_CELL = (double)V_CELL/R_TIA;

		MASB_COMM_S_sendData(struct Data_S data);
		mesura_punt = FALSE;
		counter = counter + 1;
		if (V_CELL == vObjetivo){
			if (V_CELL == cvConfiguration.eVertex1){
				vObjetivo = cvConfiguration.eVertex2;
			}else{
				if (vObjetivo == cv.Configuration.eVertex2){
					vObjetivo = cv.Configuration.eBegin;
				}else{
					if (counter == cvConfiguration.cycles){
						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 0);
					}else{
						vObjetivo = cvConfiguration.eVertex1;
					}
				}
			}
		}
		if (V_CELL > vObjetivo){
			V_CELL = vObjetivo;

		}else{
			if
		}
	}
}

HAL_TIM_Base_Stop_IT(&htim3);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) {
	mesura_punt = TRUE;
}



