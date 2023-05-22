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
    MSJ *shared_memory = sett->shared_memory;
    int sem_id = sett->sem_id;

    struct sembuf wait_operation1 = {0, -1, 0};  // Operación de espera 
    struct sembuf signal_operation1 = {0, 1, 0};  // Operación de señal 

    MSJ *msj = (MSJ *)malloc(sizeof(MSJ));
    msj->pid = getpid();

    char *fecha;  // Suficiente espacio para "YYYY-MM-DD" + el carácter nulo
    char *hora;    // Suficiente espacio para "HH:MM:SS" + el carácter nulo

    while (true)
    {
        // Bloqueo el acceso a la memoria compartida
        semop(sem_id, &wait_operation1, 1);
        // Escribo en la memoria compartida

            // Obtener y mostrar la fecha
            fecha = obtenerFecha();
            // Obtener y mostrar la hora
            hora = obtenerHora();   

            // Ver que linea de la memoria compartida esta disponible y escribir
            int i = 0;

	    MSJ *tmp_shared_memory = shared_memory;
	    
     	    int vectores = 0;
            while (tmp_shared_memory[i].is == 1)
            {
            	vectores++;	      			
                i++;
            }
               printf("vectORES %d \n",vectores);
            i = 0;
            while (i < vectores)
            {
                if(tmp_shared_memory[i].pid == -1){
	      		break;		
	      	}	      			
                i++;
            }    
 
            if (i == vectores)
            {
                printf("\e[92;1m: El writer %d no encuentra espacio] \n", msj->pid);
            }
            else
            {
	        msj->linea = i;
                strcpy(msj->fecha, fecha);
                strcpy(msj->hora, hora);
                msj->is = 1;
                tmp_shared_memory[i] = *msj;
                printf("\e[92;1m Escribiendo en la linea %d fecha %s hora %s ]\n", msj->linea, msj->fecha, msj->hora);
            }

        // Tiempo que tarda en escribir
        sleep(writing);
        // Libero la memoria compartida
        semop(sem_id, &signal_operation1, 1);
        // Duerme el writer
        sleep(sleeping);
    }

    pthread_detach(pthread_self());
}


int main(int argc, char *argv[]) {    
    if (argc != 4) {
        printf("Uso: %s <num_writers> <sleeping> <writing>\n", argv[0]);
        return 1;
    }
    
    printf("Writer looking for space \n");

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
       
     // Obtener el ID del conjunto de semáforos-----
    int sem_id = semget(SEM_KEY, 0, 0);
    if (sem_id == -1) {
        perror("Error al obtener el ID del conjunto de semáforos");
        return 1;
    }

    Settings * sett = (Settings *)malloc(sizeof(Settings));
    sett->sleeping = sleeping;
    sett->actor = writing;
    sett->shared_memory = shared_memory;
    sett->sem_id = sem_id;
    int i = 0;
     
     //printf("vect %d \n",shared_memory[0].pid);
     
    while ( i < num_writers)
    {
        pthread_t writ_thread;
        if(pthread_create(&writ_thread, NULL, &writer, (void*)sett) != 0){
            printf("\e[91;103;1m Error pthread writer\e[0m\n");
            return EXIT_FAILURE;
        }
        i++;
    }
    
    while (true)
    {
        sleep(1);
    }
    

    return 0;
}
