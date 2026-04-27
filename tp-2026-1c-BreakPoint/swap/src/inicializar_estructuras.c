# include "inicializar_estructuras.h"
#include<utils/utils.h>

void inicializar_swap() {
    inicializar_logs();
    inicializar_configs();
}

void inicializar_logs() {
    swap_logger = log_create("swap.log", "SWAP", 1, LOG_LEVEL_INFO);
	if (swap_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

    swap_log_debug = log_create("swap.log", "SWAP", 1, LOG_LEVEL_TRACE);
	if (swap_log_debug == NULL) {
		printf("No se pudo crear el logger de debug\n");
		exit(1);
	}
}

void inicializar_configs() {
    //swap_config = config_create(config_path);
	swap_config = config_create("swap.config");
	if (swap_config == NULL) {
		printf("No se pudo crear el config\n");
		exit(2);
	}

    LOG_LEVEL = config_get_string_value(swap_config, "LOG_LEVEL");
    IP_KERNEL_MEMORY = config_get_string_value(swap_config, "IP_KERNEL_MEMORY");
    PUERTO_KERNEL_MEMORY = config_get_string_value(swap_config, "PUERTO_KERNEL_MEMORY");
    SWAP_FILE_PATH = config_get_string_value(swap_config, "SWAP_FILE_PATH");
    SWAP_FILE_SIZE = config_get_int_value(swap_config, "SWAP_FILE_SIZE");
    BLOCK_SIZE = config_get_int_value(swap_config, "BLOCK_SIZE");
}