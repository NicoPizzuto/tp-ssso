#include "atender_conexiones.h"
#include<utils/utils.h>

void atender_cpu(int* fd) {
    log_info(kernel_memory_logger, "Kernel Memory conectada con CPU");
    bool control_key = true;
    while (control_key) {
        int cod_op = recibir_operacion(*fd);
        switch (cod_op) {
        case MENSAJE:
            break;
        case PAQUETE:
            break;
        case -1:
            log_error(kernel_memory_logger, "Desconexion de CPU");
            control_key = false;
            break;
        default:
            log_warning(kernel_memory_logger, "Operacion desconocida de CPU");
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

void atender_kernel_scheduler() {
    bool control_key = 1;
	log_info(kernel_memory_logger, "Kernel Memory conectada con Kernel Scheduler");
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_scheduler);
		switch (cod_op) {
		case MENSAJE:
			//
			break;
		case PAQUETE:
			//
			break;
		case -1:
			log_error(kernel_memory_logger, "Desconexion de KERNEL SCHEDULER");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(kernel_memory_logger, "Operacion desconocida de KERNEL SCHEDULER");
			break;
		}
	}
}

void atender_memory_stick(int* fd) {
    log_info(kernel_memory_logger, "Kernel Memory conectada con Memory Stick");
    bool control_key = true;
    while (control_key) {
        int cod_op = recibir_operacion(*fd);
        switch (cod_op) {
        case MENSAJE:
            break;
        case PAQUETE:
            break;
        case -1:
            log_error(kernel_memory_logger, "Desconexion de MEMORY STICK");
            control_key = false;
            break;
        default:
            log_warning(kernel_memory_logger, "Operacion desconocida de MEMORY STICK");
            break;
        }
    }
    pthread_mutex_lock(&mutex_memory_sticks);
    for (int i = 0; i < list_size(lista_memory_sticks); i++) {
        if (list_get(lista_memory_sticks, i) == fd) {
            list_remove(lista_memory_sticks, i);
            break;
        }
    }
    pthread_mutex_unlock(&mutex_memory_sticks);
    free(fd);
}

void atender_swap() {
    bool control_key = 1;
	log_info(kernel_memory_logger, "Kernel Memory conectada con SWAP");
    while (control_key) {
		int cod_op = recibir_operacion(fd_swap);
		switch (cod_op) {
		case MENSAJE:
			//
			break;
		case PAQUETE:
			//
			break;
		case -1:
			log_error(kernel_memory_logger, "Desconexion de SWAP");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(kernel_memory_logger, "Operacion desconocida de SWAP");
			break;
		}
	}
}