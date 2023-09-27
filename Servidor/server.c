#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Estructura para guardar la información de un cliente
typedef struct
{
    struct sockaddr_in addr;
    int hasSentMessage;
    int hasReceivedMessage;
} ClientInfo;

// Estructura para guardar la información del juego
typedef struct {
    char estado[2];
    char cliente1[6];
    char cliente2[6];
    char raqueta1[5];
    char raqueta2[5];
    char pelotaX[4];
    char pelotaY[4];
    char puntaje1[3];
    char puntaje2[3];
} Game;

// Funcion para enviar un mensaje a un cliente
void sendTextToClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, Game *game)
{
    //printf("Estado: %s, %s, %s", game->estado, game->cliente1, game->cliente2);
    char message[46];
    sprintf(message, "%s,%s,%s,%s,%s,%s,%s,%s,%s", game->estado, game->cliente1, game->cliente2, game->raqueta1, game->raqueta2, game->pelotaX, game->pelotaY, game->puntaje1, game->puntaje2);
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)client_addr, addr_len);
}

// Función para marcar un mensaje recibido por un cliente
void markReceivedMessage(ClientInfo *client, struct sockaddr_in *client_addr, int sockfd, socklen_t addr_len, Game *game) 
{
    printf("Marking message from client %s:%d\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    client->hasReceivedMessage = 1;
    client->addr = *client_addr;
    char port[6] = "";
    sprintf(port, "%d", ntohs(client_addr->sin_port));

    // Guardamos el puerto del cliente
    if (strcmp(game->cliente1, "0000") == 0)
        strcpy(game->cliente1, port);
    else
        strcpy(game->cliente2, port);
    
    sendTextToClient(sockfd, client_addr, addr_len, game);
}

// Comparar dos direcciones de cliente
int compareClientAddr(const struct sockaddr_in *addr1, const struct sockaddr_in *addr2) {
    if (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port)
        return 1;
    return 0;
}

// Funcion para recibir un mensaje de un cliente
void receiveTextFromClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, char *text, ClientInfo *client1, ClientInfo *client2, Game *game)
{
    int len = recvfrom(sockfd, text, BUFFER_SIZE, 0, (struct sockaddr *)client_addr, &addr_len);

    if (len > 0) {
        text[len] = '\0';

        printf("Received message from client at %s:%d: %s\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), text);

        // Miramos si el servidor recibió un mensaje
        if (!client1->hasReceivedMessage) {
            markReceivedMessage(client1, client_addr, sockfd, addr_len, game);
        } else if (!client2->hasReceivedMessage && !compareClientAddr(&(client1->addr), client_addr)) {
            strcpy(game->estado, "1");
            markReceivedMessage(client2, client_addr, sockfd, addr_len, game);
        }

        if (text[0] == 'P' && text[1] == 'M')
        {
            if(client_addr->sin_port == htons(atoi(game->cliente1)))
            {
                memcpy(game->raqueta1, &text[2], strlen(text)-1);
            }
            else
            {
                memcpy(game->raqueta2, &text[2], strlen(text)-1);
            }
        }
    }

}

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
        if (client1.hasReceivedMessage && client2.hasReceivedMessage) {
            // Mandamos la información a los clientes
            sendTextToClient(sockfd, &(client1.addr), addr_len, &game);
            sendTextToClient(sockfd, &(client2.addr), addr_len, &game);
        }

        
    }

    close(sockfd);
    return 0;
}
