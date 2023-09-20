#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 1024
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
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    socklen_t clientLen = sizeof(clientAddr);

    while (1) {

        // Receive message from a client
        ssize_t recvBytes = recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, (struct sockaddr*)&clientAddr, &clientLen);
        buffer[recvBytes] = '\0';

        // Print the received message
        printf("Client[%s:%d]: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

        // Send a response to the client
        printf("Server: ");
        fgets(buffer, MAX_MSG_LEN, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, clientLen);
    }

    close(sockfd);
    return 0;
}