#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PROCESOS 1
#define NUM_HILOS 20

pthread_mutex_t mutex; 

struct datos_tipo{
    int dato;
    int p;
};

void *proceso(void *datos){
    struct datos_tipo *datos_proceso;
    datos_proceso = (struct datos_tipo*) datos;
    int a, i, j, p;

    a = datos_proceso ->dato;
    p = datos_proceso ->p;

    pthread_mutex_lock(&mutex);

    for(int i = 0; i<=p; i++){
        printf("%i",a);
    }
    fflush(stdout);
    sleep(1);

    for(int i = 0 ;i<=p; i++){
        printf("-");
    }
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}

int main(){
    int error, i;
    char *valor_devuelto;

    pthread_mutex_init(&mutex,NULL);

    struct datos_tipo hilos_datos[NUM_HILOS];
    pthread_t idHilo[NUM_HILOS];

    for(int i = 0 ; i<NUM_HILOS; i++){
        hilos_datos[i].dato = i;
        hilos_datos[i].p = i+1;
    }

    for(int i = 0; i<NUM_HILOS; i++){
        error = pthread_create(&idHilo[i],NULL,proceso,(void*)(&hilos_datos[i]));
        if(error != 0){
            perror("No se pudo crear el hilo");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i<NUM_HILOS; i++){
        pthread_join(idHilo[i],(void**)&valor_devuelto);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}