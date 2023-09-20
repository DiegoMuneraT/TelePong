#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Conexion tomada de Geeks and Geeks




int main()
{
	int sockfd;
    char buffer[MAX];
    const char *hello = "Hello from client";
	struct sockaddr_in servaddr;


    // creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created..\n");

	
	memset(&servaddr, 0, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

    int n;
    socklen_t len;

    for (;;){
        bzero(buffer, sizeof(buffer));
		printf("Enter the string : ");
		n = 0;
		while ((buffer[n++] = getchar()) != '\n')
			;
        sendto(sockfd, buffer, sizeof(buffer), 200, (SA*)&servaddr, sizeof(servaddr));
		bzero(buffer, sizeof(buffer));
		read(sockfd, buffer, sizeof(buffer));
        n = recvfrom(sockfd, (char *)buffer, MAX, MSG_WAITALL, (SA*)&servaddr, &len);
		printf("From Server : %s", buffer);
        printf("Message sent.\n");
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
        if ((strncmp(buffer, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
    }

    close(sockfd);
    return 0;
}
