#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_PROCESOS 1

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
    struct datos_tipo hilo1_datos, hilo2_datos;
    pthread_t idhilo1, idhilo2, idhilo3, idhilo4;
    
    hilo1_datos.dato = 1;
    hilo2_datos.dato = 2;
    hilo1_datos.p = 10;
    hilo2_datos.p = 5;

    semaforo = sem_open("S",O_CREAT,0700,MAX_PROCESOS);

    error = pthread_create(&idhilo1,NULL,proceso,(void*)(&hilo1_datos));
    if(error != 0){
        perror("No se ha podido crear el hilo.");
        exit(-1);
    }

    error = pthread_create(&idhilo2,NULL,proceso,(void*)(&hilo2_datos));
    if(error != 0){
        perror("No se ha podido crear el hilo.");
        exit(-1);
    }
    pthread_join(idhilo2,(void**)&valor_devuelto);
    pthread_join(idhilo1,(void**)&valor_devuelto);

    sem_close(semaforo);
    sem_unlink("S");
    return 0;
}