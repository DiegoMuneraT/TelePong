#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in addr;
    int hasSentMessage;
    int hasReceivedMessage;
} ClientInfo;

void sendTextToClient(int sockfd, struct sockaddr_in *client_addr, socklen_t addr_len, const char *text) {
    sendto(sockfd, text, strlen(text), 0, (struct sockaddr *)client_addr, addr_len);
}

int compareClientAddr(const struct sockaddr_in *addr1, const struct sockaddr_in *addr2) {
    if (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port)
        return 1;
    return 0;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    char inputText[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    ClientInfo client1 = {{0}, 0, 0};
    ClientInfo client2 = {{0}, 0, 0};

    while (1) {
        // Receive messages from clients
        struct sockaddr_in client_addr;
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr *)&client_addr, &addr_len);
        if (len > 0) {
            buffer[len] = '\0';
            printf("Received message from client at %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

            // Check which client sent the message
            if (!client1.hasReceivedMessage) {
                printf("Marking message from client 1\n");
                client1.hasReceivedMessage = 1;
                client1.addr = client_addr;
                sendTextToClient(sockfd, &(client1.addr), addr_len, "Esperando otro jugador...");
            } else if (!client2.hasReceivedMessage) {
                printf("Marking message from client 2\n");
                client2.hasReceivedMessage = 1;
                client2.addr = client_addr;
                sendTextToClient(sockfd, &(client1.addr), addr_len, "Que empiece el juego!");
                sendTextToClient(sockfd, &(client2.addr), addr_len, "Que empiece el juego!");
            }
        }

        // Check if both clients have sent messages
        if (client1.hasReceivedMessage && client2.hasReceivedMessage) {
            // Prompt the server for input text
            printf("Enter text to send to clients: ");
            fgets(inputText, BUFFER_SIZE, stdin);
            strtok(inputText, "\n");  // Remove the newline character

            // Send the input text to both clients
            sendTextToClient(sockfd, &(client1.addr), addr_len, inputText);
            sendTextToClient(sockfd, &(client2.addr), addr_len, inputText);

            // Reset the message status
            //client1.hasReceivedMessage = 0;
            //client2.hasReceivedMessage = 0;
        }

        // Simulate some processing time before the next iteration
        usleep(10000);  // 10 milliseconds
    }

    close(sockfd);
    return 0;
}
