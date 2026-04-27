#ifndef KERNEL_SCHEDULER_H_
#define KERNEL_SCHEDULER_H_

#include "gestor_vglobales.h"

// Inicializacion logs kernel_scheduler

t_log* kernel_scheduler_logger;
t_log* kernel_scheduler_log_debug;
t_config* kernel_scheduler_config;

// Variables globales kernel_scheduler

int fd_kernel_scheduler;
int fd_kernel_memory;
t_list* lista_cpus;
pthread_mutex_t mutex_cpus;
int fd_io;

//char* config_path;
//char* PATH_PROCESO_INICIAL;

char* LOG_LEVEL;
char* IP_KERNEL_MEMORY;
char* PUERTO_ESCUCHA;
char* PUERTO_KERNEL_MEMORY;
char* PLANIFICATION_ALGORITHM;
char** QUEUES_ALGORITHMS;
int RR_QUANTUM;
char* QUEUE_PREEMPTION;
int SUSPENSION_TIMEOUT;

#endif /* KERNEL_SCHEDULER_H_ */