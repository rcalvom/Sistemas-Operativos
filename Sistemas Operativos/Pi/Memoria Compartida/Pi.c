#include <stdio.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    double Pi = 0;                                          // Se declaran Variables.
    unsigned long countS = 0, countE = 0;
    pid_t pid;
    key_t key = 7777;
    int shmId, r, status;
    double *ap;
    shmId = shmget(key,sizeof(double),0666|IPC_CREAT);    // Se reserva el espacio de la memoria compartida.
    if(shmId == -1){
        perror("The shared memory couldn't be created.");
        exit(EXIT_FAILURE);
    }
    ap = (double *) shmat(shmId,0,0);                       // Se obtiene la ubicación de la memoria compartida.
    pid = fork();
    if(pid == -1){
        perror("The subprocess couldn't be created.");
        exit(EXIT_FAILURE);
    }else if(pid == 0){                                     // Se asignas los limites de la sumatoria.
        countS = 6E9;
        countE = 12E9;
    }else{
        countS = 1;
        countE = 6E9;
    }
    for(long i = countS; i<countE;i++){                     // Se suma.
        Pi += (4.0)/(2*i-1);
        i++;
        Pi -= (4.0)/(2*i-1);
    }
    if(pid == 0){                                           // Se escribe en la memoria compartida el dato obtenido.
        *ap = Pi;
    }else{
        wait(&status);                                      // Espera a que el otro proceso termine.
        Pi += *ap;                                          // Suma los valores.
        printf("\nThe approximation to the value of Pi is: %1.12f\n\n",Pi); // Se Imprime el valor obtenido.
        r = shmdt(ap);                                      // Elimina la memoria compartida.
        if(r < 0){
            perror("The shared memory couldn't be deleted");
            exit(EXIT_FAILURE);
        }
    }

    return 0;

}