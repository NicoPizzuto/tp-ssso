#include <utils/utils.h>
#include <io.h>

int main(int argc, char* argv[]) {
    /*
    if (argc < 3) {
        printf("Uso: ./io [Archivo Config] [Tipo]\n");
        exit(1);
    }
    config_path = argv[1];
    IO_TIPO = argv[2];
    */
    // Inicializar io
	inicializar_io();

    // Conectarse como cliente a kernel scheduler
    log_info(io_logger,"Creando conexion con Kernel Scheduler\n");
    fd_kernel_scheduler = crear_conexion(IP_KERNEL_SCHEDULER, PUERTO_KERNEL_SCHEDULER);
    log_info(io_logger,"Conexion con Kernel Scheduler exitosa!\n");
    handshake_cliente(fd_kernel_scheduler, io_logger);
    int id_modulo_sch = 2;
    send(fd_kernel_scheduler, &id_modulo_sch, sizeof(int), 0);
    
    // Atender los mensajes de kernel scheduler
    pthread_t hilo_kernel_scheduler;
    pthread_create(&hilo_kernel_scheduler, NULL, (void*)atender_kernel_scheduler, NULL);
    pthread_join(hilo_kernel_scheduler, NULL);
    
    return 0;
}