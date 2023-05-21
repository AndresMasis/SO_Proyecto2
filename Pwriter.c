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
#define action "Writer"

int main(int argc, char *argv[]) {    
    if (argc != 2) {
        printf("Uso: %s <num_writers> <sleeping> <writing>\n", argv[0]);
        return 1;
    }
    
    int num_writers = atoi(argv[1]);
    int sleeping = atoi(argv[2]);
    int writing = atoi(argv[3]);
    
    
    printf("Inicialización completa. El programa inicializador terminará.\n");
    
    return 0;
}