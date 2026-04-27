#ifndef MEMORY_STICK_H_
#define MEMORY_STICK_H_

#include "gestor_vglobales.h"

// Inicializacion logs memory_stick

t_log* memory_stick_logger;
t_log* memory_stick_log_debug;
t_config* memory_stick_config;

// Variables globales memory_stick

int fd_memory_stick;
int fd_kernel_memory;
int fd_cpu;

//char* config_path;
//char* TAMANIO_MEMORY_STICK;

char* LOG_LEVEL;
char* IP_KERNEL_MEMORY;
char* PUERTO_ESCUCHA;
char* PUERTO_KERNEL_MEMORY;
int MEMORY_DELAY;

#endif /* MEMORY_STICK_H_ */