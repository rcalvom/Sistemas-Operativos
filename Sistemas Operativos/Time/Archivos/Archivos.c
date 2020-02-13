#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    long SIZE = atoi(argv[1]);
    struct timeval start, end;                               // Se declaran variables.
    int status, r = 0; 
    double StopWatch;
    pid_t pid;
    FILE *file;
    char data = '0';                                        // Se declara un char, el cual será enviado.
    gettimeofday(&start, NULL);                             // Se obtiene el tiempo inicial.
    pid = fork();                                           // Se crea el subproceso.
    if(pid == -1){
        perror("the process couldn't be created.");
        exit(EXIT_FAILURE);
    }else if(pid == 0){                                     
        file = fopen("data.dat","w+");                      // Se crea el archivo.
        if(file == NULL){
            perror("The file couldn't be opened.");
            exit(EXIT_FAILURE);
        }
        for(long i = 0; i<SIZE; i++){                       // Se escribe byte por byte en el archivo.
            r = fwrite(&data,sizeof(char),1,file);
            if(r == 0){
                perror("The file couldn't be written.");
                exit(EXIT_FAILURE);
            }
        }        
        fclose(file);                                       // Cierra el archivo.
    }else{
        wait(&status);                                      // Espera a que el otro proceso termine.
        file = fopen("data.dat","r");                       // Abre el archivo creado por el otro proceso.
        if(file == NULL){                                   
            perror("The file couldn't be opened.");
            exit(EXIT_FAILURE);
        }
        for(long i = 0; i<SIZE; i++){                       // Lee cada uno de los datos existentes en el archivo.
            r = fread(&data,sizeof(char),1,file);
            if(r == 0){
                perror("The file couldn't be read.");
                exit(EXIT_FAILURE);
            }
        }
        fclose(file);                                       // Cierra el archivo.
        remove("data.dat");                                 // Elimina el archivo.
        gettimeofday(&end, NULL);                           // Toma el tiempo final.
        StopWatch = (double)(end.tv_sec + (double)end.tv_usec/1000000) -
                    (double)(start.tv_sec + (double)start.tv_usec/1000000); // Resta los dos tiempos obtenidos.
        printf("The excecution of the program has taken: %.16g miliseconds.\n",StopWatch*1000.0); // Imprime el resultado.
        printf("%liB have been sent.\n\n",SIZE);
    }
    return 0;
}