#include "inicializar_estructuras.h"
#include<utils/utils.h>

void inicializar_cpu() {
    inicializar_logs();
    inicializar_configs();
}

void inicializar_logs() {
    /*
    char log_filename[256];
    snprintf(log_filename, sizeof(log_filename), "cpu_%s.log", CPU_IDENTIFICADOR);
    cpu_logger = log_create(log_filename, "CPU", 1, LOG_LEVEL_INFO);
    */
    cpu_logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_INFO);
	if (cpu_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

    // cpu_log_debug = log_create(log_filename, "CPU", 1, LOG_LEVEL_TRACE);
    cpu_log_debug = log_create("cpu.log", "CPU", 1, LOG_LEVEL_TRACE);
	if (cpu_log_debug == NULL) {
		printf("No se pudo crear el logger de debug\n");
		exit(1);
	}
}

void inicializar_configs() {
    // cpu_config = config_create(config_path);
    cpu_config = config_create("cpu.config");
	if (cpu_config == NULL) {
		printf("No se pudo crear el config\n");
		exit(2);
	}
	
    LOG_LEVEL = config_get_string_value(cpu_config, "LOG_LEVEL");
	IP_KERNEL_MEMORY = config_get_string_value(cpu_config, "IP_KERNEL_MEMORY");
    PUERTO_KERNEL_MEMORY = config_get_string_value(cpu_config, "PUERTO_KERNEL_MEMORY");
    IP_KERNEL_SCHEDULER = config_get_string_value(cpu_config, "IP_KERNEL_SCHEDULER");
    PUERTO_KERNEL_SCHEDULER = config_get_string_value(cpu_config, "PUERTO_KERNEL_SCHEDULER");
    IP_MEMORY_STICK = config_get_string_value(cpu_config, "IP_MEMORY_STICK");
    PUERTO_MEMORY_STICK = config_get_string_value(cpu_config, "PUERTO_MEMORY_STICK");
}