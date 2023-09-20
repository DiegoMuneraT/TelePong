#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sockfd;
    char buffer[MAX_MSG_LEN];
    struct sockaddr_in serverAddr;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        // Send a message to the server
        printf("Client: ");
        fgets(buffer, MAX_MSG_LEN, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        // Receive the response from the server
        ssize_t recvBytes = recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, NULL, NULL);
        buffer[recvBytes] = '\0';

        // Print the server's response
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}