# MASB-POT-S Project

<img align="left" src="https://img.shields.io/badge/Proyecto-Potenciostato-yellow"><img align="left" src="https://img.shields.io/badge/Entorno de desarrollo-STM32CubeIDE-blue"><img align="left" src="https://img.shields.io/badge/Versión-v0.1.0-green"></br>

This project consists on the programming of a potentiostat formed by a _front-end_ previously designed by Albert Álvarez, and a _back-end_ based on the *Evaluation Board* (EVB) NUCLEO-F401RE of STMicroelectronics. This configuration is going to be used for electrichemical testing,

The group that has developed this project is called `ecologiques` and is composed by **Maria Penon** and **Helena Riesco**.

<p align="left">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/MARIA-PENON-BOSOM.jpg">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/HELENA-RIESCO-DOMINGO.jpg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/MARIA-PENON-BOSOM.jpg" width=250 align= left alt="Maria" />
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/HELENA-RIESCO-DOMINGO.jpg" width=250 align= right alt="Helena" />
</a>
</p>













> Figure 1. Maria Penon and Helena Riesco

## Index<!-- omit in toc -->

- [MASB-POT-S Project](#masb-pot-s-project)
  - [Introduction](#introduction)
    - [Potenciostat](#Potenciostat)
    - [Cyclic voltammetry](#cyclic-voltammetry)
    - [Chronoamperometry](#Chronoamperometry)
  - [Objetives](#objetives)
  - [Materials](#Materials)
    - [Evaluation board](#Evaluation-board)
    - [Git and GitHub](#Git-and-GitHub)
  - [Final application](#final-application)
    - [Implemented workflow](#implemented-workflow)
      - [feature/chronoamperomety](#feature/chronoamperomety)
      - [feature/cyclic_voltammetry](#feature/cyclic-voltammetry)
  - [Obtained results](#Obtained-results)
    - [Cyclic voltammetry](#cyclic-voltammetry)
    - [Chronoamperometry](#chronoamperometry)
  - [Conclusions](#conclusions)
    - [Project](#project)
    - [Subject](#subject)
  - [References](#References)



## Introduction

### Potenciostat

A potentiostat is a necessary electronic devise to control a three electrode cell for an electroanalytic experiment. The system works by mantaining the working electrode potential at a constant level respect to the reference electrode potential using adjustments of the current in the auxiliar electrode. This means that it is the responsable for polarizing the cell at a voltage VCELL and a current ICELL that goes through it. To establish VCELL, a DAC model MCP4725 with I2C direction `1100000` is going to be used since it allows the generation of an output voltage between 0 to 4V [1]. In Figure 2 a basic electrical circuit of a potentiostat can be seen.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/circuit.png">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/circuit.png" alt="circuits" />
</a>
</p>
> Figure 2. Basic electrical circuit of a potentiostat [2].

This type of devices are often used in the field of medical sensors since they have high sensitivity.

The main goal of such device is that, once it has been programmed and at the will of a user, it can be used to make two electrochemical readings_ the **cyclic voltammetry** and a **chronoamperometry**

It's the correct function of these two types of measures that is the main goal of this project.



### Cyclic voltammetry

A cyclic voltammetry is an electrochemical technique of great importance in which an input potential is inserted which value is modified between two fixed values  (*Ei* y *Ef*)  and the corresponding current values are obtained.

The potential sweep is performed in the working electrode in a triangular manner. The slop of this variation corresponds to the sweeping velocity. 

This technique is of great utility, specially in studies of *redox* processes, electrocatalitic properties, etc. Three different electrodes are used, a *Working electrode (WE)*, *Reference electrode (RE)* and *Auxiliar electrode (AE)* or *Counter electrode (CE)*.

- WE - Corresponds to the electrode that has contact with the analyte, applies the desired voltage in a controlled manner.
- RE - It should have a known potential which acts as a reference to be able to measure the WE potential.
- AE - The necessary current passes through to equilibrate the observed current in the WE.

In figure 3, an example of the graphs obtained from a cyclic voltammetry can be seen.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/voltametria.png">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/voltametria.png" alt="voltametria" />
</a>
</p>

> Figure 3. a)  Waveform of the entry voltage to a cyclic voltammetry vs time b) example of a graph of the output current vs voltage [3].

### Chronoamperometry

A chronoamperometry is an electrochemical technique which transduces the activity of the biological species in an electrochemical cell to a current signal which quantifies the concentration of the interest analyte. In this technique a step signal is applied and the current through the cell is measured as a function of time. One of the advantages of this technique is that it does not require the identification of the analyte or biorreceptor. The experiment starts by maintaining the cell to a potential in which there is no faradaic process going on. Then, the potential is elevated to a value in which a redox reaction takes place. In figure an example of a potential vs time graph can be seen.



<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/crono.png">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/crono.png" alt="crono" />
</a>
</p>

> Figure 4. Input waveform to the chronoamperometry vs time



## Objetives

The objectives of this project can be summarized in the following  points.

- Program a portable potentiostat.
- Control the *Power Management Unit* (PMU) of the *front-end* module of the potentiostat.
- Communicate with the viSens-S installed application with the *host* or computer by the MASB-COMM-S protocol.
- Perform a cyclic voltammetry.
- Perform a chronoamperometry.



## Materials

As it has been commented before, the materials that are going to be used are:

### Evaluation board

STM32 Nucleo-F401RE of STMicroelectronics, Figure 5.



<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/STM.png">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/STM.png" alt="STM" />
</a>
</p>

> Figure 5.  NUCLEO-F401RE of STMicroelectronics [4].

This EVB has been used as it offers a lot of advantages to improve and simplify the code creating process. This is like this as it integrates in the EVB itself the *debugger* (electronic circuitry necessary to program the microcontroller) and exposes all the microcontroller pins in a way which facilitates the connections with the external elements during the prototyping phase, which has been taken advantage of during this project. During the creation of the work  [the microcontroller *datasheet*](https://www.st.com/resource/en/datasheet/stm32f401re.pdf), the [microcontroller family reference manual](https://www.st.com/resource/en/reference_manual/dm00096844.pdf) and the [HAL library user manual (*Hardware Abstraction Layer*)](https://www.st.com/resource/en/user_manual/dm00105879.pdf) have been consulted . This last document has been used to search the HAL libraries to used them in the STM32Cube IDE environment, which has been the program used to code.



### Git and GitHub

Git es un programa de código abierto y gratuito que ofrece herramientas para desarrollar proyectos de cualquier tamaño y embergadura. Permite crear distintas lineas de trabajo independientes dentro del mismo proyecto para poder tener ramas de desarrollo, de test y operativas. Ha sido grácias a este programa que se ha podido trabajar en el desarrollo del proyecto en equipo, pues permite guardar versiones de las modificaciones realizar para poder recuperar versiones anteriores de un mismo documento [5].

GitHub, es una aplicación gratuita para la gestión de repositorios Git. Dispone de una interfaz web para tener acceso y control de las distintas colaboraciones y desarrollos del proyecto, incluyendo la opción de jerarquización de equipos, donde es necesaria la comprobación y aceptación por un project manager de las modificaciones realizadas por un desarrollador antes que se pueda unir el nuevo código, al programa principal. De este modo, se puede controlar y ir añadiendo sólo el contenido que se ha comprovado que funciona [6].



## Final application

### Implemented workflow

Este proyecto se ha implementado mediante el uso de dos ramas individuales (siguiendo el formato feature/nombre-descriptivo-rama) que posteriormente se han agrupado en la rama develop. Una vez se han comprobado y arreglado errores, se han pasado los archivos a la rama master para la visualización del cliente.

> Primeramente se intentó realizar cada implementación del código en ramas distintas para un posterior merge de todo en la rama develop, pero tuvimos diversos problemas con el merge cuando las ramas ya estaban todas creadas con las estructuras y código desarrollado.
>
> La manera más automática que tuvimos para solucionarlo, pues ya sabiamos que los códigos por separado funcionavan, fue la de crear sólo dos ramas para su posterior merge al develop.

Las dos ramas creadas son `feature/chronoamperomety` y `feature/cyclic-voltammetry`. En la primera se ha hecho toda la gestión de la cronoamperometría mientras que en la segunda se ha implementado la voltammetría cíclica.

La gestión del resto de componentes (DAC, ADC, comunicación serie, *Timers*, relé y PMU) se ha realizado directamente en cada una de estas dos ramas principales. 

De esta manera, el workflow de la aplicación es el siguiente:



#### feature/chronoamperomety

Primeramente se inicializó la rama `feature/chronoamperomety` y se crearon las estructuras, setup y loop en un archivo generado dentro de la carpeta components, `stm32main.c`. En la Figura 6, 7 y 8, los workflows de éste archivo pueden ser observados.



<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/general_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/general_en.jpeg" width=350 alt="general" />
</a>
</p>

> Figura 6. Estructura general de archivo stm32main.c

En las dos figuras siguientes, se detallan el funcionamiento de las funciones `setup()`y `loop()`.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/setup_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/setup_en.jpeg" width=350 alt="setup" />
</a>
</p>

> Figura 7. Estructura de función `setup()`.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/loop_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/loop_en.jpeg" width=350 alt="loop" />
</a>
</p>

> Figura 8. Estructura de función `loop()`.

Una vez finalizado éste archivo, se ha creado un archivo `chronoamperometry.c` y su respectivo header `chronoamperometry.h`. En la siguiente Figura, Figura 9, se puede ver su implementació en un workflow.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/crono_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/crono_en.jpeg" width=350 alt="Crono" />
</a>
</p>


> Figura 9. Estructura general del archivo `chronoamperometry.c`



Se puede ver que el archivo de `chronoamperometry.c` no tiene una estructura especialmente complicada, ésto cambia cuando analizamos la rama `feature/cyclic_voltammetry `  y sus respectivos archivos.



#### feature/cyclic_voltammetry

En el caso de ésta rama, sólo se han creado los archivos `cyclic_voltammetry.c` y `cyclic_voltammetry.h`. En el workflow a continuación en Figura 10 se puede ver la funcionalidad del código general y en la Figura 11 se puede ver el código más específico.

> Se ha utlizado la función del Timer de la Cronoamperimetría para el contador `mesura_punt`.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/general_volta.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/general_volta.jpeg" width=350 alt="General volta" />
</a>
</p>
> Figura 10. Estructura general del archivo `cyclic_voltammetry.c`.



<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/volta.png">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/volta.png" width=350 alt="Volta" />
</a>
</p>

> Figura 11. Estructura del archivo `cyclic_voltammetry.c`.



Se puede ver que la dificultad ha crecido exponencialmente si comparamos los workflows de las dos ramas `feature`, ésto es debido a la complegidad inherente del cálculo de la voltametría.



## Obtained results

Para poder comprobar la funcionalidad del programa desarrollado sin tener la muestra y la DAC, se ha usado el programa viSense (con un videotutorial en este [link](https://www.youtube.com/watch?v=UkXToFs8g6Y)), con el que se realiza la
configuración del tipo de medida a realizar, la comunicación con la placa del microcontrolador y la visualización de los resultados obtenidos.

Para poder solventar el no disponer de la disolución, se ha conectado un potenciómetro en forma de divisor de tensión en la entrada analógica, simulando así una señal. Posteriormente, una vez se ha comprobado el correcto funcionamiento, se ha realizado la prueba en el laboratorio.

Los resultados han sido correctos, los datos se han enviado, la señal recibida con los _timers_ correctos y la sensibilidad aceptable durante las pruebas prévias al testeado con la muestra biológica. Tanto la cronoamperimetria cómo la voltametría cíclica, han funcinoado correctamente.

Los resultados de la parte experimental se pueden ver a continuación.

### Cyclic voltammetry



### Chronoamperometry





## Conclusions

### Project

Este proyecto nos ha servido para utilizar los los distintos conocimientos
que hemos ido adquiriendo y desarrollar un código para una aplicación médica. Hemos podido aplicar los conocimientos adquiridos de cómo configurar una placa, lectura de pines, tipos de comunicación, uso de timers y su configuración, relaciones entre funciones, y mucho más. Sobretodo hemos adquirido más conocimiento sobre el uso de Git y GitHub para desarrollar programas en equipo de forma cómoda y útil, lo cual será extremadamente útil en nuestro futuro laboral. Para finalizar, nos ha dado las herramientas para saber interpretar _datasheets_ y saber encontrar la información necesaria para poder adaptarnos en distintas situaciones.



### Subject

Esta asignatura nos ha proporcionado las bases para, no sólo desarrollar nuestros propios proyectos de equipos médicos con microcontroladores, sino que nos ha enseñado maneras de poder trabajar en un solo proyecto en equipo.
Pese a que el conocimiento que hemos logrado adquirir sólo son las bases de la programación de microprocesadores, nos ha abierto las puertas en un ámbito que ahora está en auge.



## References

[1] Potentiostat - an overview | ScienceDirect Topics. (2021). Retrieved 21 May 2021, from https://www.sciencedirect.com/topics/engineering/potentiostat.

[2] Chhorn, S., & Teeramongkonrasmee, A. (2018). A Portable USB-controlled Potentiostat for Paper-based Electrochemical Applications. *2018 15th International Conference on Electrical Engineering/Electronics, Computer, Telecommunications and Information Technology (ECTI-CON)*, 321-324.

[3] An Electrochemical Potentiostat Interface for Mobile Devices: Enabling Remote Medical Diagnostics. (2021). Retrieved 2 June 2021, from https://arxiv.org/ftp/arxiv/papers/1509/1509.08591.pdf.

[4] NUCLEO-F401RE STMicroelectronics | Mouser España. (2021). Retrieved 2 June 2021, from https://www.mouser.es/ProductDetail/STMicroelectronics/NUCLEO-F401RE?qs=fK8dlpkaUMvGeToFJ6rzdA==.

[5] About - Git. Available at: https://git-scm.com/about.

[6] What Exactly Is GitHub Anyway? | TechCrunch. Available at: https://techcrunch.com/2012/07/14/what-exactly-is-github-anyway/.
