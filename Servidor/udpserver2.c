#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Estructura para guardar la información de un cliente
typedef struct {
    struct sockaddr_in addr;
    int hasSentMessage;
    int hasReceivedMessage;
} ClientInfo;

// Enviar un texto a un cliente
void sendTextToClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, const char *text) {
    sendto(sockfd, text, strlen(text), 0, (struct sockaddr *)client_addr, addr_len);
}

// Comparar dos direcciones de cliente
int compareClientAddr(const struct sockaddr_in *addr1, const struct sockaddr_in *addr2) {
    if (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port)
        return 1;
    return 0;
}

// Función principal
int main() {
    // Variables
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    char inputText[BUFFER_SIZE];
    char game[200] = "0,";

    // Crear UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr *)&client_addr, &addr_len);

        if (len > 0) {
            buffer[len] = '\0';
            printf("Received message from client at %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

            // Miramos si el servidor recibió un mensaje
            if (!client1.hasReceivedMessage) {

                printf("Marking message from client 1\n");
                client1.hasReceivedMessage = 1;
                client1.addr = client_addr;
                // strcat(game,inet_ntoa(client_addr.sin_addr));
                char port[20] = "";
                sprintf(port, "%d", ntohs(client_addr.sin_port));
                // strcat(game,":");
                strcat(game,port);
                strcat(game,",");
                sendTextToClient(sockfd, &(client1.addr), addr_len, game);
            
            // Si el cliente 1 ya envió un mensaje, miramos si el cliente 2 ya envió un mensaje
            } else if (!client2.hasReceivedMessage && !compareClientAddr(&(client1.addr), &client_addr)) {

                printf("Marking message from client 2\n");
                game[0] = '1';
                // strcat(game,inet_ntoa(client_addr.sin_addr));
                char port[20] = "";
                sprintf(port, "%d", ntohs(client_addr.sin_port));
                // strcat(game,":");
                strcat(game,port);
                strcat(game,",0,0,0,0,0,0");
                client2.hasReceivedMessage = 1;
                client2.addr = client_addr;
            }
        }

        // Miramos si ambos clientes ya enviaron un mensaje
        if (client1.hasReceivedMessage && client2.hasReceivedMessage) {

            // Mandamos la información a los clientes
            sendTextToClient(sockfd, &(client1.addr), addr_len, game);
            sendTextToClient(sockfd, &(client2.addr), addr_len, game);

            // Reset the message status
            //client1.hasReceivedMessage = 0;
            //client2.hasReceivedMessage = 0;
        }
    }

    close(sockfd);
    return 0;
}
