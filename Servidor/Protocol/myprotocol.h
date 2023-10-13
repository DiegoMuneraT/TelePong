#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define NUM_THREADS 1

int PORT = 8080;
char *logFile = NULL;

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
    int newMessage;
    ClientInfo clients[2];
    Game game_data;
} GameInstance;

// Variables socket
int sockfd;
struct sockaddr_in server_addr;
socklen_t addr_len = sizeof(struct sockaddr_in);
char buffer[BUFFER_SIZE];

// Variables para guardar la información de los clientes
GameInstance games[NUM_THREADS];

// Inicializador instancias de juego
void initializeGameInstance(GameInstance *gameInstance) {
    ClientInfo client = {
        .addr = {0},
        .hasSentMessage = 0,
        .hasReceivedMessage = 0
    };

    Game game_data = {
        .estado = "0",
        .cliente1 = "0000",
        .cliente2 = "0000",
        .raqueta1 = "000",
        .raqueta2 = "000",
        .pelotaX = "000",
        .pelotaY = "000",
        .puntaje1 = "00",
        .puntaje2 = "00"
    };

    gameInstance->is_active = 0;
    gameInstance->newMessage = 0;
    gameInstance->clients[0] = client;
    gameInstance->clients[1] = client;
    gameInstance->game_data = game_data;
}

// Inicializa todas las instancias dependiento del número de hilos
void initializeInstances(){
    for (int i = 0; i < NUM_THREADS; i++)
    {
        initializeGameInstance(&games[i]);
    }
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


// Buscar un juego libre
int lookForFreeGame(){
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (games[i].is_active == 0){
            return i;
        }
    }
    return -1;
}


// Buscar el juego del cliente
int lookForClientsGame(struct sockaddr_in *client_addr){
     for (int i = 0; i < NUM_THREADS; i++)
    {
        //printf("%s,%s,%s\n", games[i].game_data.estado, games[i].game_data.cliente1, games[i].game_data.cliente2);
        if (compareClientAddr(&(games[i].clients[0].addr), client_addr)){
            return i;
        } else if (compareClientAddr(&(games[i].clients[1].addr), client_addr)){
            return i;
        }
    }
    return -1;
}


// Enviar un mensaje a un cliente
void sendTextToClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, Game *game)
{
    char message[46];
    sprintf(message, "%s,%s,%s,%s,%s,%s,%s,%s,%s", game->estado, game->cliente1, game->cliente2, game->raqueta1, game->raqueta2, game->pelotaX, game->pelotaY, game->puntaje1, game->puntaje2);
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)client_addr, addr_len);
}


// Marcar un mensaje recibido por un cliente
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


