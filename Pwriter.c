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
#define action "Writer"

void *writer(void *arg){

    Settings * sett = (Settings *)arg;
    int sleeping = sett->sleeping;
    int writing = sett->actor;

    printf("Writer %d: Iniciando\n", writing);
    sleep(sleeping);
    printf("Writer %d: Terminando\n", writing);

    pthread_exit(NULL);    
	pthread_detach(pthread_self());
}

int main(int argc, char *argv[]) {    
    if (argc != 2) {
        printf("Uso: %s <num_writers> <sleeping> <writing>\n", argv[0]);
        return 1;
    }
    
    int num_writers = atoi(argv[1]);
    int sleeping = atoi(argv[2]);
    int writing = atoi(argv[3]);
    
    // Obtener el ID de la memoria compartida--------
    int shm_id = shmget(SHM_KEY, 0, 0);
    if (shm_id == -1) {
        perror("Error al obtener el ID de la memoria compartida");
        return 1;
    }    
    MSJ *shared_memory = (MSJ *)shmat(shm_id, NULL, 0);
    if (shared_memory == (MSJ *)-1) {
        perror("Error al adjuntar la memoria compartida");
        return 1;
    }

    Settings * sett = (Settings *)malloc(sizeof(Settings));
    sett->sleeping = sleeping;
    sett->actor = writing;
    sett->shared_memory = shared_memory;
    int i = 0;
    while ( i < num_writers)
    {
        pthread_t writ_thread;
        if(pthread_create(&writ_thread, NULL, &writer, (void*)sett) != 0){
            printf("\e[91;103;1m Error pthread writer\e[0m\n");
            return EXIT_FAILURE;
        }
        i++;
    }

    printf("Writers Creados.\n");
    
    while (true)
    {
        sleep(1);
    }
    

    return 0;
}