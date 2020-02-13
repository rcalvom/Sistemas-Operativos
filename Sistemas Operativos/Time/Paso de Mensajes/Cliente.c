#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define PORT 3535

int main(int argc, char* argv[]){
    long SIZE = atoi(argv[1]);
    struct timeval start, end;                                                      // Se definen las variables.
    double StopWatch;
    gettimeofday(&start, NULL);                                                     // Se toma la hora inicial.
    int clientfd, r;
    struct sockaddr_in client;
    socklen_t len = sizeof(struct sockaddr);
    char data = '0';
    clientfd = socket(AF_INET,SOCK_STREAM,0);
    if(clientfd == -1){
        perror("Socket couldn't be created.");
        exit(EXIT_FAILURE);
    }
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");    
    bzero(client.sin_zero,8);
    r = connect(clientfd,(struct sockaddr*)&client,len);                            // Se conecta al servidor.
    if(r == -1){
        perror("The connection couldn't be created.");
        exit(EXIT_FAILURE);
    }
    for(long i = 0; i<SIZE; i++){                                                   // Se reciben los datos.  
        do{
            r = recv(clientfd,&data,sizeof(data),0); 
        }while(r!=sizeof(data));
    }
    gettimeofday(&end, NULL);                                                       // Se obtiene la hora final.
    StopWatch = (double)(end.tv_sec + (double)end.tv_usec/1000000) -                // Se restan las dos horas obtenidas.
                (double)(start.tv_sec + (double)start.tv_usec/1000000);
    printf("The excecution of the program has taken: %.16g \n",StopWatch*1000.0);   // Se muestra el resultado.
    printf("%liB have been sent.\n\n",SIZE);
    close(clientfd);
    return 0;
}