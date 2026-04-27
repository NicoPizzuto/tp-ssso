#include <utils/utils.h>

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
	socket_cliente = socket(server_info->ai_family,
                         server_info->ai_socktype,
                         server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

int iniciar_servidor(char *puerto)
{
	struct addrinfo hints, *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, puerto, &hints, &server_info) != 0)
	{
		perror("Error en getaddrinfo");
		return -1;
	}

	int socket_servidor = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (socket_servidor == -1)
	{
		perror("Error al crear socket del servidor");
		freeaddrinfo(server_info);
		return -1;
	}

	if (bind(socket_servidor, server_info->ai_addr, server_info->ai_addrlen) == -1)
	{
		perror("Error en bind");
		close(socket_servidor);
		freeaddrinfo(server_info);
		return -1;
	}

	if (listen(socket_servidor, SOMAXCONN) == -1)
	{
		perror("Error en listen");
		close(socket_servidor);
		freeaddrinfo(server_info);
		return -1;
	}

	freeaddrinfo(server_info);
	return socket_servidor;
}


int esperar_cliente(int socket_servidor)
{
	// aceptar cliente-hilo padre
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	if (socket_cliente == -1)
	{
		perror("Error al aceptar conexión del cliente");
		return -1;
	}
	// atiende el cliente-dif hilos
	uint32_t handshake;
	uint32_t resultOk = 0;
	uint32_t resultError = -1;
	size_t bytes = recv(socket_cliente, &handshake, sizeof(int32_t), MSG_WAITALL);

	if (bytes == -1)
	{
		perror("Error al recibir handshake del cliente");
		close(socket_cliente);
		return -1;
	}

	if (handshake == 1)
	{
		bytes = send(socket_cliente, &resultOk, sizeof(int32_t), 0);
		if (bytes == -1)
		{
			perror("Error al enviar handshake de respuesta al cliente");
			close(socket_cliente);
			return -1;
		}
	}
	else
	{
		bytes = send(socket_cliente, &resultError, sizeof(int32_t), 0);
		if (bytes == -1)
		{
			perror("Error al enviar handshake de error al cliente");
			close(socket_cliente);
			return -1;
		}
	}

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) // recv BLOQUEANTE
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

void handshake_cliente(int socket_cliente, t_log *logger)
{
	size_t bytes;

	int32_t handshake = 1;
	int32_t result;
	uint32_t resultOk = 0;
	bytes = send(socket_cliente, &handshake, sizeof(int32_t), 0);
	bytes = recv(socket_cliente, &result, sizeof(int32_t), MSG_WAITALL);

	if (result == resultOk)
	{
		log_info(logger, "Handshake exitoso");
	}
	else
	{
		log_info(logger, "Handshake fallido");
	}
}

//	-------------------------	BUFFERS -------------------------
t_buffer* crear_buffer()
{
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = 0;
	buffer->stream = NULL;
	return buffer;
}

void eliminar_buffer(t_buffer* buffer){

	while(buffer->stream!=NULL){
		free(buffer->stream);
	}
	if(buffer!= NULL){
		free(buffer);
	}
	
}

void agregar_al_buffer(t_buffer* buff, void* stream, int tamanio)
{
	if(buff->size == 0){
		buff->stream = malloc(sizeof(int) + tamanio);
		memcpy(buff->stream, &tamanio, sizeof(int));
		memcpy(buff->stream + sizeof(int), stream, tamanio);
	}else{
	
		buff->stream = realloc(buff->stream, buff->size + tamanio + sizeof(int));
		memcpy(buff->stream + buff->size, &tamanio, sizeof(int));
		memcpy(buff->stream + buff->size + sizeof(int), stream, tamanio);
	}
	buff->size += sizeof(int);
	buff->size += tamanio;
}

void cargar_int_al_buffer(t_buffer* buff, int tamanio)
{
	agregar_al_buffer(buff, &tamanio, sizeof(int));
}

void cargar_uint32_t_al_buffer(t_buffer* buff, uint32_t tamanio)
{
	agregar_al_buffer(buff, &tamanio, sizeof(uint32_t));
}

