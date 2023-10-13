#include "Protocol/myprotocol.h"
#include <pthread.h>

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
