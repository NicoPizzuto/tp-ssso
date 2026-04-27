#ifndef CPU_H_
#define CPU_H_

#include<stdio.h>
#include<stdlib.h>
#include "gestor_vglobales.h"
#include "inicializar_estructuras.h"
#include "atender_conexiones.h"

typedef struct {
    int PID;
    uint32_t PC;
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t SI;
    uint32_t DI;

} t_registro_cpu;

t_registro_cpu* registro_cpu_temp;
bool flag_fin_de_ciclo;
bool flag_syscall;
char* instruccion_a_ejecutar;
char* motivo_desalojo;


//FUNCIONES
void buscar_ctx_a_memoria(int pid);
void recibir_registros(t_buffer* buffer_respuesta_ctx, int pid);
void ciclo_de_instruccion();
void ciclo_de_instruccion_fetch();
void pedir_instruccion_a_kernel_memory(int pid, int pc);
void ciclo_de_instruccion_decode();
bool validar_instruccion_recibida(char* instruccion_a_validar);
void ciclo_de_instruccion_execute();
void ciclo_de_instruccion_check_interrupt();
uint8_t* detectar_registro_uint8(char* registro_a_detectar);
uint32_t* detectar_registro_uint32(char* registro_a_detectar);
void enviar_ctx_a_kernel_memory(t_registro_cpu* registro_cpu_temp);
void devolver_control_a_kernel(t_registro_cpu* registro_cpu_temp, char* instruccion_ejecutada, char* parametro1, char* parametro2);
op_code enumeracion_syscalls(char* motivo_desalojo);

#endif /* CPU_H_ */