#include <utils/utils.h>
#include <swap.h>

int main(int argc, char* argv[]) {
    /*
    if (argc < 2) {
        printf("Uso: ./swap [Archivo Config]\n");
        exit(1);
    }
    config_path = argv[1];
    */
    // Inicializar swap
	inicializar_swap();

    // Conectarse como cliente a kernel memory
    log_info(swap_logger,"Creando conexion con Kernel Memory\n");
    fd_kernel_memory = crear_conexion(IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);
    log_info(swap_logger,"Conexion con Kernel Memory exitosa!\n");
    handshake_cliente(fd_kernel_memory, swap_logger);
    int id_modulo_mem = 4;
    send(fd_kernel_memory, &id_modulo_mem, sizeof(int), 0);

    // Atender los mensajes de kernel memory
    pthread_t hilo_kernel_memory;
    pthread_create(&hilo_kernel_memory, NULL, (void*)atender_kernel_memory, NULL);
    pthread_join(hilo_kernel_memory, NULL);
    
    return 0;
}