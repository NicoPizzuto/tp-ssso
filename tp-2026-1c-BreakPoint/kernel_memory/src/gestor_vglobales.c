#ifndef KERNEL_MEMORY_H_
#define KERNEL_MEMORY_H_

#include "gestor_vglobales.h"

// Inicializacion logs kernel_memory

t_log* kernel_memory_logger;
t_log* kernel_memory_log_debug;
t_config* kernel_memory_config;

// Variables globales kernel_memory

int fd_kernel_memory;
int fd_kernel_scheduler;
t_list* lista_cpus;
t_list* lista_memory_sticks;
pthread_mutex_t mutex_cpus;
pthread_mutex_t mutex_memory_sticks;
int fd_swap;

// char* config_path;

char* LOG_LEVEL;
char* PUERTO_ESCUCHA;
int SEGMENT_MAX_SIZE;
char* ALLOCATION_STRATEGY;
int INSTRUCTION_DELAY;
int COMPACTION_DELAY;
char* SCRIPTS_BASEPATH;

#endif /* KERNEL_MEMORY_H_ */