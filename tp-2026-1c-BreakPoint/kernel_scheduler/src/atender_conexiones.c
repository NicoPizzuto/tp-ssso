#include "atender_conexiones.h"
#include<utils/utils.h>

void atender_kernel_memory() {
	log_info(kernel_scheduler_logger, "Kernel Scheduler conectada con Kernel Memory");
    bool control_key = true;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_memory);
		switch (cod_op) {
		case MENSAJE:
			//
			break;
		case PAQUETE:
			//
			break;
		case -1:
			log_error(kernel_scheduler_logger, "Desconexion de KERNEL MEMORY");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(kernel_scheduler_logger, "Operacion desconocida de KERNEL MEMORY");
			break;
		}
	}
}

void atender_cpu(int* fd) {
    log_info(kernel_scheduler_logger, "Kernel Scheduler conectada con CPU (fd=%d)", *fd);
    bool control_key = true;
    while (control_key) {
        int cod_op = recibir_operacion(*fd);
        switch (cod_op) {
        case MENSAJE:
            break;
        case PAQUETE:
            break;
        case -1:
            log_error(kernel_scheduler_logger, "Desconexion de CPU (fd=%d)", *fd);
            control_key = false;
            break;
        default:
            log_warning(kernel_scheduler_logger, "Operacion desconocida de CPU");
            break;
        }
    }
    pthread_mutex_lock(&mutex_cpus);
    for (int i = 0; i < list_size(lista_cpus); i++) {
        if (list_get(lista_cpus, i) == fd) {
            list_remove(lista_cpus, i);
            break;
        }
    }
    pthread_mutex_unlock(&mutex_cpus);
    free(fd);
}

void atender_io() {
	log_info(kernel_scheduler_logger, "Kernel Scheduler conectada con IO");
    bool control_key = true;
    while (control_key) {
		int cod_op = recibir_operacion(fd_io);
		switch (cod_op) {
		case MENSAJE:
			//
			break;
		case PAQUETE:
			//
			break;
		case -1:
			log_error(kernel_scheduler_logger, "Desconexion de I/O");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(kernel_scheduler_logger, "Operacion desconocida de I/O");
			break;
		}
	}
}