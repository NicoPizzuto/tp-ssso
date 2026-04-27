#ifndef CPU_H_
#define CPU_H_

#include "gestor_vglobales.h"

// Inicializacion logs cpu

t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

// Variables globales cpu

int fd_kernel_memory;
int fd_kernel_scheduler;
int fd_memory_stick;

// char* config_path;
// char* CPU_IDENTIFICADOR;

char* LOG_LEVEL;
char* IP_KERNEL_MEMORY;
char* PUERTO_KERNEL_MEMORY;
char* IP_KERNEL_SCHEDULER;
char* PUERTO_KERNEL_SCHEDULER;
char* IP_MEMORY_STICK;
char* PUERTO_MEMORY_STICK;

#endif /* CPU_H_ */