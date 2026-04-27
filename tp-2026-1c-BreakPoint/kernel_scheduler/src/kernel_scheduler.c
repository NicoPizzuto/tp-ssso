#include <utils/utils.h>
#include <kernel_scheduler.h>

int main(int argc, char* argv[]) {
    /*
    if (argc < 3) {
        printf("Uso: ./kernel_scheduler [Archivo Config] [Path Proceso Inicial]\n");
        exit(1);
    }
    config_path = argv[1];
    PATH_PROCESO_INICIAL = argv[2];
    */

    pthread_t t2;
    bool esperando_clientes = true;
    int fd_cliente;

    // Inicializar estructuras de kernel scheduler
    inicializar_kernel_scheduler();

    // Iniciar servidor de kernel scheduler
    fd_kernel_scheduler = iniciar_servidor(PUERTO_ESCUCHA);
    log_info(kernel_scheduler_logger, "Servidor listo para aceptar conexiones");


    //---------------------------------------- KERNEL MEMORY ----------------------------------------
    // Conectarse como cliente a kernel memory
    fd_kernel_memory = conectarMemoria();

    // Atender los mensajes de kernel memory
    pthread_t hilo_kernel_memory;
    pthread_create(&hilo_kernel_memory, NULL, (void*)atender_kernel_memory, NULL);
    pthread_detach(hilo_kernel_memory);

    while(esperando_clientes){

        fd_cliente = esperar_cliente(fd_kernel_scheduler);

        int id_modulo;
        recv(fd_cliente, &id_modulo, sizeof(int), MSG_WAITALL);
        switch (id_modulo){
            case 1: {
                int* fd_nuevo = malloc(sizeof(int));
                *fd_nuevo = fd_cliente;
                pthread_mutex_lock(&mutex_cpus);
                list_add(lista_cpus, fd_nuevo);
                pthread_mutex_unlock(&mutex_cpus);
                pthread_t t;
                pthread_create(&t, NULL, (void *)atender_cpu, fd_nuevo);
                pthread_detach(t);
            }
            break;

            case 2:
            fd_io = fd_cliente;
            pthread_create(&t2, NULL, (void *)atender_io, &fd_io);
            break;

            default:
            esperando_clientes = false;
            close(fd_io);
            close(fd_cliente);
            log_warning(kernel_scheduler_logger, "Modulo no reconocido\n");
            break;
        }
    }

    //--------------------------------------------- CPU ---------------------------------------------
    // Esperar conexion de cpu
    // log_info(kernel_scheduler_logger, "Esperando conexion de cpu...");
    // fd_cpu = esperar_cliente(fd_kernel_scheduler);

    // // Atender los mensajes de cpu
    // pthread_t hilo_cpu;
    // pthread_create(&hilo_cpu, NULL, (void*)atender_cpu, NULL);
    // pthread_detach(hilo_cpu);

    // //--------------------------------------------- IO ---------------------------------------------
    // // Esperar conexion de io
    // log_info(kernel_scheduler_logger, "Esperando conexion de io...");
    // fd_io = esperar_cliente(fd_kernel_scheduler, kernel_scheduler_logger, "IO");

    // // Atender los mensajes de io
    // pthread_t hilo_io;
    // pthread_create(&hilo_io, NULL, (void*)atender_io, NULL);
    // pthread_join(hilo_io, NULL);
    
    liberar_conexion(fd_kernel_memory);
    return 0;
}

int conectarMemoria(){
    fd_kernel_memory = crear_conexion(IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);

    if (fd_kernel_memory <= 0)
    {
        log_error(kernel_scheduler_logger, "No se pudo conectar con Memoria");
    }
    else
    {
        log_info(kernel_scheduler_logger, "Conexion con memoria exitosa en socket:%i", fd_kernel_memory);
    }

    handshake_cliente(fd_kernel_memory, kernel_scheduler_logger);

    int id_modulo = 1;
    send(fd_kernel_memory, &id_modulo, sizeof(int), 0);

    return fd_kernel_memory;
}