#ifndef GESTOR_VGLOBALES_H_
#define GESTOR_VGLOBALES_H_

#include<utils/utils.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>

// Inicializacion logs cpu

extern t_log* cpu_logger;
extern t_log* cpu_log_debug;
extern t_config* cpu_config;

// Variables globales cpu

extern int fd_kernel_memory;
extern int fd_kernel_scheduler;
extern int fd_memory_stick;

// extern char* config_path;
// extern char* CPU_IDENTIFICADOR;

extern char* LOG_LEVEL;
extern char* IP_KERNEL_MEMORY;
extern char* PUERTO_KERNEL_MEMORY;
extern char* IP_KERNEL_SCHEDULER;
extern char* PUERTO_KERNEL_SCHEDULER;
extern char* IP_MEMORY_STICK;
extern char* PUERTO_MEMORY_STICK;

extern bool flag_fin_de_ciclo;
extern bool flag_syscall;
extern char* motivo_desalojo;
extern char* instruccion_a_ejecutar;

#endif /* GESTOR_VGLOBALES_H_ */