void cargar_void_al_buffer(t_buffer* buff, void* tamanio)
{
	agregar_al_buffer(buff, tamanio, sizeof(tamanio));
}

void cargar_uint8_t_al_buffer(t_buffer* buff, uint8_t tamanio)
{
	agregar_al_buffer(buff, &tamanio, sizeof(uint8_t));
}

void cargar_string_al_buffer(t_buffer* buff, char* tamanio)
{
	agregar_al_buffer(buff, tamanio, strlen(tamanio)+1);
}


t_buffer* recibir_buffer(int socket_cliente)
{
	t_buffer* buffer = malloc(sizeof(t_buffer));
	if(	recv(socket_cliente, &(buffer->size), sizeof(int), MSG_WAITALL)>0){
		buffer->stream = malloc(buffer->size);
		if(recv(socket_cliente, buffer->stream, buffer->size, MSG_WAITALL)>0){
			return buffer;
		}else{
			printf("Error al recibir el void stream del buffer del socket cliente");
			exit(EXIT_FAILURE);
		}
	}else{
		printf("Error al recibir el tamaño del buffer del socket cliente");
		exit(EXIT_FAILURE);		
	}
	return buffer;
}


void* extraer_buffer(t_buffer* buff) {
    if (buff->size <= 0) {
        perror("ERROR al intentar extraer un contenido de t_buffer vacio o con tamaño negativo");
        exit(EXIT_FAILURE);
    }

    int tamanio;
    memcpy(&tamanio, buff->stream, sizeof(int));

    if (tamanio > buff->size - sizeof(int)) {
        perror("ERROR el tamaño a extraer es mayor que el tamaño del buffer");
        exit(EXIT_FAILURE);
    }

    void *caja = malloc(tamanio);
    if (caja == NULL) {
        perror("ERROR al asignar memoria para extraer el contenido del buffer");
        exit(EXIT_FAILURE);
    }

    memcpy(caja, (char*)buff->stream + sizeof(int), tamanio);

    int un_nuevo_tamanio = buff->size - sizeof(int) - tamanio;
    if (un_nuevo_tamanio < 0) {
        perror("ERROR DEL NUEVO TAMANIO - Contiene un tamaño negativo");
        exit(EXIT_FAILURE);
    }

    void* nuevo_stream = malloc(un_nuevo_tamanio);
    if (nuevo_stream == NULL && un_nuevo_tamanio > 0) {
        perror("ERROR al asignar memoria para el nuevo stream del buffer");
        exit(EXIT_FAILURE);
    }

    memcpy(nuevo_stream, (char*)buff->stream + sizeof(int) + tamanio, un_nuevo_tamanio);
    free(buff->stream);
    buff->size = un_nuevo_tamanio;
    buff->stream = nuevo_stream;

    return caja;
}

int extraer_int_buffer(t_buffer* buff) {
    int* entero = (int*)extraer_buffer(buff);
    int valor_retorno = *entero;
    free(entero);
    return valor_retorno;
}
void* extraer_void_buffer(t_buffer* buff){
	void* un_void = extraer_buffer(buff);
	return un_void;
}

char* extraer_string_buffer(t_buffer* buff){
	char* un_string = extraer_buffer(buff);
	return un_string;
}

uint32_t extraer_uint32_t_buffer(t_buffer* buff){
	uint32_t* valor = extraer_buffer(buff);
	uint32_t valor_retorno = *valor;
	free(valor);
	return valor_retorno;
}

uint8_t extraer_uint8_t_buffer(t_buffer* buff){
	uint8_t* valor = extraer_buffer(buff);
	uint8_t valor_retorno = *valor;
	free(valor);
	return valor_retorno;
}


// -------------------------	PAQUETES -------------------------
t_paquete* crear_super_paquete(op_code codigo, t_buffer* buff)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo;
	paquete->buffer = buff;
	return paquete;
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void eliminar_paquete(t_paquete* paquete)
{	
	//eliminar_buffer(paquete->buffer);
	free(paquete);
}