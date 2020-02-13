#include <stdio.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    long SIZE = atoi(argv[1]);
    struct timeval start, end;                                              // Se inicializan las variables.
    double StopWatch;
    pid_t pid;
    key_t key = 7777;
    int shmId, r;
    char *ap;
    char data = '0';
    gettimeofday(&start, NULL);                                             // Se obtiene la hora actual.
    shmId = shmget(key,sizeof(char),0666|IPC_CREAT);                        // Se crea la memoria compartida.
    if(shmId < 0){
        perror("The shared memory couldn't be created.");
        exit(EXIT_FAILURE);
    }
    ap = (char *) shmat(shmId,0,0);                                         // Se obtiene el apuntador de la memoria compartida.
    *ap = '1';
    pid = fork();                                                           // Se crea el subproceso.
    if(pid == -1){
        perror("The subprocess couldn't be created.");
        exit(EXIT_FAILURE);
    }else if(pid == 0){                                                     // Se Escribe el dato en la memoria compartida solo si ya se ha leido.
        for(long i = 0; i<SIZE; i++){
            while(*ap == '0'){}
            *ap = data;
        }
    }else{
        for(long i = 0; i<SIZE; i++){                                       // Se lee el dato de la memoria compartida solo si ya se ha escrito.
            while(*ap == '1'){}
            data = *ap;
            *ap = '1';
        }
        gettimeofday(&end, NULL);                                           // Se obtiene la hora.
        StopWatch = (double)(end.tv_sec + (double)end.tv_usec/1000000) -
                    (double)(start.tv_sec + (double)start.tv_usec/1000000); // Se restan las horas.
        printf("The excecution of the program has taken: %.16g \n",StopWatch*1000.0); // Se muestra el resultado.
        printf("%liB have been sent.\n\n",SIZE);
        r = shmdt(ap);                                                          // Se elimina la memoria compartida.
        if(r < 0){
            perror("The shared memory couldn't be deleted");
            exit(EXIT_FAILURE);
        }
    }
    return 0;

}