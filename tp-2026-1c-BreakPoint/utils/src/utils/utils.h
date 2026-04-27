#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdlib.h>
#include <stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<commons/log.h>
#include<string.h>
#include <commons/string.h>

typedef enum
{
	MENSAJE,
	PAQUETE,
	//CPU
	PROCESO_A_RUNNING,
	RESPUESTA_CTX,
	PEDIR_INSTRUCCION_KERNEL_MEMORY,
	RESPUESTA_PEDIR_INSTRUCCION,
	ENVIAR_CTX_MODIFICADO_KERNEL_MEMORY,
	RESPUESTA_CTX_GUARDADO,


	MUTEX_CREATE,
	MUTEX_LOCK,
	MUTEX_UNLOCK,
	SLEEP,
	INIT_PROC,
	


	//KERNEL SCHEDULER


	//IO


}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

int iniciar_servidor(char* puerto);
int esperar_cliente(int socket_servidor);
int recibir_operacion(int socket_cliente);

t_buffer* crear_buffer();
void eliminar_buffer(t_buffer* buffer);
void agregar_al_buffer(t_buffer* buff, void* stream, int tamanio);
t_buffer* recibir_buffer(int socket_cliente);
void* extraer_buffer(t_buffer* buff);
void cargar_int_al_buffer(t_buffer* buff, int tamanio);
void cargar_uint32_t_al_buffer(t_buffer* buff, uint32_t tamanio);
void cargar_void_al_buffer(t_buffer* buff, void* tamanio);
void cargar_uint8_t_al_buffer(t_buffer* buff, uint8_t tamanio);
void cargar_string_al_buffer(t_buffer* buff, char* tamanio);
int extraer_int_buffer(t_buffer* buff);
void* extraer_void_buffer(t_buffer* buff);
char* extraer_string_buffer(t_buffer* buff);
uint32_t extraer_uint32_t_buffer(t_buffer* buff);
uint8_t extraer_uint8_t_buffer(t_buffer* buff);
t_paquete* crear_super_paquete(op_code codigo, t_buffer* buff);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, int bytes);

//-------------------------CONEXIONES CLIENTES---------------------------------
int crear_conexion(char *ip, char* puerto);
void liberar_conexion(int socket_cliente);
void handshake_cliente(int socket_cliente, t_log *logger);

#endif
