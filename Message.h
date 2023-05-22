#ifndef MESSAGE_H
#define MESSAGE_H

// Estructuras


// Esta estructura se utilizará para almacenar los mensajes en la memoria compartida
typedef struct MSJ{
    int pid;
    char fecha[11]; // "YYYY-MM-DD" (10 caracteres + 1 para el terminador nulo)
    char hora[9]; // "HH:MM:SS" (8 caracteres + 1 para el terminador nulo)
    int linea;
    char mensaje[200];
} MSJ;

// Esta estructura se utilizará para las configuraciones de los writers y readers
typedef struct Settings{
	int sleeping;       	
	int actor;
    MSJ *shared_memory;
} Settings;

 

// ------------------------------------------------------------------------------------------Funciones

// Función para actualizar la bitácora
void update_bitacora(const char* message, char* action) {
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
        
    fprintf(file, "PID: %d, Action: %s, Time: %s, Message: %s\n", pid, action, formatted_time, message);
    
    fclose(file);
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

--------------------------------------------------------------------------------
// Llenamos la cadena de fecha con la fecha actual
void obtenerFecha(char* fecha, size_t tam) {
    time_t tiempo;
    struct tm* tiempo_local;
    tiempo = time(NULL);
    tiempo_local = localtime(&tiempo);
    strftime(fecha, tam, "%Y-%m-%d", tiempo_local);
}

// Llenamos la cadena de hora con la hora actual
void obtenerHora(char* hora, size_t tam) {
    time_t tiempo;
    struct tm* tiempo_local;
    tiempo = time(NULL);
    tiempo_local = localtime(&tiempo);    
    strftime(hora, tam, "%H:%M:%S", tiempo_local);
}


ejemplo de uso:

char fecha[11];  // Suficiente espacio para "YYYY-MM-DD" + el carácter nulo
char hora[9];    // Suficiente espacio para "HH:MM:SS" + el carácter nulo

// Obtener y mostrar la fecha
obtenerFecha(fecha, sizeof(fecha));
printf("Fecha: %s\n", fecha);

// Obtener y mostrar la hora
obtenerHora(hora, sizeof(hora));
printf("Hora: %s\n", hora);


*/

