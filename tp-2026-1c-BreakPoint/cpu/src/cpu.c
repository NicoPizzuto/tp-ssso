#include <utils/utils.h>
#include <cpu.h>

int main(int argc, char* argv[]) {
    /*
    if (argc < 3) {
        printf("Uso: ./cpu [Archivo Config] [Identificador]\n");
        exit(1);
    }
    config_path = argv[1];
    CPU_IDENTIFICADOR = argv[2];
    */
    // Inicializar cpu
    inicializar_cpu();


    // Conectarse como cliente a kernel memory
    log_info(cpu_logger,"Creando conexion con Kernel Memory\n");
    fd_kernel_memory = crear_conexion(IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);
    log_info(cpu_logger,"Conexion con Kernel Memory exitosa!\n");
    handshake_cliente(fd_kernel_memory, cpu_logger);
    int id_modulo_mem = 2;
    send(fd_kernel_memory, &id_modulo_mem, sizeof(int), 0);

    // Conectarse como cliente a kernel scheduler
    log_info(cpu_logger,"Creando conexion con Kernel Scheduler\n");
    fd_kernel_scheduler = crear_conexion(IP_KERNEL_SCHEDULER, PUERTO_KERNEL_SCHEDULER);
    log_info(cpu_logger,"Conexion con Kernel Scheduler exitosa!\n");
    handshake_cliente(fd_kernel_scheduler, cpu_logger);
    int id_modulo_sch = 1;
    send(fd_kernel_scheduler, &id_modulo_sch, sizeof(int), 0);

    // Conectarse como cliente a memory stick
    log_info(cpu_logger,"Creando conexion con Memory Stick\n");
    fd_memory_stick = crear_conexion(IP_MEMORY_STICK, PUERTO_MEMORY_STICK);
    log_info(cpu_logger,"Conexion con Memory Stick exitosa!\n");
    handshake_cliente(fd_memory_stick, cpu_logger);

    // Atender los mensajes de kernel memory
    pthread_t hilo_kernel_memory;
    pthread_create(&hilo_kernel_memory, NULL, (void*)atender_kernel_memory, NULL);
    pthread_detach(hilo_kernel_memory);

    // Atender los mensajes de kernel scheduler
    pthread_t hilo_kernel_scheduler;
    pthread_create(&hilo_kernel_scheduler, NULL, (void*)atender_kernel_scheduler, NULL);
    pthread_detach(hilo_kernel_scheduler);

    // Atender los mensajes de memory stick
    pthread_t hilo_memory_stick;
    pthread_create(&hilo_memory_stick, NULL, (void*)atender_memory_stick, NULL);
    pthread_join(hilo_memory_stick, NULL);

    return 0;
}

void buscar_ctx_a_memoria(int pid) {
    t_buffer* buffer_buscar_ctx = crear_buffer();
    cargar_int_al_buffer(buffer_buscar_ctx, pid);

    t_paquete* paquete_buscar_ctx = crear_super_paquete(PROCESO_A_RUNNING, buffer_buscar_ctx);
    enviar_paquete(paquete_buscar_ctx, fd_kernel_memory);

    free(buffer_buscar_ctx);
    eliminar_paquete(paquete_buscar_ctx);
}


void recibir_registros(t_buffer* buffer_respuesta_ctx, int pid) {
    registro_cpu_temp->PID = pid;
    registro_cpu_temp->PC = extraer_uint32_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->AX = extraer_uint8_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->BX = extraer_uint8_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->CX = extraer_uint8_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->DX = extraer_uint8_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->EAX = extraer_uint32_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->EBX = extraer_uint32_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->ECX = extraer_uint32_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->EDX = extraer_uint32_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->SI = extraer_uint32_t_buffer(buffer_respuesta_ctx);
    registro_cpu_temp->DI = extraer_uint32_t_buffer(buffer_respuesta_ctx);

    log_info(cpu_logger, "Contexto del proceso con PID %d recibido de KERNEL MEMORY", registro_cpu_temp->PID);
    
}


void ciclo_de_instruccion() {
    flag_fin_de_ciclo = false;
    flag_syscall = false;
    log_info(cpu_logger, "INICIO DE CILCO DE INSTRUCCION CON PID: %d " , registro_cpu_temp->PID);
    while(flag_fin_de_ciclo != true && flag_syscall != true){
        
        ciclo_de_instruccion_fetch();

        ciclo_de_instruccion_decode();

        ciclo_de_instruccion_execute(instruccion_a_ejecutar);

        ciclo_de_instruccion_check_interrupt();
        

    }

    log_info(cpu_logger, "CIERRE DE CICLO DE INSTRUCCION CON PID: %d " , registro_cpu_temp->PID);
}

