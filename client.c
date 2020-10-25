#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *RecibirMensajes(void* sockfd){
	int clientSocket = *((int *) sockfd);
	while(1){
		char data[250];
		int read = recv(clientSocket,data,250,0);
		data[read] = '\0';
		printf("%s",data);
		if(strcmp(data,"Bye desde el server\n")==0){
			pthread_exit(NULL);
		}
	}
}

int main(int argc, char **argv){
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char* username = argv[3];
	
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	struct hostent *serverInfo = gethostbyname(ip);
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	bcopy((char *)serverInfo->h_addr,
        (char *)&serverAddr.sin_addr.s_addr, serverInfo->h_length);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0){
        printf("ERROR\n");
    }
	
	char bienvenida[250];
	strcpy(bienvenida, "");
	strcat(bienvenida, username);
	strcat(bienvenida, " se ha unido al chat...\n");
	send(sockfd,bienvenida,strlen(bienvenida),0);
	
	pthread_t thread;
	pthread_create(&thread, NULL, RecibirMensajes, (void *) &sockfd );
	
	
	while(1){
		char c;
		char message[250];
		char bye[250];
		char byebye[250];
		char despedida[250];
		char complete_message[250];
		
		strcpy(byebye, "Bye\n");
		
		strcpy(despedida, "");
		strcat(despedida, username);
		strcat(despedida, " se ha salido del chat...\n");
		strcpy(bye,username);
		strcat(bye,": ");
		strcat(bye,"bye\n");
		
		
		strcpy(complete_message, "");
		strcat(complete_message, username);
		strcat(complete_message, ": ");
		fgets(message,250,stdin);
		strcat(complete_message, message);
		
		if(strcmp(complete_message,bye)==0){
			send(sockfd,complete_message,strlen(complete_message),0);
			send(sockfd,despedida,strlen(despedida),0);
			send(sockfd,byebye,strlen(byebye),0);
			close(sockfd);
			exit(0);
		} else {
			send(sockfd,complete_message,strlen(complete_message),0);
		}
		
	}
	
	
	return 0;
}
