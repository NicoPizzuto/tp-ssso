#include "atender_conexiones.h"
#include<utils/utils.h>

void atender_kernel_memory() {
    bool control_key = true;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_memory);
		switch (cod_op) {
		case RESPUESTA_CTX:
			t_buffer* buffer_respuesta_ctx = crear_buffer();
			buffer_respuesta_ctx = recibir_buffer(fd_kernel_memory);
			int pid = extraer_int_buffer(buffer_respuesta_ctx);
			recibir_registros(buffer_respuesta_ctx, pid);
			
			log_info(cpu_logger, "Contexto recibido de KERNEL MEMORY");

			free(buffer_respuesta_ctx);
			break;
		case RESPUESTA_PEDIR_INSTRUCCION:
			t_buffer* buffer_respuesta_pedir_instruccion = crear_buffer();
			buffer_respuesta_pedir_instruccion = recibir_buffer(fd_kernel_memory);
			instruccion_a_ejecutar = extraer_string_buffer(buffer_respuesta_pedir_instruccion);
			free(buffer_respuesta_pedir_instruccion);
			
			break;
		case RESPUESTA_CTX_GUARDADO:
			log_info(cpu_logger, "Contexto guardado en KERNEL MEMORY");
			break;
		case -1:
			log_error(cpu_logger, "Desconexion de KERNEL MEMORY");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(cpu_logger, "Operacion desconocida de KERNEL MEMORY");
			break;
		}
	}
}

void atender_kernel_scheduler() {
    bool control_key = true;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_scheduler);
		switch (cod_op) {
		case PROCESO_A_RUNNING:
			t_buffer* buffer_proceso_a_running = crear_buffer();
			buffer_proceso_a_running = recibir_buffer(fd_kernel_scheduler);
			int pid = extraer_int_buffer(buffer_proceso_a_running);
			log_info(cpu_logger, "Proceso con PID %d a RUNNING", pid);

			buscar_ctx_a_memoria(pid);

			free(buffer_proceso_a_running);
			
			break;
		case -1:
			log_error(cpu_logger, "Desconexion de KERNEL SCHEDULER");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(cpu_logger, "Operacion desconocida de KERNEL SCHEDULER");
			break;
		}
	}
}

void atender_memory_stick() {
    bool control_key = true;
    while (control_key) {
		int cod_op = recibir_operacion(fd_memory_stick);
		switch (cod_op) {
		case MENSAJE:
			//
			break;
		case PAQUETE:
			//
			break;
		case -1:
			log_error(cpu_logger, "Desconexion de MEMORY STICK");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(cpu_logger, "Operacion desconocida de MEMORY STICK");
			break;
		}
	}
}