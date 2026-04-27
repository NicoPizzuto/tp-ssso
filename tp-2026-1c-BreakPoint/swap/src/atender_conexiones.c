#include "atender_conexiones.h"
#include<utils/utils.h>

void atender_kernel_memory() {
	log_info(swap_logger, "SWAP conectado con Kernel Memory");
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
			log_error(swap_logger, "Desconexion de KERNEL MEMORY");
            control_key = 0;
			// return EXIT_FAILURE;
            break;
		default:
			log_warning(swap_logger, "Operacion desconocida de KERNEL MEMORY");
			break;
		}
	}
}