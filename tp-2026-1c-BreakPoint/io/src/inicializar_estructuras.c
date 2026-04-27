# include "inicializar_estructuras.h"
#include<utils/utils.h>

void inicializar_io() {
    inicializar_logs();
    inicializar_configs();
}

void inicializar_logs() {
	/*
    char log_filename[256];
    snprintf(log_filename, sizeof(log_filename), "io_%s.log", IO_TIPO);
    io_logger = log_create(log_filename, "IO", 1, LOG_LEVEL_INFO);
	*/
	io_logger = log_create("io.log", "IO", 1, LOG_LEVEL_INFO);
	if (io_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

    // io_log_debug = log_create(log_filename, "IO", 1, LOG_LEVEL_TRACE);
	io_log_debug = log_create("io.log", "IO", 1, LOG_LEVEL_TRACE);
	if (io_log_debug == NULL) {
		printf("No se pudo crear el logger de debug\n");
		exit(1);
	}
}

void inicializar_configs() {
    // io_config = config_create(config_path);
	io_config = config_create("io.config");
	if (io_config == NULL) {
		printf("No se pudo crear el config\n");
		exit(2);
	}

    LOG_LEVEL = config_get_string_value(io_config, "LOG_LEVEL");
	IP_KERNEL_SCHEDULER = config_get_string_value(io_config, "IP_KERNEL_SCHEDULER");
	PUERTO_KERNEL_SCHEDULER = config_get_string_value(io_config, "PUERTO_KERNEL_SCHEDULER");
}