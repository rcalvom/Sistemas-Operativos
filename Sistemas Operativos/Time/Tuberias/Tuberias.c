#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char* argv[]){
    long SIZE = atoi(argv[1]);
    struct timeval start, end;                                          // Se definen las variables a utilizar
    double StopWatch;
    char data = '0';
    int pipefd[2], r;
    pid_t pid;
    gettimeofday(&start, NULL);                                         // Se obtiene el tiempo inicial.
    pipe(pipefd);                                                       // Se crea la tubería.
    if(pipefd == NULL){
        perror("The pide couldn't be created.\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();                                                       // Se crea el subproceso.
    if(pid == -1){
        perror("The subprocess couldn't be created.");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        r = close(pipefd[0]);                                           // Cierra el extremo de tuberia que no se va a utilizar.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
        for(long i = 0; i<SIZE; i++){
            r = write(pipefd[1],&data,sizeof(char));                    // Escribe los datos en la tubería.
            if(r == 0){
                perror("The data couldn't be written.");
                exit(EXIT_FAILURE);
            }
        }
        r = close(pipefd[1]);                                           // Cierra el extremo de tuberia ya utilizado.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
    }else{
        r = close(pipefd[1]);                                           // Cierra el extremo que no va a utilizar.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }   
        for(long i = 0; i<SIZE; i++){                                   // Se lee en simultaneo para evitar bloqueo en la tuberia.
            r = read(pipefd[1],&data,sizeof(char));
            if(r == 0){
                perror("The data couldn't be read.");
                exit(EXIT_FAILURE);
            }
        }
        gettimeofday(&end, NULL);                                       // Obtiene la hora actual.
        StopWatch = (double)(end.tv_sec + (double)end.tv_usec/1000000) -
                    (double)(start.tv_sec + (double)start.tv_usec/1000000);     // Resta las dos horas obtenidas.
        printf("The excecution of the program has taken: %.16g \n",StopWatch*1000.0); // Imprime el resultado.
        printf("%liB have been sent.\n\n",SIZE);
        r = close(pipefd[0]);                                           // Cierra el extremo que no va a utilizar.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}