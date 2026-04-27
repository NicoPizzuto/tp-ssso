#ifndef ATENDER_CONEXIONES_H_
#define ATENDER_CONEXIONES_H_

#include "gestor_vglobales.h"

void atender_cpu(int* fd);
void atender_kernel_scheduler();
void atender_memory_stick(int* fd);
void atender_swap();

#endif /* ATENDER_CONEXIONES_H_ */