#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Conexion tomada de Geeks and Geeks

// Driver function
int main()
{
    char buffer[MAX];
	int sockfd;
	socklen_t len;
	pid_t childpid;
	struct sockaddr_in servaddr, cliaddr;
    const char *hello = "Hello from server";

    // Creating socket file descriptor
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Fillig server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) < 0) 
    {
		printf("socket bind failed...\n");
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket successfully binded..\n");

    int n;
	
	len = sizeof(cliaddr); //len is value/result

	for (;;) {
		// read the message from client and copy it in buffer
		n = recvfrom(sockfd, (char *)buffer, MAX, MSG_WAITALL, (SA*)&cliaddr, &len);
		// print buffer which contains the client contents
		buffer[n] = '\0';
    	printf("Client : %s\n", buffer);
		// copy server message in the buffer
		while ((buffer[n++] = getchar()) != '\n')
			;

		// and send that buffer to client
		sendto(sockfd, buffer, sizeof(buffer), 200, (SA*)&cliaddr, len);
		printf("message sent.\n");
		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buffer, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
		// if msg contains "move" then server make move.
	}
	
    return 0;
    
}