void ciclo_de_instruccion_fetch(){
    pedir_instruccion_a_kernel_memory(registro_cpu_temp->PID , registro_cpu_temp->PC);
    log_info(cpu_logger, "## PID: %d - FETCH - Program Counter: %d " ,registro_cpu_temp->PID , registro_cpu_temp->PC);
}

void pedir_instruccion_a_kernel_memory(int pid, int pc) {
    t_buffer* buffer_pedir_instruccion = crear_buffer();
    cargar_int_al_buffer(buffer_pedir_instruccion, pid);
    cargar_int_al_buffer(buffer_pedir_instruccion, pc);

    t_paquete* paquete_pedir_instruccion = crear_super_paquete(PEDIR_INSTRUCCION_KERNEL_MEMORY, buffer_pedir_instruccion);
    enviar_paquete(paquete_pedir_instruccion, fd_kernel_memory);

    free(buffer_pedir_instruccion);
    eliminar_paquete(paquete_pedir_instruccion);
}

void ciclo_de_instruccion_decode(){
    if(validar_instruccion_recibida(instruccion_a_ejecutar) == true){
        log_info(cpu_logger, "INSTRUCCION RECIBIDA VALIDA CON PID: %d " , registro_cpu_temp->PID);
    }else{
        log_info(cpu_logger, "INSTRUCCION RECIBIDA INVALIDA CON PID: %d " , registro_cpu_temp->PID);
        flag_fin_de_ciclo = true;
    }
    
}

bool validar_instruccion_recibida(char* instruccion_a_validar){
    int i=0;
    char* array_instrucciones[] ={"NOOP" , "SET" , "SUM" , "SUB" , "JNZ" , "COPY_MEM" , "MOV_IN " , "MOV_OUT" , "MUTEX_CREATE" , "MUTEX_LOCK" , "MUTEX_UNLOCK" , "MEM_ALLOC", "MEM_FREE" , "SLEEP" , "STDOUT" , "STDIN" , "INIT_PROC" , NULL};
    char** instruccion = string_split(instruccion_a_validar," ");
    while(array_instrucciones[i]!=NULL){
        if(strcmp(array_instrucciones[i], instruccion[0]) == 0){
            return true;
        }
        i++;
    }
    free(instruccion);
    return false;
}

