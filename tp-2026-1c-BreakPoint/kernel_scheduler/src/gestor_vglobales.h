#ifndef GESTOR_VGLOBALES_H_
#define GESTOR_VGLOBALES_H_

#include<utils/utils.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>

// Inicializacion logs kernel_scheduler

extern t_log* kernel_scheduler_logger;
extern t_log* kernel_scheduler_log_debug;
extern t_config* kernel_scheduler_config;

// Variables globales kernel_scheduler

extern int fd_kernel_scheduler;
extern int fd_kernel_memory;
extern int fd_io;

extern t_list* lista_cpus;
extern pthread_mutex_t mutex_cpus;

//extern char* config_path;
//extern char* PATH_PROCESO_INICIAL;

extern char* LOG_LEVEL;
extern char* IP_KERNEL_MEMORY;
extern char* PUERTO_ESCUCHA;
extern char* PUERTO_KERNEL_MEMORY;
extern char* PLANIFICATION_ALGORITHM;
extern char** QUEUES_ALGORITHMS;
extern int RR_QUANTUM;
extern char* QUEUE_PREEMPTION;
extern int SUSPENSION_TIMEOUT;

#endif /* GESTOR_VGLOBALES_H_ */