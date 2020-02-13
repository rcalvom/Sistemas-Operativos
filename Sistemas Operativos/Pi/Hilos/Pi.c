#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int Hilos = 0;
double Pi = 0;
double *PiArray;
long Iteraciones = 0;

void *CalculoPi(void* ap){                                          // Función que calcula una suma parcial del calculo de Pi.
    double Pilocal  = 0;
    long countS = Iteraciones/Hilos* (*(int*)ap) + 1;               // Teniendo en cuenta el número de hilo, calcula los limites de la sumatoria.
    long countE = Iteraciones/Hilos* (*(int*)ap + 1) + 1;           
    for(long i = countS; i<countE;i++){                             // Se calcula Pi usando los limites correspondientes al hilo.
        Pilocal += (4.0)/(2*i-1);
        i++;
        Pilocal -= (4.0)/(2*i-1);
    }
    *(PiArray+(*(int*)ap)) = Pilocal;                               // Se guarda el valor obtenido de la suma parcial en una posición de PiArray.
}


int main(int argc, char* argv[]){ 
    Hilos = atoi(argv[1]);                                          // Obtenemos el número de hilos a utilizar.
    Iteraciones = atol(argv[2]);                                    // Obtenemos el número de iteraciones a realizar.
    PiArray = malloc(sizeof(double)*Hilos);                         // Se declara un espacio de memoria donde cada hilo almacenará una suma parcial.
    int data[Hilos];                                                // Se declara un arreglo para indicarle a los hilos que parte de la suma van a realizar.
    pthread_t tfd[Hilos];                                           // Creamos el arreglo de hilos.
    for(int i = 0; i<Hilos; i++){
        data[i] = i;                                                
        pthread_create(&tfd[i],NULL,CalculoPi,&data[i]);            // Se crean los hilos.
    }
    for(int i = 0; i<Hilos; i++){                                   // Se espera a que todos los hijos hayan terminado.
        pthread_join(tfd[i],NULL);
    }
    for(int i = 0; i<Hilos; i++){                                   // Se suman todas la sumas parciales en a variable Pi
        Pi += *(PiArray+i);
    }
    printf("El valor calculado de Pi es: %1.12f\n\n",Pi);           // Se imprime el valor obtenido en consola.      
    free(PiArray);                                                  // Se librera espacio de memoria ya no utilizado.
    return 0;
}
