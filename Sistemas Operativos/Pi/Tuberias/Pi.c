#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    int pipefd[2], r;                                   // Se declaran Variables.
    double Pi = 0, PiFork = 0;
    unsigned long countS = 0, countE = 0;
    pid_t pid;
    pipe(pipefd);                                       // Se crea la tubería.
    if(pipefd == NULL){
        perror("The pide couldn't be created.\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();                                       // Se crea el subproceso.
    if(pid == -1){
        perror("The subprocess couldn't be created.");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        r = close(pipefd[0]);                           // Se cierra el extremo de tuberia que no se va a utilizar.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
        countS = 6E9;                                   // Se asignan los limites de la sumatoria.
        countE = 12E9;
    }else{
        r = close(pipefd[1]);                           // Se cierra el extremo de tuberia que no se va a utilizar.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
        countS = 1;                                     // Se asignan los limites de la sumatoria.
        countE = 6E9;
    }

    for(long i = countS; i<countE;i++){                 // Se realiza la suma.
        Pi += (4.0)/(2*i-1);
        i++;
        Pi -= (4.0)/(2*i-1);
    }

    if(pid == 0){
        r = write(pipefd[1],&Pi,sizeof(double));        // Se escribe el valor obtenido a través de la tuberia.
        if(r == 0){
            perror("The data couldn't be written.");
            exit(EXIT_FAILURE);
        }
        r = close(pipefd[1]);                           // Cierra el extremo de tuberia restante.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
    }else{
        wait(NULL);                                     // Espera a que el subproceso termine los calculos.
        r = read(pipefd[0],&PiFork,sizeof(double));     // Se Lee lo obtenido por el otro proceso a través de la tuberia.
        if(r == 0){
            perror("The pipe couldn't be read.");
            exit(EXIT_FAILURE);
        }
        Pi += PiFork;                                   // Se Suman los valores.
        printf("\nThe approximation to the value of Pi is: %1.12f\n\n",Pi); // Se Imprime el valor obtenido en consola.
        r = close(pipefd[0]);                           // Cierra el extremo de tuberia restante.
        if(r == -1){
            perror("The pipe couldn't be closed.");
            exit(EXIT_FAILURE);
        }
    }
    return 0;   
}