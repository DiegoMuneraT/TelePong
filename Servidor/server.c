#include "Protocol/myprotocol.h"
#include <pthread.h>

// Ejecutar juego en los threads
void* handleGame(void* gameIndex) {
    int index = *((int*) gameIndex);
    char charIndex = index + '0';
    free(gameIndex);
    int port = PORT+index+1;

    // Inicializar la comunicación del thread
    startCommunication(port, &sockfd[index+1], &server_addr[index+1]);

    while (1) {
        if (games[index].is_active == 1) {
            // Recibimos y enviamos mensajes
            REQUEST("MSG", charIndex);
            REQUEST("UPD", charIndex);
        }

        // Sleep por algun periodo de tiempo
        // usleep(10000);
    }

    endCommunication(sockfd[index+1]);

    return NULL;
}

// Función principal
int main(int argc, char *argv[]) {

    if(argc != 3){
        fprintf(stderr, "Uso: %s <PORT> <LogFile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Guardar el puerto y el archivo de log
    PORT = atoi(argv[1]);
    // char logFile[256];
    // sprintf(logFile, "Logs/%s", argv[2]);

    // // Redirigir las salidas al archivo de log
    // freopen(logFile, "w", stdout);
    // freopen(logFile, "w", stderr);

    // Inicializar la comunicación
    startCommunication(PORT, &sockfd[0], &server_addr[0]);
    
    // Inicializar instancias de juego
    initializeInstances();

    // Crear threads para manejar las instancias de juego
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        int *threadIndex = malloc(sizeof(int));
        if (threadIndex == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        *threadIndex = i;
        if (pthread_create(&threads[i], NULL, handleGame, (void*)threadIndex) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }


    while (1) {
        // Recibimos y enviamos mensajes
        REQUEST("GET", 'N');
        REQUEST("SND", 'N');
        // usleep(10000);
    }

    // Terminamos socket
    endCommunication(sockfd[0]);

    // Terminamos los threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}


