#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int newMessage = 0;

typedef struct{
    struct sockaddr_in addr;
    int hasSentMessage;
    int hasReceivedMessage;
} ClientInfo;

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

typedef struct{
    int is_active;
    ClientInfo clients[2];
    Game game_data;
} GameInstance;


// Funcion para enviar un mensaje a un cliente
void sendTextToClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, Game *game)
{
    char message[46];
    sprintf(message, "%s,%s,%s,%s,%s,%s,%s,%s,%s", game->estado, game->cliente1, game->cliente2, game->raqueta1, game->raqueta2, game->pelotaX, game->pelotaY, game->puntaje1, game->puntaje2);
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)client_addr, addr_len);
}

// Comparar dos direcciones de cliente 
int compareClientAddr(const struct sockaddr_in *addr1, const struct sockaddr_in *addr2)
{
    if(addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port)
    {
        return 1;
    }
    return 0;
}

// Funcion para marcar un mensaje recibido por un cliente
void markReceivedMessage(ClientInfo *client, struct sockaddr_in *client_addr, int sockfd, socklen_t addr_len, Game *game)
{
    printf("Marking message from client at %s:%d as received\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    client->hasReceivedMessage = 1;
    client->addr = *client_addr;
    char port[6];
    sprintf(port, "%d", ntohs(client_addr->sin_port));

    if (strcmp(game->cliente1, "0000") == 0)
    {
        strcpy(game->cliente1, port);
    }
    else
    {
        strcpy(game->cliente2, port);
    }
    sendTextToClient(sockfd, client_addr, addr_len, game);

}


// Funcion para recibir un mensaje de un cliente
void receiveTextFromClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, char *text, ClientInfo *client1, ClientInfo *client2, Game *game)
{
    int len = recvfrom(sockfd, text, BUFFER_SIZE, 0, (struct sockaddr *)client_addr, &addr_len);

    if (len > 0)
    {

        text[len] = '\0';

        printf("Received message from client at %s:%d: %s\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), text);

        if (!client1->hasReceivedMessage)
        {
            markReceivedMessage(client1, client_addr, sockfd, addr_len, game);
        }
        else if (!client2->hasReceivedMessage && !compareClientAddr(&(client1->addr), client_addr))
        {
            strcpy(game->estado, "1");
            markReceivedMessage(client2, client_addr, sockfd, addr_len, game);
        }

        if (text[0] == 'P' && text[1] == 'M')
        {
            if (client_addr->sin_port == htons(atoi(game->cliente1)))
            {
                memcpy(game->raqueta1, &text[2], strlen(text) - 1);
            }
            else
            {
                memcpy(game->raqueta2, &text[2], strlen(text) - 1);
            }
        }
        else if (text[0] == 'S' && text[1] == 'C')
        {
            if (client_addr->sin_port == htons(atoi(game->cliente1)))
            {
                memcpy(game->puntaje1, &text[2], strlen(text) - 1);
            }
            else
            {
                memcpy(game->puntaje2, &text[2], strlen(text) - 1);
            }
        }
        newMessage = 1;
    }
}

void REQUEST(char *header, char value)
{
    if(header[0] == 'G' && header[1] == 'E' && header[2] == 'T')
    {

    }
}