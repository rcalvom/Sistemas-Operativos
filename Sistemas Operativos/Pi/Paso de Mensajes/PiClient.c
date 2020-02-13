#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define PORT 3535

int main(int argc, char* argv[]){
    int clientfd, r;                                                    // Se declaran variables.
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr);
    double Pi = 0, PiFork = 0;
    clientfd = socket(AF_INET,SOCK_STREAM,0);                           // Se inicializa el socket.
    if(clientfd == -1){
        perror("Socket couldn't be created.");
        exit(-1);
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr(argv[1]);    
    bzero(client.sin_zero,8);
    r = connect(clientfd,(struct sockaddr*)&client,len);                // Se conecta a el servidor.
    for(long i = 1; i<6E9;i++){                                         // Se realiza la sumatoria.
        Pi += (4.0)/(2*i-1);
        i++;
        Pi -= (4.0)/(2*i-1);
    }
    if(r == -1){
        perror("The connection couldn't be created.");
        exit(-1);
    }do{
        r = recv(clientfd,&PiFork,sizeof(double),0);                    // Se recibe el dato obtenido por el servidor.
    }while(r!=sizeof(double));
    Pi += PiFork;                                                       // Se realiza la suma.
    printf("\nThe approximation to the value of Pi is: %1.12f\n\n",Pi); // Se imprime el valor obtenido.
    close(clientfd);                                                    // Se cierra el socket.
    return 0;
}