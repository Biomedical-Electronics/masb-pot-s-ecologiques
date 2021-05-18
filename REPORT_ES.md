# Proyecto MASB-POT-S

<img align="left" src="https://img.shields.io/badge/Proyecto-Potenciostato-yellow"><img align="left" src="https://img.shields.io/badge/Entorno de desarrollo-STM32CubeIDE-blue"><img align="left" src="https://img.shields.io/badge/Versión-v0.1.0-green"></br>

Éste proyecto consiste en la programación de un potenciostato formado por un *front-end* previamente diseñado por Albert Álvarez, y un *back-end* basado en la *Evaluation Board* (EVB) NUCLEO-F401RE de STMicroelectronics. Ésta configuración se va a realizar para llevar a cabo pruebas electroquímicas. 

El grupo que ha realizado éste proyecto se llama `ecologiques` y está formado por Helena Riesco y Maria Penón.

## Contenidos <!-- omit in toc -->

- [Proyecto MASB-POT-S](#proyecto-masb-pot-s)
  - [Introducción](#introducción)
  - [Objetivos](#objetivos)
  - [Materiales](#Materiales)
  - [Aplicación final](#aplicación-final)
    - [Workflow implementado](#workflow implementado)
  - [Resultados obtenidos](#Resultados obtenidos)
    - [Voltametría cíclica](#voltametría_cíclica)
    - [Cronoamperometría](#cronoamperometría)
  - [Conclusiones](#conclusiones)
    - [Proyecto](#proyecto)
    - [Asignatura](#asignatura)

## Introducción

Un potenciostato es un dispositivo electrónico necesario para controlar una celda de tres electrodos para un experimento electroanalítico. El sistema funciona manteniendo el potencial del electrodo de trabajo a un nivel constante con respecto al potencial del electrodo de referencia mediante el ajuste de la corriente en un electrodo auxiliar. Esto significa que es el responsable de polarizar la celda electroquímica a una tensión VCELL y la tensión ICELL que circula por ella. Para establecer VCELL, se va a utilizar la DAC modelo MCP4725 con dirección I2C `1100000`, pues permite generar una tensión de salida de 0 a 4V [1].

Estos dispositivos son muy usados en el mundo de la sensórica médica ya que tienen una sensibilidad muy grande.

El objetivo es que, una vez programado y a voluntad del usuario, se pueda utilizar para realizar dos medidas electroquímicas: la **voltammetría cíclica** y la **cronoamperometría**.

### Voltammetría cíclica

Una voltammetría cíclica es una técnica electroquímica de gran importancia en la que se aplica un potencial de entrada cuyo valor se va modificando entre dos valores fijos (*Ei* y *Ef*) y se obtienen los valores de corriente correspondientes.

El barrido de potencial se realiza sobre el electrodo de trabajo y se hace de manera triangular. La pendiente de esta variación se corresponde con la velocidad de barrido. 

Esta técnica es de gran utilidad, especialmente en estudios de procesos *redox*, propiedades electrocatalíticas, etc. Se utilizan tres electrodos diferentes, un *Working electrode (WE)*, *Reference electrode (RE)* y *Auxiliar electrode (AE)* o *Counter electrode (CE)*.

- WE - Corresponde al electrodo que tiene contacto con el analito, aplica el potencial deseado de manera controlada.
- RE - Debe tener un potencial conocido que actúe como referencia para poder medir el potencial del WE.
- AE - Pasa la corriente necesaria para equilibrar la corriente observada en el WE.

### Cronoamperometría

Una Cronoamperometría (CA) es una técnica electroquímica que transduce la actividad de las especies biológicas de una celda electroquímica en una señal de corriente que cuantifica la concentración del analito de interés. En esta técnica se aplica una señal escalón y se mide la corriente a través de la celda en función del tiempo. Una de las ventajas de esta técnica es que no requiere etiquetado de analito o biorreceptor. El experimento comienza manteniendo la celda a un potencial en el que no ocurre ningún proceso faradaico. Entonces, el potencial se eleva a un valor en el cual ocurre una reacción redox.



## Objetivos

Los objetivos de éste proyecto se pueden resumir en los siguientes cinco puntos.

- Programar un potenciostato portable.
- Controlar la *Power Management Unit* (PMU) del módulo *front-end* del potenciostato.
- Comunicarse con la aplicación viSens-S instalada con el *host* u ordenador mediante el protocolo MASB-COMM-S.
- Realizar una voltammetría cíclica.
- Realizar una cronoamperometría.



## Materiales

Cómo se ha mencionado anteriormente, se va a usar:

- NUCLEO-F401RE de STMicroelectronics
- Potenciostato

El microcontrolador se va a programar mediante el software STM32CubeIDE usando cómo referencia la [documentación de la placa]().

## Aplicación final

### Workflow implementado

Éste proyecto se ha implementado mediante el uso de ramas individuales (siguiendo el formato feature/nombre-descriptivo-rama) que posteriormente se han agrupado en la rama develop. Una vez se han comprovado y arreglado errores, se han pasado los archivos a la rama master para la visualización del cliente.

 



## Resultados obtenidos

### Voltametría cíclica

### Cronoamperometría



## Conclusiones

### Proyecto

### Asignatura

