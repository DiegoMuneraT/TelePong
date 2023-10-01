#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Protocol/myprotocol.h"

// Función principal
int main() {

    // Variables de estado
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    

    Game game = {"0","0000","0000","000","000","000","000","00","00"};
    
    // Crear UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Enlazar el socket con la dirección del servidor
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Variables para guardar la información de los clientes
    ClientInfo client1 = {{0}, 0, 0};
    ClientInfo client2 = {{0}, 0, 0};

    while (1) {

        // Recibir un mensaje de un cliente
        struct sockaddr_in client_addr;

        // Recibimos el mensaje
        receiveTextFromClient(sockfd, &client_addr, addr_len, buffer, &client1, &client2, &game);

        // Miramos si ambos clientes ya enviaron un mensaje
        if (client1.hasReceivedMessage && client2.hasReceivedMessage && newMessage == 1) {
            // Mandamos la información a los clientes
            sendTextToClient(sockfd, &(client1.addr), addr_len, &game);
            sendTextToClient(sockfd, &(client2.addr), addr_len, &game);//aca
            newMessage = 0;
            printf("Sent game State: %s,%s,%s,%s,%s,%s,%s,%s,%s", game.estado, game.cliente1, game.cliente2, game.raqueta1, game.raqueta2, game.pelotaX, game.pelotaY, game.puntaje1, game.puntaje2);
        }
    }

    close(sockfd);
    return 0;
}
