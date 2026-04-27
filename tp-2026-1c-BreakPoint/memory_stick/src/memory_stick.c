#include <utils/utils.h>
#include <memory_stick.h>

int main(int argc, char* argv[]) {
    /*
    if (argc < 3) {
        printf("Uso: ./memory_stick [Archivo Config] [Tamanio]\n");
        exit(1);
    }
    config_path = argv[1];
    TAMANIO_MEMORY_STICK = argv[2];
    */
    // Inicializar memory stick
    inicializar_memory_stick();

    // Iniciar servidor de memory stick
    fd_memory_stick = iniciar_servidor(PUERTO_ESCUCHA);
    log_info(memory_stick_logger, "Servidor listo para aceptar conexiones");

    //---------------------------------------- KERNEL MEMORY ----------------------------------------
    // Conectarse como cliente a kernel memory
    log_info(memory_stick_logger,"Creando conexion con Kernel Memory\n");
    fd_kernel_memory = crear_conexion(IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);
    log_info(memory_stick_logger,"Conexion con Kernel Memory exitosa!\n");
    handshake_cliente(fd_kernel_memory, memory_stick_logger);
    int id_modulo_mem = 3;
    send(fd_kernel_memory, &id_modulo_mem, sizeof(int), 0);

    // Atender mensajes de kernel memory
    pthread_t hilo_kernel_memory;
    pthread_create(&hilo_kernel_memory, NULL, (void*)atender_kernel_memory, NULL);
    pthread_detach(hilo_kernel_memory);

    //--------------------------------------------- CPU ---------------------------------------------
    // Esperar conexion de cpu
    log_info(memory_stick_logger, "Esperando conexion de cpu...");
    fd_cpu = esperar_cliente(fd_memory_stick);

    // Atender los mensajes de cpu
    pthread_t hilo_cpu;
    pthread_create(&hilo_cpu, NULL, (void*)atender_cpu, NULL);
    pthread_join(hilo_cpu, NULL);

    return 0;
}

