#ifndef GESTOR_VGLOBALES_H_
#define GESTOR_VGLOBALES_H_

#include<utils/utils.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>

// Inicializacion logs kernel_memory

extern t_log* kernel_memory_logger;
extern t_log* kernel_memory_log_debug;
extern t_config* kernel_memory_config;

// Variables globales kernel_memory

extern int fd_kernel_memory;
extern int fd_kernel_scheduler;
extern int fd_swap;

extern t_list* lista_cpus;
extern t_list* lista_memory_sticks;
extern pthread_mutex_t mutex_cpus;
extern pthread_mutex_t mutex_memory_sticks;

// extern char* config_path;

extern char* LOG_LEVEL;
extern char* PUERTO_ESCUCHA;
extern int SEGMENT_MAX_SIZE;
extern char* ALLOCATION_STRATEGY;
extern int INSTRUCTION_DELAY;
extern int COMPACTION_DELAY;
extern char* SCRIPTS_BASEPATH;

#endif /* GESTOR_VGLOBALES_H_ */