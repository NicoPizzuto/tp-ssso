#ifndef KERNEL_MEMORY_H_
#define KERNEL_MEMORY_H_

#include<stdio.h>
#include<stdlib.h>
#include "gestor_vglobales.h"
#include "inicializar_estructuras.h"
#include "atender_conexiones.h"

int atenderKernelScheduler();
int atenderMemoryStick();
int atenderSWAP();

#endif /* KERNEL_MEMORY_H_ */