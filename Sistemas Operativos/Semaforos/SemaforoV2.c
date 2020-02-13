#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_PROCESOS 4
#define NUM_HILOS 4

sem_t *semaforo;

struct datos_tipo{
    int dato;
    int p;
};

void *proceso(void *datos){
    struct datos_tipo *datos_proceso;
    datos_proceso = (struct datos_tipo*)datos;
    int a,i,j,p;
    sem_wait(semaforo);
    a = datos_proceso->dato;
    p = datos_proceso->p;

    for(int i = 0; i<=p; i++){
        printf("%i",a);
    }
    fflush(stdout);
    sleep(EXIT_FAILURE);

    for(int i = 0; i<=p;i++){
        printf("-");
    }
    fflush(stdout);
    sem_post(semaforo);
}

int main(){
    int error;
    char *valor_devuelto;

    struct datos_tipo hilos_datos[NUM_HILOS];
    pthread_t idHilos[NUM_HILOS];
    
    semaforo = sem_open("S",O_CREAT,0700,MAX_PROCESOS);

    for(int i = 0; i<NUM_HILOS; i++){
        hilos_datos[i].dato = i;
        hilos_datos[i].p = i;
    }

    for(int i = 0; i<NUM_HILOS; i++){
        error = pthread_create(&idHilos[i],NULL,proceso,(void*)(&hilos_datos[i]));
        if(error != 0){
            perror("No se ha podido crear el hilo.");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i<NUM_HILOS; i++){
        pthread_join(idHilos[i],(void**)&valor_devuelto);
    }


    sem_close(semaforo);
    sem_unlink("S");
    return 0;
}