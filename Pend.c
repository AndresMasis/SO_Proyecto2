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
#define action "Finalizador"

int main() {
    // Obtener el ID de la memoria compartida---------------------------------------
    int shm_id = shmget(SHM_KEY, 0, 0);
    if (shm_id == -1) {
        perror("Error al obtener el ID de la memoria compartida");
        return 1;
    }    
    // Desvincular y eliminar la memoria compartida
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("Error al eliminar la memoria compartida");
        return 1;
    }
    
    // Obtener el ID del conjunto de semáforos---------------------------------------
    int sem_id = semget(SEM_KEY, 0, 0);
    if (sem_id == -1) {
        perror("Error al obtener el ID del conjunto de semáforos");
        return 1;
    }    
    // Eliminar el conjunto de semáforos
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Error al eliminar el conjunto de semáforos");
        return 1;
    }
    
    /*

    // Eliminar  procesos  ------------------------------------------------------------------------------
    system("killall init");

    */
    // Cerrar el archivo de bitácora------------------------------------------------------------------------------
    FILE *bitacora = fopen("bitacora.txt", "a");
    if (bitacora == NULL) {
        perror("Error al abrir el archivo de bitácora");
        return 1;
    }
    fprintf(bitacora, " \n \t### --- EXECUTION ENDED --- ###  \n \n");    
    fclose(bitacora);

   
    printf("Finalización completa. Todos los recursos han sido liberados.\n");
    
    return 0;
}
