#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Manejo de cadenas
#include <string.h>
// Tipos de datos
#include <ctype.h>
#include <limits.h>
// Interfaz
#include <ncurses.h>  //ADD AT THE END OF COMPILING THE FILE -lncurses
// Hilos
#include <pthread.h>
// Errores
#include <errno.h>
// Tiempo
#include <time.h>
// Memoria compartida
#include <sys/ipc.h>
#include <sys/shm.h>
// Semáforos
#include <sys/sem.h>
// Mensajes
#include "Message.h"

#define SHM_KEY 1234
#define SEM_KEY 9999
#define action "Inicializador"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main(int argc, char *argv[]) {
    int num_vectors;  // Número de vectores o líneas de la memoria compartida
    
    if (argc != 2) {
        printf("Uso: %s <num_vectors>\n", argv[0]);
        return 1;
    }    
    num_vectors = atoi(argv[1]);
    vectores = num_vectors;

    // Crear la memoria compartida
    // num vectors * sizeof(MSJ)
    int shm_id = shmget(SHM_KEY, num_vectors * sizeof(MSJ), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Error al crear la memoria compartida");
        return 1;
    }

    // Adjuntar la memoria compartida
    MSJ *shared_memory = (MSJ *)shmat(shm_id, NULL, 0);
    if (shared_memory == (MSJ *)-1) {
        perror("Error al adjuntar la memoria compartida");
        return 1;
    }
    
    
    // Desvincular la memoria compartida
    if (shmdt(shared_memory) == -1) {
        perror("Error al desvincular la memoria compartida");
        return 1;
    }
    
     // SEMAFOROS --------------------------------------------------------------------------------
     // 0: Memoria ( el primer semaforo es para la memoria compartida si se puede acceder o no)
     // 1: Lectores Egoistas

    int sem_id = semget(SEM_KEY, 2, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("Error al crear el conjunto de semáforos");
        return 1;
    }
    // Inicializar los semáforos contadores con valores específicos
    union semun arg;
    unsigned short values[3] = {1,3};  // Valores iniciales para los semáforos, este valor es la cantidad de procesos que pueden acceder a la memoria compartida al mismo tiempo 
    arg.array = values;

    if (semctl(sem_id, 0, SETALL, arg) == -1) {
        perror("Error al inicializar los semáforos contadores");
        return 1;
    }

    printf("Inicialización completa. El programa inicializador terminará.\n");
    
    return 0;
}

    /*
 // Utilizar los semáforos contadores
    struct sembuf sb;
    
    // Operación de espera (decremento) para el primer semáforo
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("Error al realizar la operación del primer semáforo");
        return 1;
    }
    
    // Operación de espera (decremento) para el segundo semáforo
    sb.sem_num = 1;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("Error al realizar la operación del segundo semáforo");
        return 1;
    }
    
    // Operación de espera (decremento) para el tercer semáforo
    sb.sem_num = 2;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("Error al realizar la operación del tercer semáforo");
        return 1;
    }
    
    // ... Hacer algo mientras los semáforos estén disponibles ...
    
    // Operación de señal (incremento) para los semáforos
    sb.sem_op = 1;
    
    // Liberar el primer semáforo
    sb.sem_num = 0;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("Error al liberar el primer semáforo");
        return 1;
    }
    
    // Liberar el segundo semáforo
    sb.sem_num = 1;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("Error al liberar el segundo semáforo");
        return 1;
    }
    
    // Liberar el tercer semáforo
    sb.sem_num = 2;
    if (semop(sem_id, &sb, 1) == -1) {
        perror("Error al liberar el tercer semáforo");
        return 1;
    }
    */
    //----------------------------------------------------- Fin SEMAFOROS


/*

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

int main() {
    int shm_id = shmget(SHM_KEY, 0, 0);
    if (shm_id == -1) {
        perror("Error al obtener el ID de la memoria compartida");
        return 1;
    }

    //change float for MSJ    
    float *shared_memory = (float *)shmat(shm_id, NULL, 0);
    if (shared_memory == (float *)-1) {
        perror("Error al adjuntar la memoria compartida");
        return 1;
    }
    
    // Acceder y utilizar la memoria compartida ver si se puede acceder con puntero y parar en null

    // Acceder y utilizar la memoria compartida
    int num_vectors = 2;  // Número de vectores/líneas
    

    for (int i = 0; i < num_vectors; i++) {
        // Acceder a los valores de los vectores/líneas
        float value = shared_memory[i];
        printf("Valor del vector/línea %d: %.2f\n", i, value);
    }
    
    // Desvincular la memoria compartida
    if (shmdt(shared_memory) == -1) {
        perror("Error al desvincular la memoria compartida");
        return 1;
    }
    
    return 0;
}

*/



/*  --------------------------------------------------------------------Como usar los semaforos

  // Realizar operaciones de espera y señal en los semáforos
 // Realizar operaciones de espera y señal en los semáforos
    struct sembuf wait_operation1 = {0, -1, 0};  // Operación de espera en el semáforo 1
    struct sembuf signal_operation1 = {0, 1, 0};  // Operación de señal en el semáforo 1
    struct sembuf wait_operation2 = {1, -1, 0};  // Operación de espera en el semáforo 2
    struct sembuf signal_operation2 = {1, 1, 0};  // Operación de señal en el semáforo 2

    printf("Proceso realizando operaciones de espera y señal en los semáforos...\n");

    // Operaciones de espera y señal en el semáforo 1
    semop(sem_id, &wait_operation1, 1);
    printf("Proceso realizó una operación de espera en el semáforo 1\n");

    semop(sem_id, &signal_operation1, 1);
    printf("Proceso realizó una operación de señal en el semáforo 1\n");

    // Operaciones de espera y señal en el semáforo 2
    semop(sem_id, &wait_operation2, 1);
    printf("Proceso realizó una operación de espera en el semáforo 2\n");

    semop(sem_id, &signal_operation2, 1);
    printf("Proceso realizó una operación de señal en el semáforo 2\n");

*/