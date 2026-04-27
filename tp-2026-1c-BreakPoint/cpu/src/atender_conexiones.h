#ifndef ATENDER_CONEXIONES_H_
#define ATENDER_CONEXIONES_H_

#include "gestor_vglobales.h"

void atender_kernel_memory();
void atender_kernel_scheduler();
void atender_memory_stick();

void buscar_ctx_a_memoria(int pid);
void recibir_registros(t_buffer* buffer_respuesta_ctx, int pid);

#endif /* ATENDER_CONEXIONES_H_ */
