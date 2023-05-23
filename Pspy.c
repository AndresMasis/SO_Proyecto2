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
// SPY
#include "Spy.h"


#define SHM_KEY 1234
#define SEM_KEY 9999
#define action "Spy"

int main() {

    printf("\n\t\t ### Spy ###\n\n");
    printf("\n\t # Memory \n");
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

    // Leer la memoria compartida
    int i = 0;
    MSJ *tmp_shared_memory = shared_memory;	    
    int vectores = 0;
    while (tmp_shared_memory[i].is == 1)
    {
    	vectores++;	      			
        i++;
    }
    i = 0;
    while (i < vectores)
    {
        if(tmp_shared_memory[i].pid != -1){	
        	printf("Leyendo el mensaje PID: %ld Linea:%d Fecha: %s Hora: %s \n",tmp_shared_memory[i].pid,  tmp_shared_memory[i].linea, tmp_shared_memory[i].fecha, tmp_shared_memory[i].hora);		
      	}	
      	else{      	
		printf("Leyendo el mensaje PID: NULL Linea:%d Fecha: NULL Hora: NULL\n",i);	     	
      	}	
        i++;
    }    
    
    struct shmid_ds shm_info; 
 
    // Obtener información sobre la memoria compartida 
    if (shmctl(shm_id, IPC_STAT, &shm_info) == -1) { 
        perror("Error al obtener información sobre la memoria compartida"); 
        return; 
    } 
 
    // PID del proceso actual 
    pid_t current_pid = getpid(); 
 
    // Imprimir los PIDs de los procesos que tienen acceso a la memoria compartida 
    printf("\n\t #PIDs con acceso a la memoria compartida\n"); 
    for (int i = 0; i < shm_info.shm_nattch; i++) { 
        pid_t pid = shm_info.shm_cpid; 
       
        printf("\t - %d\n", pid); 
        
    }
    
    
    
    printf("\n\t # States \n");
    printData();    
    
    printf("\n\n");
    return 0;
}
