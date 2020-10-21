#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv){
	char* ip = argv[1];
	char* port = argv[2];
	char* username = argv[3];
	
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
    if(connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress))<0){
        printf("ERROR\n");
    }
	char* bienvenida = username + "se ha conectado al chat.";
	write(sockfd,bienvenida,strlen(bienvenida),0);
	
	
	while(1){
		fgets(message,250,stdin);
		if(strcmp(message,"bye")==0){
			char* despedida = username + "se ha desconectado del chat";
			write(sockfd,despedida,strlen(despedida),0)
			close(sockfd)
		}
		write(sockfd,message,strlen(message),0);
		while(read(sockfd,&c,1)){
        printf("%c", &c);
    	}
		
	}
	
	
	return 0;
}