void ciclo_de_instruccion_execute(char* instruccion_a_ejecutar){
    char** instruccion = string_split(instruccion_a_ejecutar," ");

    if(strcmp(instruccion[0], "NOOP") == 0){ // NOOP - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s>", registro_cpu_temp->PID, instruccion[0]);

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;

    }else if(strcmp(instruccion[0], "SET") == 0){ // SET [AX] [VALOR] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

        uint8_t* registro_referido = detectar_registro_uint8(instruccion[1]);
        *registro_referido = (uint8_t) atoi(instruccion[2]);
        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;

    }else if(strcmp(instruccion[0], "SUM") == 0){ // SUM [AX] [BX] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

        uint8_t* registro_referido_destino = detectar_registro_uint8(instruccion[1]);
		uint8_t* registro_referido_origen = detectar_registro_uint8(instruccion[2]);
        *registro_referido_destino += *registro_referido_origen;

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;

    }else if(strcmp(instruccion[0], "SUB") == 0){ // SUB [AX] [BX] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

        uint8_t* registro_referido_destino = detectar_registro_uint8(instruccion[1]);
	    uint8_t* registro_referido_origen = detectar_registro_uint8(instruccion[2]);
        *registro_referido_destino -= *registro_referido_origen;

	    registro_cpu_temp->PC = registro_cpu_temp->PC + 1;

    }else if(strcmp(instruccion[0], "JNZ") == 0){ // JNZ [AX] [VALOR] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

        uint8_t* registro_referido = detectar_registro_uint8(instruccion[1]);
		if(*registro_referido != 0) {
		    registro_cpu_temp->PC = atoi(instruccion[2]);
            }else{
                registro_cpu_temp->PC = registro_cpu_temp->PC + 1;
            }

    }else if(strcmp(instruccion[0], "COPY_MEM") == 0){ // COPY_MEM [DIRECCION] [TAMANIO]
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

    }else if(strcmp(instruccion[0], "MOV_IN") == 0){ // MOV_IN [AX] [DATOS]
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

    }else if(strcmp(instruccion[0], "MOV_OUT") == 0){ // MOV_OUT [AX] [DATOS]
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);



//  ----------------------------------------- SYSCALLS ----------------------------------------------------------------------------------

    }else if(strcmp(instruccion[0], "MUTEX_CREATE") == 0){ // MUTEX_CREATE [MUTEX_1] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;
		enviar_ctx_a_kernel_memory(registro_cpu_temp);
        devolver_control_a_kernel(registro_cpu_temp,instruccion[0],instruccion[1],NULL);

    }else if(strcmp(instruccion[0], "MUTEX_LOCK") == 0){ // MUTEX_LOCK [MUTEX_1] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;
        enviar_ctx_a_kernel_memory(registro_cpu_temp);
        devolver_control_a_kernel(registro_cpu_temp,instruccion[0],instruccion[1],NULL);
        

    }else if(strcmp(instruccion[0], "MUTEX_UNLOCK") == 0){ // MUTEX_UNLOCK [MUTEX_1] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;
        enviar_ctx_a_kernel_memory(registro_cpu_temp);
        devolver_control_a_kernel(registro_cpu_temp,instruccion[0],instruccion[1],NULL);

    }else if(strcmp(instruccion[0], "MEM_ALLOC") == 0){ // MEM_ALLOC [TAMANIO]
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

    }else if(strcmp(instruccion[0], "MEM_FREE") == 0){ // MEM_FREE [DIRECCION]
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

    }else if(strcmp(instruccion[0], "SLEEP") == 0){ // SLEEP [TIEMPO] - FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;
		enviar_ctx_a_kernel_memory(registro_cpu_temp);
        devolver_control_a_kernel(registro_cpu_temp,instruccion[0],instruccion[1],NULL);
        flag_fin_de_ciclo =true;
        flag_syscall = true;

    }else if(strcmp(instruccion[0], "STDOUT") == 0){ // STDOUT [DATOS] 
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

    }else if(strcmp(instruccion[0], "STDIN") == 0){ // STDIN [DATOS] 
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1]);

    }else if(strcmp(instruccion[0], "INIT_PROC") == 0){ // INIT_PROC [ARCHIVO DE INSTRUCCIONES] [PRIORIDAD]- FINALIZADO
        log_info(cpu_logger, "## PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", registro_cpu_temp->PID, instruccion[0], instruccion[1], instruccion[2]);

        registro_cpu_temp->PC = registro_cpu_temp->PC + 1;
        enviar_ctx_a_kernel_memory(registro_cpu_temp);
        devolver_control_a_kernel(registro_cpu_temp,instruccion[0],instruccion[1],instruccion[2]);

    }
    free(instruccion);
    
}

void devolver_control_a_kernel(t_registro_cpu* registro_cpu_temp, char* instruccion_ejecutada, char* parametro1, char* parametro2){
    op_code motivo_desalojo_int = enumeracion_syscalls(instruccion_ejecutada);
    
    t_buffer* buffer_motivo_desalojo = crear_buffer();
    cargar_int_al_buffer(buffer_motivo_desalojo, registro_cpu_temp->PID);
    cargar_string_al_buffer(buffer_motivo_desalojo, instruccion_ejecutada);
    if(parametro1 != NULL){
        cargar_string_al_buffer(buffer_motivo_desalojo, parametro1);
        log_info(cpu_logger, "arg1 %s", parametro1);
    }
    if(parametro2 != NULL){
        cargar_string_al_buffer(buffer_motivo_desalojo, parametro2);
        log_info(cpu_logger, "arg2 %s", parametro2);
    }
    //Enviar al KERNEL_SCHEDULER instruccion EXIT
    t_paquete* paquete_motivo_desalojo = crear_super_paquete(motivo_desalojo_int, buffer_motivo_desalojo);
    enviar_paquete(paquete_motivo_desalojo,fd_kernel_scheduler);

    free(buffer_motivo_desalojo);
    eliminar_paquete(paquete_motivo_desalojo);
    
}

op_code enumeracion_syscalls(char* motivo_desalojo){
    if(strcmp(motivo_desalojo, "MUTEX_CREATE") == 0){
        return MUTEX_CREATE;
    }else if(strcmp(motivo_desalojo, "MUTEX_LOCK") == 0){
        return MUTEX_LOCK;
    }else if(strcmp(motivo_desalojo, "MUTEX_UNLOCK") == 0){
        return MUTEX_UNLOCK;
    }else if(strcmp(motivo_desalojo, "SLEEP") == 0){
        return SLEEP;
    }else if(strcmp(motivo_desalojo, "INIT_PROC") == 0){
        return INIT_PROC;
    }else{
        return -1;
    }
}

