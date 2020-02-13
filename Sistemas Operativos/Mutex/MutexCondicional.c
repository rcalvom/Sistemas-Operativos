#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#define MAX_BUFFER 16
#define MAX_DATOS 1000

pthread_mutex_t mutex;
pthread_cond_t vacio, lleno;
int buffer[MAX_BUFFER], num_datos;

struct datos_tipo{
    int dato;
    int p;
};

void *consumidor(){
    struct datos_tipo *datos_proceso;
    int a, j, p, posicion = 0, dato;

    for(int i = 0; i<MAX_DATOS;i++){
        pthread_mutex_lock(&mutex);

        while(num_datos == 0){
            pthread_cond_wait(&lleno,&mutex);
        }

        dato =  buffer[posicion];
        if(posicion == 15){
            posicion = 0;
        }else{
            posicion ++;
        }

        num_datos --;

        if(num_datos == MAX_BUFFER-1){
            pthread_cond_signal(&lleno);
        }

        pthread_mutex_unlock(&mutex);
        printf("\nse ha consimido el dato %d",dato);
        fflush(stdout);
        sleep(1);
    }
    pthread_exit(EXIT_SUCCESS);
}

void *productor(){
    struct datos_tipo *datos_proceso;
    int a,i,j,p,posicion = 0, dato;

    for(int i = 0; i<MAX_DATOS; i++){
        pthread_mutex_lock(&mutex);
        while(num_datos == MAX_BUFFER){
            pthread_cond_wait(&lleno,&mutex);
        }

        buffer[posicion] = i;
        dato = i;

        if(posicion == 15){
            posicion = 0;
        }else{
            posicion ++;
        }

        num_datos ++;

        if(num_datos == 1){
            pthread_cond_signal(&vacio);
        }

        pthread_mutex_unlock(&mutex);
        printf("\nse ha producido el dato: %d",dato);
        fflush(stdout);

    }
    pthread_exit(EXIT_SUCCESS);
}

int main(){

    int error;
    char *valor_devuelto;

    pthread_t idHilo1, idHilo2, idHilo3, idHilo4;

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&lleno,NULL);
    pthread_cond_init(&vacio,NULL);

    error = pthread_create(&idHilo1,NULL,productor,NULL);
    if(error != 0){
        perror("No se pudo crear el hilo");
        exit(EXIT_FAILURE);
    }

    error = pthread_create(&idHilo2,NULL,consumidor,NULL);
    if(error != 0){
        perror("No se pudo crear el hilo");
        exit(EXIT_FAILURE);
    }

    pthread_join(idHilo2,(void**)valor_devuelto);
    pthread_join(idHilo1,(void**)valor_devuelto);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&vacio);
    pthread_cond_destroy(&lleno);

    return 0;
}