// Recibir un mensaje de un cliente
void receiveTextFromClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, char *text, int clientsGame)
{
    int len = recvfrom(sockfd, text, BUFFER_SIZE, 0, (struct sockaddr *)client_addr, &addr_len);

    if (len > 0)
    {
        printf("Received message from client at %s:%d: %s\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), text);


        int gameIndex;

        text[len] = '\0';


        if (text[0] == 'P' && text[1] == 'L' && text[2] == 'J'){
            gameIndex = lookForFreeGame();

            if (gameIndex != -1){

                if (!games[gameIndex].clients[0].hasReceivedMessage)
                {
                    markReceivedMessage(&games[gameIndex].clients[0], client_addr, sockfd, addr_len, &games[gameIndex].game_data);
                }
                else if (!games[gameIndex].clients[1].hasReceivedMessage && !compareClientAddr(&(games[gameIndex].clients[0].addr), client_addr))
                {
                    strcpy(games[gameIndex].game_data.estado, "1");
                    markReceivedMessage(&games[gameIndex].clients[1], client_addr, sockfd, addr_len, &games[gameIndex].game_data);
                }
                games[gameIndex].newMessage = 1;
            } else {
                printf("No hay juegos disponibles");
            }
        }else {
            
            gameIndex = clientsGame;

            if (text[0] == 'P' && text[1] == 'M')
            {
                if (client_addr->sin_port == htons(atoi(games[gameIndex].game_data.cliente1)))
                {
                    memcpy(games[gameIndex].game_data.raqueta1, &text[2], strlen(text) - 1);
                }
                else
                {
                    memcpy(games[gameIndex].game_data.raqueta2, &text[2], strlen(text) - 1);
                }
            }
            else if (text[0] == 'S' && text[1] == 'C')
            {
                if (client_addr->sin_port == htons(atoi(games[gameIndex].game_data.cliente1)))
                {
                    memcpy(games[gameIndex].game_data.puntaje1, &text[2], strlen(text) - 1);
                }
                else
                {
                    memcpy(games[gameIndex].game_data.puntaje2, &text[2], strlen(text) - 1);
                }
            }
            games[gameIndex].newMessage = 1;
        }
    }
}

// Iniciar socket
void startCommunication(){
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
}

// Terminar socket
void endCommunication(){
    printf("Closing communication on port %d\n", PORT);
    close(sockfd);
}

// Protocolo
void REQUEST(char *header, char value)
{

    if(header[0] == 'G' && header[1] == 'E' && header[2] == 'T' && value == 'N')
    {// Obtener mensaje de cliente nuevo y asignarle un juego
        printf("Entro 1\n");
        struct sockaddr_in client_addr;
        receiveTextFromClient(sockfd, &client_addr, addr_len, buffer, 0);
    } else if (header[0] == 'S' && header[1] == 'N' && header[2] == 'D' && value == 'N')
    {
        // Enviar primer estado a los nuevos clientes y empezar juego
        printf("Entro 2\n");
        struct sockaddr_in client_addr;
        int clientsGame = lookForClientsGame(&client_addr);
        if (clientsGame != -1 && games[clientsGame].is_active == 0 && games[clientsGame].clients[0].hasReceivedMessage && games[clientsGame].clients[1].hasReceivedMessage) {
            // Mandamos la información a los clientes
            games[clientsGame].is_active = 1;
            sendTextToClient(sockfd, &(games[clientsGame].clients[0].addr), addr_len, &games[clientsGame].game_data);
            sendTextToClient(sockfd, &(games[clientsGame].clients[1].addr), addr_len, &games[clientsGame].game_data);
            games[clientsGame].newMessage = 0;
            printf("Sent first game State: %s,%s,%s,%s,%s,%s,%s,%s,%s\n", games[clientsGame].game_data.estado, games[clientsGame].game_data.cliente1, games[clientsGame].game_data.cliente2, games[clientsGame].game_data.raqueta1, games[clientsGame].game_data.raqueta2, games[clientsGame].game_data.pelotaX, games[clientsGame].game_data.pelotaY, games[clientsGame].game_data.puntaje1, games[clientsGame].game_data.puntaje2);
        }
    } else if (header[0] == 'M' && header[1] == 'S' && header[2] == 'G')
    {
        // Recibir mensaje de ambos clientes con juego ya asignado
        printf("Entro 3\n");
        int clientsGame = value - '0';
        // AQUÍ ESTA EL ERROR
        receiveTextFromClient(sockfd,&(games[clientsGame].clients[0].addr),addr_len,buffer, clientsGame);
        // receiveTextFromClient(sockfd,&(games[clientsGame].clients[1].addr),addr_len,buffer, clientsGame);
    } else if (header[0] == 'U' && header[1] == 'P' && header[2] == 'D')
    {
        // Enviar estado a clientes con juego ya asignado
        printf("Entro 4\n");
        int clientsGame = value - '0';
        if (games[clientsGame].newMessage == 1){
            sendTextToClient(sockfd, &(games[clientsGame].clients[0].addr), addr_len, &games[clientsGame].game_data);
            sendTextToClient(sockfd, &(games[clientsGame].clients[1].addr), addr_len, &games[clientsGame].game_data);
            games[clientsGame].newMessage = 0;
            printf("Sent game state: %s,%s,%s,%s,%s,%s,%s,%s,%s\n", games[clientsGame].game_data.estado, games[clientsGame].game_data.cliente1, games[clientsGame].game_data.cliente2, games[clientsGame].game_data.raqueta1, games[clientsGame].game_data.raqueta2, games[clientsGame].game_data.pelotaX, games[clientsGame].game_data.pelotaY, games[clientsGame].game_data.puntaje1, games[clientsGame].game_data.puntaje2);
        }
    }
}