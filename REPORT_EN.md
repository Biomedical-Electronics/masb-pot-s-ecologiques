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

Git is an open code program and free which offers tools to develop project of whatever size. It allows for the creation of independent working lines within the same project in order to have development branches, test branches and operative branches. Thanks to this program it has been possible to work in the development in the project as a team, as it allows saving versions of the modifications performed in order to be able to restore previous versions of the same document [5].

GitHub, is a free app for the management of Git repositories. It has a web interface in order to have access and control to the different collaborations and developments of the project, including the hierarchy of teams, where the approval of the modifications performed by a developer from a project manager is necessary before the new code can be merged into the main program. In this way, only the content that has been made sure that it works can be added and controlled [6].



## Final application

### Implemented workflow

This project has been implemented via the use of two individual branches (following the format feature/descriptive-name-branch) which have then been added to the develop branch. Once it has been run and the errors have been fixed, the files have been passed to the master branch for the client's visualization.

> Firstly, performing the implementation in different branches for its later incorporation into the develop branch was tried, however, different problems with the merge were encountered when all the structures and code had already been developed.
>
> The most automatic way to solve it, as we already new the different codes worked, was to create only two branches for the merge with develop.

The two branches created are `feature/chronoamperometry` and `feature/cyclic-voltammetry`. In the first one, all the management of the chronoamperometry has been done while in the second the cyclic voltammetry has been implemented.

The management of the rest of the components (DAC, ADC, comunicación serie, *Timers*, relay and PMU) have been performed directly in each of these two main branches. 

In this way, the workflow of the app is the following:



#### feature/chronoamperomety

Firstly, the branch `feature/chronoamperomety` was initialized and the structures setup and loop were created in a file inside the components directory, `stm32main.c`. In Figures 6, 7 and 8, the workflows of this file can be observed.



<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/general_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/general_en.jpeg" width=350 alt="general" />
</a>
</p>

> Figure 6. General structure of the file stm32main.c

In the two following figures, the functioning of the functions `setup()`and `loop()` are detailed.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/setup_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/setup_en.jpeg" width=350 alt="setup" />
</a>
</p>

> Figure 7. Structure of the function `setup()`.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/loop_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/loop_en.jpeg" width=350 alt="loop" />
</a>
</p>

> Figure 8. Structure of the `loop()` function.

Once finished this file, the file  `chronoamperometry.c` has been created and also its respective header `chronoamperometry.h`. In the following figure, Figure 9, its implementation can be seen in a workflow.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/crono_en.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/crono_en.jpeg" width=350 alt="Crono" />
</a>
</p>


> Figure 9. General structure of the file `chronoamperometry.c`



It can be seen that the file `chronoamperometry.c` does not have a specially complex structure, this changes when we analyze the branch `feature/cyclic_voltammetry `  and the corresponding files.



#### feature/cyclic_voltammetry

In the case of this branch, only the files `cyclic_voltammetry.c` and `cyclic_voltammetry.h` have been created. In the following workflow, figure 10, it can be seen the general code functionality and in figure 11 the more specific code can be seen.

> The timer function of the chronoamperometry has been used for the `mesura_punt` counter.

<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/general_volta.jpeg">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/general_volta.jpeg" width=350 alt="General volta" />
</a>
</p>
> Figure 10. General structure of the file `cyclic_voltammetry.c`.



<p align="center">
<a href="masb-pot-s-ecologiques/Docs/assets/imgs/volta.png">
<img src="masb-pot-s-ecologiques/Docs/assets/imgs/volta.png" width=350 alt="Volta" />
</a>
</p>

> Figure 11. Structure of the file `cyclic_voltammetry.c`.



It can be seen that the dificulty has increased exponentially if we compare the workflows of the two `feature` branches, this is due to the complexity inherent to the voltammetry calculation.



## Obtained results

To be able to test the functionality of the developed program without having the sample and the DAC, the program viSense has been used (with a videotutorial in this [link](https://www.youtube.com/watch?v=UkXToFs8g6Y)), with which the measurement to perform can be configured and also the communication with the microcontroller board can be done as well as the visualization of the results obtained.

To solve the problem of not having the dissolution, a potentiometer has been connected in the form of a tension divisor connected to the analog input, simulating in this way a signal was simulated. After this, once the correct functioning has been made sure, the test in the laboratory has been done.

The results have been correct, the data has been sent, the received signal with the correct timers and an acceptable sensibility have been obtained. Both the chronoamperometry and the cyclic voltammetry have been carried out correctly.

Following, the experimental results can be seen.

### Cyclic voltammetry



### Chronoamperometry





## Conclusions

### Project

This project has served as a way of using the different concepts acquired and to develop a code for a medical application. We have been able to apply the concepts learnt about how to configure a board. Hemos podido aplicar los conocimientos adquiridos de cómo configurar una placa, lectura de pines, tipos de comunicación, uso de timers y su configuración, relaciones entre funciones, y mucho más. Sobretodo hemos adquirido más conocimiento sobre el uso de Git y GitHub para desarrollar programas en equipo de forma cómoda y útil, lo cual será extremadamente útil en nuestro futuro laboral. Para finalizar, nos ha dado las herramientas para saber interpretar _datasheets_ y saber encontrar la información necesaria para poder adaptarnos en distintas situaciones.



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
