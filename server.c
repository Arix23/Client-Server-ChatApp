/*
Ari Valenzuela - A01635584
Nataly Hernández - A01631314
Diego Vazquez - A00226803
*/
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <signal.h> 

int clientsConnected = 0;
pthread_t thread[1024];
int clientesSocketID[1024];


void signalHandler(int signum){
	//terminar server
	char byeServer[250];
	strcpy(byeServer, "Bye desde el server\n");
    for(int i = 0;i<clientsConnected;i++){
		send(clientesSocketID[i],byeServer,250,0);
		close(clientesSocketID[i]);
		pthread_cancel(thread[i]);
		
	}
	exit(0);
}

void * ManejoCliente(void* numClient){
	int numCliente = *((int *) numClient);
	numCliente = numCliente-1;
	while(1){
		char data[250];
		int read = recv(clientesSocketID[numCliente],data,250,0); //recibir mensajes del cliente 
		data[read] = '\0';
		printf("%s",data);
		
		
		if(strncmp(data,"Bye",3)==0){//terminar cliente
			close(clientesSocketID[numCliente]);
			clientesSocketID[numCliente] = -1;
			break;
		} else{//mandar el mensaje a los demás clientes 
			for(int i = 0;i<clientsConnected;i++){
				if(i!=numCliente && clientesSocketID[i]!=-1){
					send(clientesSocketID[i],data,250,0);
				}
			}
		}
		
		
	}
	pthread_exit(NULL);
}



int main(int argc, char **argv){
	signal(2,signalHandler);
	
	int port = atoi(argv[1]);
	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	bind(sockfd,(const struct sockaddr *)&serverAddress,sizeof(serverAddress));
	listen(sockfd, 5);
	char message[250]; // los mensajes son de máximo 250 caracteres
	char muchosconectados[250]; // variable utilizada en caso de tener más de 5 clientes en el servidor
	strcpy(message, "Bienvenido al chat!\n");
	strcpy(muchosconectados,"Hay muchos conectados\n");
	while(1){
		struct sockaddr_in clientAddress;
		
        int clientSize = sizeof(clientAddress);
		if(clientsConnected<5){
			clientesSocketID[clientsConnected] = accept(sockfd, (struct sockaddr *)&clientAddress, (unsigned int*) &clientSize);
			write(clientesSocketID[clientsConnected],message,strlen(message)); // aqui en strlen se especificica la longitud máxima del mensaje
			pthread_create(&thread[clientsConnected], NULL, ManejoCliente, (void *) &clientsConnected);
			clientsConnected++;
		} else{ 
			//Caso de demasiados conectados, se acepta el cliente solo para mandar mensaje de servidor lleno
			int clientID;
			clientID = accept(sockfd, (struct sockaddr *)&clientAddress, (unsigned int*) &clientSize);
			write(clientID,muchosconectados,strlen(muchosconectados));
			close(clientID);
		}
        
	}
	pthread_exit(NULL);
	
	
	
	return 0;
}