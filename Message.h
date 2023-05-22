#ifndef MESSAGE_H
#define MESSAGE_H

int vectores;

// Estructuras

// Esta estructura se utilizará para almacenar los mensajes en la memoria compartida
typedef struct MSJ{
    int pid;
    char fecha[11]; // "YYYY-MM-DD" (10 caracteres + 1 para el terminador nulo)
    char hora[9]; // "HH:MM:SS" (8 caracteres + 1 para el terminador nulo)
    int linea;
    int is;
} MSJ;

// Esta estructura se utilizará para las configuraciones de los writers y readers
typedef struct Settings{
	int sleeping;       	
	int actor;
    	int sem_id;
    	MSJ *shared_memory;
} Settings;

 

// ------------------------------------------------------------------------------------------Funciones

// Función para actualizar la bitácora
void update_bitacora(MSJ* mess, char* action) {
    char* message;
    int pid = getpid();
    FILE* file = fopen("bitacora.txt", "a");
    if (file == NULL) {
        perror("Error al abrir el archivo de bitácora");
        return;
    }
    
    time_t current_time;
    struct tm* local_time;
    char formatted_time[20];    
    current_time = time(NULL);
    local_time = localtime(&current_time);    
    strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S", local_time);
        
    fprintf(file, "PID: %d, Action: %s, Time: %s, \n \t Message:[PID: %d FECHA: %s HORA: %s LINEA: %d] \n", pid, action, formatted_time, mess->pid, mess->fecha, mess->hora, mess->linea);
    
    fclose(file);
}

// Llenamos la cadena de fecha con la fecha actual 
char* obtenerFecha() { 
    char* fecha = (char*)malloc(11 * sizeof(char)); 
    time_t tiempo; 
    struct tm* tiempo_local; 
    tiempo = time(NULL); 
    tiempo_local = localtime(&tiempo); 
    strftime(fecha, 11, "%Y-%m-%d", tiempo_local); 
    return fecha; 
} 
 
// Llenamos la cadena de hora con la hora actual 
char* obtenerHora() { 
    char* hora = (char*)malloc(9 * sizeof(char)); 
    time_t tiempo; 
    struct tm* tiempo_local; 
    tiempo = time(NULL); 
    tiempo_local = localtime(&tiempo); 
    strftime(hora, 9, "%H:%M:%S", tiempo_local); 
    return hora; 
}

#endif

/*
#include <time.h>

MSJ mensaje;
time_t tiempo;
struct tm *tiempo_local;
// Obtener la hora actual
tiempo = time(NULL);
tiempo_local = localtime(&tiempo);

// Llenar la estructura Mensaje con la hora y fecha actual
strftime(mensaje.fecha, sizeof(mensaje.fecha), "%Y-%m-%d", tiempo_local);
strftime(mensaje.hora, sizeof(mensaje.hora), "%H:%M:%S", tiempo_local);

// Imprimir los valores obtenidos
printf("Fecha: %s\n", mensaje.fecha);
printf("Hora: %s\n", mensaje.hora);

*/

