#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    double Pi = 0, PiFork = 0;                      // Se declaran Variables.
    unsigned long countS = 0, countE = 0;
    pid_t pid;  
    FILE *file;
    pid = fork();                                   // Se crea el subproceso.
    if(pid == -1){                                  // Se comprueba si el hijo no pudo ser creado.
        perror("the process couldn't be created."); 
        exit(EXIT_FAILURE);
    }else if(pid == 0){                             // Se Asignan los valores correspondientes de los limites de la sumatoria.
        countS = 6E9;
        countE = 12E9;
    }else{
        countS = 1;
        countE = 6E9;
    }
    for(long i = countS; i<countE;i++){             // Se calcula Pi usando los limites anteriormente dados.
        Pi += (4.0)/(2*i-1);
        i++;
        Pi -= (4.0)/(2*i-1);
    }
    if(pid == 0){        
        file = fopen("Pi.dat","w+");                // Se crea el archivo.
        if(fwrite(&Pi,sizeof(double),1,file) == 0){ // Se escribe el valor obtenido por el subproceso hijo.
            perror("The file couldn't be created.");
            exit(EXIT_FAILURE);
        }
        fclose(file);                               // Se cierra el archivo.
    }else{
        wait(NULL);                                 // Se espera a que el otro proceso termine.
        file = fopen("Pi.dat","r");
        if(fread(&PiFork,sizeof(double),1,file) == 0){ // Se lee el dato del archivo.
            perror("The file couldn't be read.");
            exit(EXIT_FAILURE);
        }
        fclose(file);
        Pi += PiFork;                               // Se suman los dos valores obtenidos de las sumatorias.
        printf("\nThe approximation to the value of Pi is: %1.12f\n\n",Pi); // Se Imprime el valor en consola.
        remove("Pi.dat");                           // Se elimina el archivo para evitar inconvenientes en una futura ejecución.
    }
    return 0;
}