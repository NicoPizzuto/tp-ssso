#ifndef GESTOR_VGLOBALES_H_
#define GESTOR_VGLOBALES_H_

#include<utils/utils.h>
#include<pthread.h>
#include<commons/log.h>
#include<commons/config.h>

// Inicializacion logs swap

extern t_log* swap_logger;
extern t_log* swap_log_debug;
extern t_config* swap_config;

// Variables globales swap

extern int fd_kernel_memory;

//extern char* config_path;

extern char* LOG_LEVEL;
extern char* SWAP_FILE_PATH;
extern int SWAP_FILE_SIZE;
extern int BLOCK_SIZE;
extern char* IP_KERNEL_MEMORY;
extern char* PUERTO_KERNEL_MEMORY;

#endif /* GESTOR_VGLOBALES_H_ */