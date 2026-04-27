#ifndef IO_H_
#define IO_H_

#include "gestor_vglobales.h"

// Inicializacion logs io

t_log* io_logger;
t_log* io_log_debug;
t_config* io_config;

// Variables globales io

int fd_kernel_scheduler;

// char* config_path;
// char* IO_TIPO;

char* LOG_LEVEL;
char* IP_KERNEL_SCHEDULER;
char* PUERTO_KERNEL_SCHEDULER;

#endif /* IO_H_ */