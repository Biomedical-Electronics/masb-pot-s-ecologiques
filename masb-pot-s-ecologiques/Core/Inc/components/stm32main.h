/*
 * stm32main.h
 *
 *  Created on: 12 may. 2021
 *      Author: ayram
 */

#ifndef INC_COMPONENTS_STM32MAIN_H_
#define INC_COMPONENTS_STM32MAIN_H_

 #include "stm32f4xx_hal.h"

 struct Handles_S {
     //UART_HandleTypeDef *huart;
     // Aqui iriamos anadiendo los diferentes XXX_HandleTypeDef que necesitaramos anadir.
     UART_HandleTypeDef *huart2;
     I2C_HandleTypeDef *hi2c1;
     TIM_HandleTypeDef *htim3;
     ADC_HandleTypeDef *hadc1;

 };

// Prototypes.
void setup(struct Handles_S *handles);
void loop(void);

#endif /* INC_COMPONENTS_STM32MAIN_H_ */
