#ifndef SWAP_H_
#define SWAP_H_

#include "gestor_vglobales.h"

// Inicializacion logs swap

t_log* swap_logger;
t_log* swap_log_debug;
t_config* swap_config;

// Variables globales swap

int fd_kernel_memory;

//char* config_path;

char* LOG_LEVEL;
char* SWAP_FILE_PATH;
int SWAP_FILE_SIZE;
int BLOCK_SIZE;
char* IP_KERNEL_MEMORY;
char* PUERTO_KERNEL_MEMORY;

#endif /* SWAP_H_ */