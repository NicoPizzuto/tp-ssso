#include <utils/utils.h>
#include <kernel_memory.h>

int main(int argc, char* argv[]) {
    /*
    if (argc < 2) {
        printf("Uso: ./kernel_memory [Archivo Config]\n");
        exit(1);
    }
    config_path = argv[1];
    */
    bool esperando_clientes = true;
    int fd_cliente;

    // Inicializar estructuras de kernel memory
    inicializar_kernel_memory();

    // Iniciar servidor de kernel memory
    fd_kernel_memory = iniciar_servidor(PUERTO_ESCUCHA);
    log_info(kernel_memory_logger, "Servidor listo para aceptar conexiones");
    
    pthread_t t1, t4;

    while(esperando_clientes){

        fd_cliente = esperar_cliente(fd_kernel_memory);

        int id_modulo;
        recv(fd_cliente, &id_modulo, sizeof(int), MSG_WAITALL);
        switch (id_modulo){
            case 1:
                fd_kernel_scheduler = fd_cliente;
                pthread_create(&t1, NULL, (void *)atender_kernel_scheduler, &fd_kernel_scheduler);
            break;

            case 2: {
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

            case 3: {
                int* fd_nuevo = malloc(sizeof(int));
                *fd_nuevo = fd_cliente;
                pthread_mutex_lock(&mutex_memory_sticks);
                list_add(lista_memory_sticks, fd_nuevo);
                pthread_mutex_unlock(&mutex_memory_sticks);
                pthread_t t;
                pthread_create(&t, NULL, (void *)atender_memory_stick, fd_nuevo);
                pthread_detach(t);
            }
            break;

            case 4:
            fd_swap = fd_cliente;
            pthread_create(&t4, NULL, (void *)atender_swap, &fd_swap);
            break;

            default:
            esperando_clientes = false;
            close(fd_kernel_scheduler);
            close(fd_cliente);
            log_warning(kernel_memory_logger, "Modulo no reconocido\n");
            break;
        }
    }

    return 0;
}
