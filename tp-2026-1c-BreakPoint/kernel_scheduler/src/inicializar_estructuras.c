#include "inicializar_estructuras.h"
#include<utils/utils.h>

void inicializar_kernel_scheduler() {
    inicializar_logs();
    inicializar_configs();
    lista_cpus = list_create();
    pthread_mutex_init(&mutex_cpus, NULL);
}

void inicializar_logs() {
    kernel_scheduler_logger = log_create("kernel_scheduler.log", "KERNEL_SCHEDULER", 1, LOG_LEVEL_INFO);
	if (kernel_scheduler_logger == NULL) {
		printf("No se pudo crear el logger\n");
		exit(1);
	}

    kernel_scheduler_log_debug = log_create("kernel_scheduler.log", "KERNEL_SCHEDULER", 1, LOG_LEVEL_TRACE);
	if (kernel_scheduler_log_debug == NULL) {
		printf("No se pudo crear el logger de debug\n");
		exit(1);
	}
}

void inicializar_configs() {
    //kernel_scheduler_config = config_create(config_path);
    kernel_scheduler_config = config_create("kernel_scheduler.config");
	if (kernel_scheduler_config == NULL) {
		printf("No se pudo crear el config\n");
		exit(2);
	}

    LOG_LEVEL = config_get_string_value(kernel_scheduler_config, "LOG_LEVEL");
    IP_KERNEL_MEMORY = config_get_string_value(kernel_scheduler_config, "IP_KERNEL_MEMORY");
    PUERTO_ESCUCHA = config_get_string_value(kernel_scheduler_config, "PUERTO_ESCUCHA");
    PUERTO_KERNEL_MEMORY = config_get_string_value(kernel_scheduler_config, "PUERTO_KERNEL_MEMORY");
    PLANIFICATION_ALGORITHM = config_get_string_value(kernel_scheduler_config, "PLANIFICATION_ALGORITHM");
    QUEUES_ALGORITHMS = config_get_array_value(kernel_scheduler_config, "QUEUES_ALGORITHMS");
    RR_QUANTUM = config_get_int_value(kernel_scheduler_config, "RR_QUANTUM");
    QUEUE_PREEMPTION = config_get_string_value(kernel_scheduler_config, "QUEUE_PREEMPTION");
    SUSPENSION_TIMEOUT = config_get_int_value(kernel_scheduler_config, "SUSPENSION_TIMEOUT");
}