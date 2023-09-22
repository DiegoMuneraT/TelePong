#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 1024
// #define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sockfd;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in serverAddr, clientAddr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    // Cualquier dirección IP
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error en el enlace");
        exit(1);
    }

    printf("Servidor escuchando en el puerto %d...\n", SERVER_PORT);

    socklen_t clientLen = sizeof(clientAddr);

    while (1) {
        // Recibir mensaje del cliente
        ssize_t recvBytes = recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, (struct sockaddr*)&clientAddr, &clientLen);
        buffer[recvBytes] = '\0';

        // Imprimir el mensaje recibido
        printf("Cliente[%s:%d]: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

        if (strcmp(buffer, "exit\n") == 0) {
            printf("Cliente[%s:%d] se ha desconectado\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
            break;
        }

        // Enviar una respuesta al cliente
        printf("Servidor: ");
        fgets(buffer, MAX_MSG_LEN, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, clientLen);
    }

    close(sockfd);
    return 0;
}