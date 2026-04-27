#include "inicializar_estructuras.h"
#include<utils/utils.h>

void inicializar_memory_stick() {
    inicializar_logs();
    inicializar_configs();
}

void inicializar_logs() {
    memory_stick_logger = log_create("memory_stick.log", "MEMORY_STICK", 1, LOG_LEVEL_INFO);
	if (memory_stick_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

    memory_stick_log_debug = log_create("memory_stick.log", "MEMORY_STICK", 1, LOG_LEVEL_TRACE);
	if (memory_stick_log_debug == NULL) {
		printf("No se pudo crear el logger de debug\n");
		exit(1);
	}
}

void inicializar_configs() {
    //memory_stick_config = config_create(config_path);
	memory_stick_config = config_create("memory_stick.config");
	if (memory_stick_config == NULL) {
		printf("No se pudo crear el config\n");
		exit(2);
	}

    LOG_LEVEL = config_get_string_value(memory_stick_config, "LOG_LEVEL");
    IP_KERNEL_MEMORY = config_get_string_value(memory_stick_config, "IP_KERNEL_MEMORY");
    PUERTO_ESCUCHA = config_get_string_value(memory_stick_config, "PUERTO_ESCUCHA");
    PUERTO_KERNEL_MEMORY = config_get_string_value(memory_stick_config, "PUERTO_KERNEL_MEMORY");
    MEMORY_DELAY = config_get_int_value(memory_stick_config, "MEMORY_DELAY");
}

