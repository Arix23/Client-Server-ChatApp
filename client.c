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
	char* bienvenida = username;
	write(sockfd,bienvenida,strlen(bienvenida));
	
	
	while(1){
		char c;
		char message[250];
		fgets(message,250,stdin);
		if(strcmp(message,"bye")==0){
			char* despedida = strcat(username,"se ha desconectado del chat");
			write(sockfd,despedida,strlen(despedida));
			close(sockfd);
		}
		write(sockfd,message,strlen(message));
		while(read(sockfd,&c,1)){
        printf("%c", &c);
    	}
		
	}
	
	
	return 0;
}
