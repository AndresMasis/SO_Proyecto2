#ifndef SPY_H
#define SPY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <pthread.h>

#define MAX_PID_LENGTH 20
#define FILENAME "spy_data.txt"
#define HEAD 1111

typedef struct SpyNode {
    struct SpyNode* next;
    long pid;
    int type, action;
} SpyNode;

// Mutex para sincronizar el acceso al archivo y a la memoria compartida
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

void printData() {

    // Obtener el ID de la memoria compartida SPY--------
    int shm_id = shmget(HEAD, 0, 0);
    if (shm_id == -1) {
        perror("Error al obtener el ID de la memoria compartida SPY");
        return 1;
    }    
    SpyNode *head = (SpyNode *)shmat(shm_id, NULL, 0);
    if (head == (SpyNode *)-1) {
        perror("Error al adjuntar la memoria compartida SPY");
        return 1;
    }
    
//    SpyNode* currentNode = head[0].next;
    char* actions[] = {"Memoria", "Bloqueado", "Durmiendo","Acceso a Memoria compartida","Sin acceso a Memoria compartida"};
    char* types[] = {"Writer", "Reader", "Reader Egoista"};

    int count = 1;
    while (count < 200) {
    	
    	if(head[count].pid == 0)
    		break;
        printf("PID: %ld, Type: %s, Action: %s\n", head[count].pid, types[head[count].type], actions[head[count].action]);
        count++;
    }
    
    if(count == 1)
	printf(" Sin acciones todavia \n");
}

void writeData(long pid, int action, int type) {
    // Obtener el ID de la memoria compartida SPY--------
    int shm_id = shmget(HEAD, 0, 0);
    if (shm_id == -1) {
        perror("Error al obtener el ID de la memoria compartida SPY");
        return 1;
    }    
    SpyNode *head = (SpyNode *)shmat(shm_id, NULL, 0);
    if (head == (SpyNode *)-1) {
        perror("Error al adjuntar la memoria compartida SPY");
        return 1;
    }

    // Bloquear el mutex antes de acceder a la lista enlazada
    pthread_mutex_lock(&list_mutex);
   
    int pidExists = 0;
    int count  = 1;
    // Buscar si el PID ya existe en la lista enlazada
    while (count < 200) {
        if ((head[count].pid == pid && head[count].type == 3) )||( (head[count].pid == pid && head[count].type == 4)){
            pidExists = 2;
        }
        else if(head[count].pid == pid) {
            pidExists = 1;
            break;
        }
        else if(head[count].pid == 0) {
            break;
        }
        count++;
    }
    // Si el PID existe, sobrescribir los valores
    if (pidExists) {
        head[count].action = action;
        head[count].type = type;
    }
    else {
        head[count].pid = pid;
        head[count].action = action;
        head[count].type = type;
        head[count].next = NULL;
    }


    // Desbloquear el mutex después de terminar de acceder a la lista enlazada
    pthread_mutex_unlock(&list_mutex);
}

#endif
