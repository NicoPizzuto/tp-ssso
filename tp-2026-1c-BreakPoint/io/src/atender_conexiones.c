#include "atender_conexiones.h"
#include<utils/utils.h>

void atender_kernel_scheduler() {
	log_info(io_logger, "IO conectada con Kernel Scheduler");
    bool control_key = true;
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
			log_error(io_logger, "Desconexion de KERNEL SCHEDULER");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(io_logger, "Operacion desconocida de KERNEL SCHEDULER");
			break;
		}
	}
}