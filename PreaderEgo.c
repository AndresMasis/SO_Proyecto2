#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Memoria compartida
#include <sys/ipc.h>
#include <sys/shm.h>
// Semáforos
#include <sys/sem.h>
// Mensajes
#include "Message.h"

#define SHM_KEY 1234
#define SEM_KEY 9999
#define action "Reader-Egoista"

int main(int argc, char *argv[]) {    
    if (argc != 2) {
        printf("Uso: %s <num_readers_ego> <sleeping> <reading>\n", argv[0]);
        return 1;
    }
    
    int num_readers_ego = atoi(argv[1]);
    int sleeping = atoi(argv[2]);
    int reading = atoi(argv[3]);
    
    /*
    No permitir que más de 3 readers egoístas tengan acceso a la memoria
    compartida de manera consecutiva. Si se presenta esta situación la memoria compartida
    debe ser entregado a algún otro proceso que esté compitiendo por él. SI no hay nadie más
    podrán seguir leyendo los egoístas hasta que alguien solicite el recurso

    */
    
    printf("Inicialización completa. El programa inicializador terminará.\n");
    
    return 0;
}