void enviar_ctx_a_kernel_memory(t_registro_cpu* registro_cpu_temp){
    t_buffer* buffer_enviar_ctx = crear_buffer();
    int pid_enviar_ctx_kernel_memory = registro_cpu_temp->PID;
    cargar_int_al_buffer(buffer_enviar_ctx, pid_enviar_ctx_kernel_memory);
    int pc_enviar_ctx_kernel_memory = registro_cpu_temp->PC;
    cargar_int_al_buffer(buffer_enviar_ctx, pc_enviar_ctx_kernel_memory);
    uint8_t ax_enviar_ctx_kernel_memory = registro_cpu_temp->AX;
    cargar_uint8_t_al_buffer(buffer_enviar_ctx, ax_enviar_ctx_kernel_memory);
    uint8_t bx_enviar_ctx_kernel_memory = registro_cpu_temp->BX;
    cargar_uint8_t_al_buffer(buffer_enviar_ctx, bx_enviar_ctx_kernel_memory);
    uint8_t cx_enviar_ctx_kernel_memory = registro_cpu_temp->CX;
    cargar_uint8_t_al_buffer(buffer_enviar_ctx, cx_enviar_ctx_kernel_memory);
    uint8_t dx_enviar_ctx_kernel_memory = registro_cpu_temp->DX;
    cargar_uint8_t_al_buffer(buffer_enviar_ctx, dx_enviar_ctx_kernel_memory);
    uint32_t eax_enviar_ctx_kernel_memory = registro_cpu_temp->EAX;
    cargar_uint32_t_al_buffer(buffer_enviar_ctx, eax_enviar_ctx_kernel_memory);
    uint32_t ebx_enviar_ctx_kernel_memory = registro_cpu_temp->EBX;
    cargar_uint32_t_al_buffer(buffer_enviar_ctx, ebx_enviar_ctx_kernel_memory);
    uint32_t ecx_enviar_ctx_kernel_memory = registro_cpu_temp->ECX;
    cargar_uint32_t_al_buffer(buffer_enviar_ctx, ecx_enviar_ctx_kernel_memory);
    uint32_t edx_enviar_ctx_kernel_memory = registro_cpu_temp->EDX;
    cargar_uint32_t_al_buffer(buffer_enviar_ctx, edx_enviar_ctx_kernel_memory);
    uint32_t si_enviar_ctx_kernel_memory = registro_cpu_temp->SI;
    cargar_uint32_t_al_buffer(buffer_enviar_ctx, si_enviar_ctx_kernel_memory);
    uint32_t di_enviar_ctx_kernel_memory = registro_cpu_temp->DI;
    cargar_uint32_t_al_buffer(buffer_enviar_ctx, di_enviar_ctx_kernel_memory);

    t_paquete* paquete_enviar_ctx = crear_super_paquete(ENVIAR_CTX_MODIFICADO_KERNEL_MEMORY, buffer_enviar_ctx);
    enviar_paquete(paquete_enviar_ctx, fd_kernel_memory);

    free(buffer_enviar_ctx);
    eliminar_paquete(paquete_enviar_ctx);
}

uint8_t* detectar_registro_uint8(char* registro_a_detectar){
    if(strcmp(registro_a_detectar, "AX") == 0){
        return &(registro_cpu_temp->AX);
    }else if(strcmp(registro_a_detectar, "BX") == 0){
        return &(registro_cpu_temp->BX);
    }else if(strcmp(registro_a_detectar, "CX") == 0){
        return &(registro_cpu_temp->CX);
    }else if(strcmp(registro_a_detectar, "DX") == 0){
        return &(registro_cpu_temp->DX);
    }
    return NULL;
}

uint32_t* detectar_registro_uint32(char* registro_a_detectar){
    if(strcmp(registro_a_detectar, "EAX") == 0){
        return &(registro_cpu_temp->EAX);
    }else if(strcmp(registro_a_detectar, "EBX") == 0){
        return &(registro_cpu_temp->EBX);
    }else if(strcmp(registro_a_detectar, "ECX") == 0){
        return &(registro_cpu_temp->ECX);
    }else if(strcmp(registro_a_detectar, "EDX") == 0){
        return &(registro_cpu_temp->EDX);
    }else if(strcmp(registro_a_detectar, "SI") == 0){
        return &(registro_cpu_temp->SI);
    }else if(strcmp(registro_a_detectar, "DI") == 0){
        return &(registro_cpu_temp->DI);
    }
    return NULL;
}

void ciclo_de_instruccion_check_interrupt(){
    if (flag_syscall == false) {
        enviar_ctx_a_kernel_memory(registro_cpu_temp);
        flag_fin_de_ciclo=true;
    } else{
        log_info(cpu_logger , "No hay interrupción para el hilo PID: %d", registro_cpu_temp->PID);
    }
}
