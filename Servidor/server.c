#include "Protocol/myprotocol.h"
#include <pthread.h>

void* handleGame(void* gameIndex) {
    int index = *((int*) gameIndex);
    char charIndex = index + '0';
    free(gameIndex);

    while (1) {
        if (games[index].is_active == 1) {
            // Recibimos y enviamos mensajes
            REQUEST("GET", charIndex);
            REQUEST("SND", charIndex);
        }

        // Sleep for a short duration (adjust as needed)
        usleep(10000);
    }

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
    char logFile[256];
    sprintf(logFile, "Logs/%s", argv[2]);

    // Redirigir las salidas al archivo de log
    freopen(logFile, "w", stdout);
    freopen(logFile, "w", stderr);

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


    //Inicializar la comunicación
    startCommunication();
    
    //Inicializar instancias de juego
    initilizeInstances();

    while (1) {
        // Recibimos y enviamos mensajes
        REQUEST("GET", 'N');
        REQUEST("SND", 'N');
    }

    endCommunication();
     for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}


