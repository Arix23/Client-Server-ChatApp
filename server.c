#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>

int clientsConnected = 0;





int main(int argc, char **argv){
	int port = atoi(argv[1]);
	
	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	bind(sockfd,(const struct sockaddr *)&serverAddress,sizeof(serverAddress));
	listen(sockfd, 5);
	while(1){
		char message[250];
		strcpy(message, "Bienvenido al chat!");
		struct sockaddr_in clientAddress;
        int clientSize = sizeof(clientAddress);
        int clientSocket = accept(sockfd, (struct sockaddr *)&clientAddress, (unsigned int*) &clientSize);
		write(clientSocket,message,strlen(message));
        char c;
        while(read(clientSocket,&c,1)){
        printf("%c", c);
    	}
	}
	
	return 0;
}