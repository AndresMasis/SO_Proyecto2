#ifndef SPY_H
#define SPY_H

#define MAX_PID_LENGTH 20
#define FILENAME "spy_data.txt"
#define HEAD 1111

typedef struct SpyNode{
    struct SpyNode* next;
    long pid;
    int type, action;
} SpyNode;



// Mutex para sincronizar el acceso al archivo
extern pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

void printData() {
    // Bloquear el mutex antes de acceder a la lista enlazada
    pthread_mutex_lock(&list_mutex);

    SpyNode* currentNode = head;
    char* actions[] = {"Memoria", "Bloqueado", "Durmiendo"};
    char* types[] = {"Writer", "Reader", "Reader Egoista"};

    while (currentNode != NULL) {
        printf("PID: %ld, Action: %s, Type: %s\n", currentNode->pid, actions[currentNode->action], types[currentNode->type]);
        currentNode = currentNode->next;
    }

    // Desbloquear el mutex después de terminar de acceder a la lista enlazada
    pthread_mutex_unlock(&list_mutex);
}

void writeData(long pid, int action, int type) {
    // Bloquear el mutex antes de acceder a la lista enlazada
    pthread_mutex_lock(&list_mutex);

    printf("Write Data:");

    SpyNode* currentNode = head;
    SpyNode* prevNode = NULL;
    int pidExists = 0;

    // Buscar si el PID ya existe en la lista enlazada
    while (currentNode != NULL) {
        if (currentNode->pid == pid) {
            pidExists = 1;
            break;
        }
        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    printf("PID: %ld", pid);

    // Si el PID existe, sobrescribir los valores
    if (pidExists) {
        currentNode->action = action;
        currentNode->type = type;
    } else {
        // Si el PID no existe, crear un nuevo nodo
        SpyNode* newNode = (SpyNode*)malloc(sizeof(SpyNode));
        newNode->pid = pid;
        newNode->action = action;
        newNode->type = type;
        newNode->next = NULL;

        // Insertar el nuevo nodo al final de la lista enlazada
        if (prevNode != NULL) {
            prevNode->next = newNode;
        } else {
            head = newNode;
        }
    }

    // Desbloquear el mutex después de terminar de acceder a la lista enlazada
    pthread_mutex_unlock(&list_mutex);
}

#endif
