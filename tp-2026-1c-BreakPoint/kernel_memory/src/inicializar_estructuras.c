#include "inicializar_estructuras.h"
#include<utils/utils.h>

void inicializar_kernel_memory() {
    inicializar_logs();
    inicializar_configs();
    lista_cpus = list_create();
    lista_memory_sticks = list_create();
    pthread_mutex_init(&mutex_cpus, NULL);
    pthread_mutex_init(&mutex_memory_sticks, NULL);
}

void inicializar_logs() {
    kernel_memory_logger = log_create("kernel_memory.log", "KERNEL_MEMORY", 1, LOG_LEVEL_INFO);
	if (kernel_memory_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

    kernel_memory_log_debug = log_create("kernel_memory.log", "KERNEL_MEMORY", 1, LOG_LEVEL_TRACE);
	if (kernel_memory_log_debug == NULL) {
		printf("No se pudo crear el logger de debug\n");
		exit(1);
	}
}

void inicializar_configs() {
    //kernel_memory_config = config_create(config_path);
    kernel_memory_config = config_create("kernel_memory.config");
	if (kernel_memory_config == NULL) {
		printf("No se pudo crear el config\n");
		exit(2);
	}

    LOG_LEVEL = config_get_string_value(kernel_memory_config, "LOG_LEVEL");
    PUERTO_ESCUCHA = config_get_string_value(kernel_memory_config, "PUERTO_ESCUCHA");
    SEGMENT_MAX_SIZE = config_get_int_value(kernel_memory_config, "SEGMENT_MAX_SIZE");
    ALLOCATION_STRATEGY = config_get_string_value(kernel_memory_config, "ALLOCATION_STRATEGY");
    INSTRUCTION_DELAY = config_get_int_value(kernel_memory_config, "INSTRUCTION_DELAY");
    COMPACTION_DELAY = config_get_int_value(kernel_memory_config, "COMPACTION_DELAY");
    SCRIPTS_BASEPATH = config_get_string_value(kernel_memory_config, "SCRIPTS_BASEPATH");
}
