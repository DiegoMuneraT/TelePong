#include "Protocol/myprotocol.h"
#include <pthread.h>

void* handleGame(void* gameIndex) {
    int index = *((int*) gameIndex);
    char charIndex = index + '0';
    while (1) {
        // Process game instance with index
        // ...
        if (games[index].is_active == 1){
            printf("obteniendo recursos");
            // Recibimos el mensaje
            REQUEST("GET",charIndex);
            
            // Enviamos mensaje
            REQUEST("SND",charIndex);
        }

        // Sleep for a short duration (adjust as needed)
        // usleep(10000);
    }
    return NULL;
}


// Función principal
int main() {
    //Inicializar la comunicación
    startCommunication();
    
    //Inicializar instancias de juego
    initilizeInstances();
    
    // Crear threads para manejar las instancias de juego
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
       pthread_create(&threads[i], NULL, handleGame, (void*)&i);
    }

    while (1) {

        // Recibimos el mensaje
        REQUEST("GET",'N');

        // Enviamos mensaje
        REQUEST("SND",'N');

    }

    endCommunication();
     for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
