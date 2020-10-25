#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>

int clientsConnected = 0;
pthread_t thread[1024];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void * ManejoCliente(void* sockClient){
	int ClientSocketInt = *((int *) sockClient);
	while(1){
		char data[250];
		int read = recv(ClientSocketInt,data,250,0);
		data[read] = '\0';
		printf("%s\n",data);
	}
}



int main(int argc, char **argv){
	int port = atoi(argv[1]);
	
	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	bind(sockfd,(const struct sockaddr *)&serverAddress,sizeof(serverAddress));
	listen(sockfd, 5);
	char message[250];
	strcpy(message, "Bienvenido al chat!");
	while(1){
		struct sockaddr_in clientAddress;
        int clientSize = sizeof(clientAddress);
        int clientSocket = accept(sockfd, (struct sockaddr *)&clientAddress, (unsigned int*) &clientSize);
		write(clientSocket,message,strlen(message));
		pthread_create(&thread[clientsConnected], NULL, ManejoCliente, (void *) &clientSocket);
		clientsConnected++;
	}
	
	
	return 0;
}