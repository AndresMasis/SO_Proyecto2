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
#define action "ReaderEgo"
int reads = 0;

 // Mutex para sincronizar el acceso a la  y a la memoria compartida
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
.
void *reader(void *arg){
    Settings * sett = (Settings *)arg;
    int sleeping = sett->sleeping;
    int reading = sett->actor;
    MSJ *shared_memory = sett->shared_memory;
    int sem_id = sett->sem_id;

    struct sembuf wait_operation1 = {0, -1, 0};  // Operación de espera 
    struct sembuf signal_operation1 = {0, 1, 0};  // Operación de señal 

    MSJ *msj = (MSJ *)malloc(sizeof(MSJ));
    msj->pid = (long)pthread_self();
    long pid2 = (long)pthread_self();

    char *fecha;  // Suficiente espacio para "YYYY-MM-DD" + el carácter nulo
    char *hora;    // Suficiente espacio para "HH:MM:SS" + el carácter nulo
    
    int linea = 0;
    
    while (true)
    {        
    writeData(pid2, 3, 2);
        	
	writeData(pid2, 1, 2);
        // Bloqueo el acceso a la memoria compartida
        semop(sem_id, &wait_operation1, 1);       

            // Leo en la memoria compartida
 	        writeData(pid2, 0, 2);
            // Ver que linea de la memoria compartida tiene un mensaje
            int i = 0;
	        MSJ *tmp_shared_memory = shared_memory;	    
     	    int vectores = 0;
            while (tmp_shared_memory[i].is == 1)
            {
            	vectores++;	      			
                i++;
            }
            i = linea;
            while (i < vectores)
            {
                if(tmp_shared_memory[i].pid != -1){
	      		break;		
	      	}	      			
                i++;
            }    
 
            if (i == vectores)
            {
                printf("\e[92;1m: El reader-ego %ld no encuentra nada que leer \n", msj->pid);
            }
            else
            {
                // Obtener y mostrar la fecha
                fecha = obtenerFecha();
                // Obtener y mostrar la hora
                hora = obtenerHora();   
                linea = i;
                
                msj->pid = tmp_shared_memory[i].pid;
	            msj->linea = tmp_shared_memory[i].linea;
                strcpy(msj->fecha, tmp_shared_memory[i].fecha);
                strcpy(msj->hora, tmp_shared_memory[i].hora);

                update_bitacora( msj, action);
                printf("\e[92;1m Robando la linea %d fecha %s hora %s \n", msj->linea, msj->fecha, msj->hora);

                tmp_shared_memory[i].pid = -1;
                memcpy(tmp_shared_memory[i].fecha,"YYYY-MM-DD",10);
                memcpy(tmp_shared_memory[i].hora,"YY-MM-DD",8);
                tmp_shared_memory[i].linea = i;
                tmp_shared_memory[i].is = 1;
                
                // Tiempo que tarda en leer
                sleep(reading);
                linea++;
                if (linea == vectores)
			        linea = 0;
            }
          
        // Libero la memoria compartida
        semop(sem_id, &signal_operation1, 1);   
        writeData(pid2, 2, 2);
        // Duerme el lector
        sleep(sleeping);
    }

    pthread_detach(pthread_self());
}


int main(int argc, char *argv[]) {    
    if (argc != 4) {
        printf("Uso: %s <num_readers> <sleeping> <reading>\n", argv[0]);
        return 1;
    }
    
    printf("Reader looking for a line \n");

    int num_readers = atoi(argv[1]);
    int sleeping = atoi(argv[2]);
    int reading = atoi(argv[3]);
    
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
    sett->actor = reading;
    sett->shared_memory = shared_memory;
    sett->sem_id = sem_id;
    int i = 0;
     
     //printf("vect %d \n",shared_memory[0].pid);
     
    while ( i < num_readers)
    {
        pthread_t reader_thread;
        if(pthread_create(&reader_thread, NULL, &reader, (void*)sett) != 0){
            printf("\e[91;103;1m Error pthread reader\e[0m\n");
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
