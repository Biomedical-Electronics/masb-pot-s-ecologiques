/**
 ******************************************************************************
 * @file		chronoamperometry.c
 * @brief		Gestión de la cronoamperometría.
 * @author		Albert Álvarez Carulla
 * @copyright	Copyright 2020 Albert Álvarez Carulla. All rights reserved.
 ******************************************************************************
 */
// definimos las macros
#define FALSE  			0
#define TRUE 			1

#include "components/chronoamperometry.h"
#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include <math.h>
#include "main.h"
#include "components/ad5280_driver.h"
#include "components/i2c_lib.h"
#include "components/mcp4725_driver.h"

//static UART_HandleTypeDef *huart;
extern MCP4725_Handle_T hdac;
extern ADC_HandleTypeDef hadc1;
//static I2C_HandleTypeDef *hi2c1;
extern TIM_HandleTypeDef htim3;

volatile _Bool mesura_punt = FALSE;
double R_TIA = 10000;

void Mesurant_Crono(struct CA_Configuration_S caConfiguration) {

	uint32_t samp_period = caConfiguration.samplingPeriodMs;
	uint32_t meas_time = caConfiguration.measurementTime * 1000;

	double V_CELL_pre = caConfiguration.eDC; // fijamos la tensión de la celda electroquímica a eDC
	double V_DAC = (1.65 - (V_CELL_pre / 2.0));

	MCP4725_SetOutputVoltage(hdac, V_DAC);

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);   //cerramos relé

	__HAL_TIM_SET_AUTORELOAD(&htim3, samp_period * 10); // Fijamos el periodo.
	// El mutliplicar el samplingPeriodMs por 10 para fijar el periodo solo es valido
	// si se fija una frecuencia de trabajo para el timer de 10 kHz.

	__HAL_TIM_SET_COUNTER(&htim3, 0); // Reiniciamos el contador del timer a 0.
	HAL_TIM_Base_Start_IT(&htim3); // Iniciamos el timer.

	uint32_t counter = 0;
	uint32_t repeticio = 0;
	uint32_t V_ADC = 0;
	//HAL_ADC_Start(&hadc1);


	double V_CELL = (double) (1.65 - V_ADC) * 2;
	double I_CELL = (double) V_CELL / R_TIA;

	mesura_punt = FALSE; //Iniciamos la variable en FALSE para que no entre en el while a no ser que haya pasado el tiempo necesario

	counter = counter + samp_period; // Creamos  el contador que nos permitirá controlar el tiempo de medición

	struct Data_S data;
	data.point = repeticio;
	data.timeMs = counter;
	data.voltage = V_CELL;
	data.current = I_CELL;

	MASB_COMM_S_sendData(data); // enviamos los datos inciales

	while (counter <= meas_time) {
		if (mesura_punt == TRUE) {

			HAL_ADC_Start(&hadc1);

			HAL_ADC_PollForConversion(&hadc1, 100);
			V_ADC = HAL_ADC_GetValue(&hadc1)*3.3/4095.0; //conversion tenint en compte (voltatge referencia/4096) ja que opera a 12 bits

			V_CELL = (double) (1.65 - V_ADC) * 2;
			double I_CELL = (double) V_CELL / R_TIA;
			//mesura_punt = FALSE;

			data.point = repeticio;
			data.timeMs = counter;
			data.voltage = V_CELL;
			data.current = I_CELL;

			MASB_COMM_S_sendData(data);

			counter = counter + samp_period;
			repeticio = repeticio + 1;

			mesura_punt = FALSE;
		}

	}

	HAL_TIM_Base_Stop_IT(&htim3);


}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) { // creamos el callback que nos permitirá entrar
															   // en el while
mesura_punt = TRUE;

}
