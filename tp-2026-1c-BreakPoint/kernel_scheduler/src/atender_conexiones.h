#ifndef ATENDER_CONEXIONES_H_
#define ATENDER_CONEXIONES_H_

#include "gestor_vglobales.h"

void atender_kernel_memory();
void atender_cpu(int* fd);
void atender_io();

#endif /* ATENDER_CONEXIONES_H_ */