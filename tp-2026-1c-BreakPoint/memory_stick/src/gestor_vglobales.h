#ifndef GESTOR_VGLOBALES_H_
#define GESTOR_VGLOBALES_H_

#include<utils/utils.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>

// Inicializacion logs memory_stick

extern t_log* memory_stick_logger;
extern t_log* memory_stick_log_debug;
extern t_config* memory_stick_config;

// Variables globales memory_stick

extern int fd_memory_stick;
extern int fd_kernel_memory;
extern int fd_cpu;

//extern char* config_path;
//extern char* TAMANIO_MEMORY_STICK;

extern char* LOG_LEVEL;
extern char* IP_KERNEL_MEMORY;
extern char* PUERTO_ESCUCHA;
extern char* PUERTO_KERNEL_MEMORY;
extern int MEMORY_DELAY;

#endif /* GESTOR_VGLOBALES_H_ */
