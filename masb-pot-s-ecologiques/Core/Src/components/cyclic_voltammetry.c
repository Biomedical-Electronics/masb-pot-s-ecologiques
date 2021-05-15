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

double vObjetivo = 0; //inicializamos la variable vObjetivo

V_CELL = cvConfiguration.eBegin; // fijamos la tensión de la celda electroquímica a eBegin

vObjetivo = cvConfiguration.eVertex1; // establecemos como voltaje objetivo el del vértice 1

// cerramos el relé
HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, 1);

//Aquí faltaría hacer lo de medir
