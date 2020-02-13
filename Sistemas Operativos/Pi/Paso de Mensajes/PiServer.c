#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORT 3535
#define BACKLOG 2

int main(){
    int serverfd, clientfd, r;                                      // Se declaran variables.
    double Pi = 0;
    struct sockaddr_in server, client;                              // Se configura y se crea el socket.
    socklen_t len = sizeof(struct sockaddr);
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd == -1){
        perror("The socket couldn't be created.");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero((server.sin_zero), 8); 
    r = bind(serverfd, (struct sockaddr*)&server, len);
    if(r == -1){
        perror("The addres couldn't be asigned");
        exit(EXIT_FAILURE);
    }
    r = listen(serverfd, BACKLOG);                                  // Se pone el servidor a la escucha de solicitudes entrantes.
    if(r == -1){
        perror("The server can't listen for connections.");
        exit(EXIT_FAILURE);
    }
    clientfd = accept(serverfd, (struct sockaddr*) &clientfd, &len); // Se acepta una solicitud de conexión entrante.
    if(clientfd == -1){
        perror("The connection couldn't be accepted.");
        exit(EXIT_FAILURE);
    }

    for(long i = 6E9; i<12E9;i++){                                  // Se realiza la sumatoria.
        Pi += (4.0)/(2*i-1);
        i++;
        Pi -= (4.0)/(2*i-1);
    }

    r = send(clientfd, &Pi, sizeof(double), 0);                     // Se envía el dato obtenido.
    if(r == -1){
        perror("The message couldn't be sended.");
        exit(EXIT_FAILURE);
    }
    close(clientfd);                                                // Se cierran los sockets.
    close(serverfd);
    return 0;
}