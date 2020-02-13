#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "Functions.h"

#define MAXSIZE 100                     // Tamaño máximo del comando bash.
#define DATELENGTH 14                   // Tamaño de la fecha.

int main(int argc, char *argv[]){
    char *FileName, *FilePath, *FileBackup, *bash, *Date;
    FILE *file;
    struct stat *status;
    struct tm *tm;
    time_t t;

    FileName = argv[1];                             // Se recibe el nombre del archivo.
    FilePath = Malloc(MAXSIZE);                     // Se reservan los espacios necesarios.
    FileBackup = Malloc(MAXSIZE);
    bash = Malloc(MAXSIZE);
    Date = Malloc(DATELENGTH);
    status = Malloc(sizeof(struct stat));
    t = time(NULL);                                 // Se obtiene el tiempo actual.               

    strcat(FilePath, "Files/");                     // Se concatenan las dos rutas a verificar. El archivo original y su copia de seguridad.
    strcat(FilePath, FileName);
    strcat(FileBackup, "Backups/");
    strcat(FileBackup, FileName);
    strcat(FileBackup, "_");

    file = fopen(FilePath, "r");                    // Si el archivo original no existe.
    if(file == NULL){
        printf("No se ha encontrado el archivo.\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    Stat(FilePath, status);                         // Si si existe, se lee los datos del archivo en la estructura status.

    tm = localtime(&status->st_mtim.tv_sec);        // Se obtiene la ultima fecha y hora de modifición del archivo.
    strftime(Date, DATELENGTH, "%y%m%d-%H%M%S", tm);
    strcat(FileBackup, Date);

    file = fopen(FileBackup, "r");
    if(file == NULL){                                               // Si la copia no ha sido creada, se crea.
        strcat(bash, "cp \"" );
        strcat(bash, FilePath);
        strcat(bash, "\" \"");
        strcat(bash, FileBackup);
        strcat(bash, "\"");
        system(bash);                                               // Se realiza la copia con un comando bash.
        printf("Se ha creado la copia de seguridad correctamente.\n");
    }else{                                                          // Si la copia ya ha sido creada, se crea el enlace duro...
        tm = localtime(&t);
        bzero(Date, DATELENGTH);
        bzero(FileBackup, strlen(FileBackup));
        strcat(FileBackup, "Backups/");
        strcat(FileBackup, FileName);
        strcat(FileBackup, "_");
        strftime(Date, DATELENGTH, "%y%m%d-%H%M%S", tm);            // ... con la fecha actual del sistema.
        strcat(FileBackup, Date);
        fclose(file);
        strcat(bash, "ln \"" );
        strcat(bash, FilePath);
        strcat(bash, "\" \"");
        strcat(bash, FileBackup);
        strcat(bash, "\"");
        system(bash);
        printf("Se ha creado el enlace duro correctamente.\n");
    }
    free(FilePath);                                                 // Al finalizar se libera el espacio inicialmente reservado.
    free(FileBackup);
    free(bash);
    free(Date);
}