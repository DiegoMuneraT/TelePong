#include "Protocol/myprotocol.h"
#include <pthread.h>

// Función principal
int main() {

    //Inicializar la comunicación
    startCommunication();
    
    //Inicializar instancias de juego
    initilizeInstances();

    while (1) {

        // Recibimos el mensaje
        REQUEST("GET",'0');

        // Enviamos mensaje
        REQUEST("SND",0);

    }

    endCommunication();
    return 0;
}
