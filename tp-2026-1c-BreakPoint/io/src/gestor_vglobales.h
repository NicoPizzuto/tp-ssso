#ifndef GESTOR_VGLOBALES_H_
#define GESTOR_VGLOBALES_H_

#include<utils/utils.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>

// Inicializacion logs io

extern t_log* io_logger;
extern t_log* io_log_debug;
extern t_config* io_config;

// Variables globales io

extern int fd_kernel_scheduler;

// extern char* config_path;
// extern char* IO_TIPO;

extern char* LOG_LEVEL;
extern char* IP_KERNEL_SCHEDULER;
extern char* PUERTO_KERNEL_SCHEDULER;

#endif /* GESTOR_VGLOBALES_H_ */