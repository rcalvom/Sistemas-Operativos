#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <stdbool.h>
#include "Functions.h"

// Determina si dos cadenas de carcteres son iguales
bool equals(char *String1, char *String2){
    for(int i = 0; i < SIZE; i++){
        if(*(String1 + i) != *(String2 + i))
            return false;
        if((int)(*(String1 + i)) == 0)
            break;
    }
    return true;
}

// Convierte un entero a una cadena de caracteres.
char* IntToString(unsigned int toConvert){
    char *c = Malloc(2);
    *c = (char)(toConvert % 10 + '0');
    *(c + 1) = '\0';
    toConvert /= 10;
    while(toConvert > 0){
        char *a = Malloc(2);
        *a = (char)(toConvert % 10 + '0');
        *(a + 1) = '\0';
        strcat(a, c);
        toConvert /= 10;
        Free(c);
        c = a;
    }
    return c;
}

//Pasar cada caracter de una cadena a mayúsculas.
void toUpperCase(char *word){
    for(int i = 0; i < SIZE; i++){
        if(*(word + i) >= 'a' && *(word + i)<='z')
            *(word + i) = (char)(*(word + i) - 32);
    }
}

//Pasar cada caracter de una cadena a minusculas.
void toLowerCase(char *word){
    for(int i = 0; i < SIZE; i++){
        if(*(word + i) >= 'A' && *(word + i) <= 'Z')
            *(word + i) = (char)(*(word + i) + 32);
    }
}

//Copia el cada caracter de la cadena original en la cadena copia
void CopyString(char *original, char *copia){
    for(int i = 0; i < SIZE; i++){
        *(copia + i) = *(original + i);
        if(*(original + i) == '\0')
            break;
    }
}

// Escribe el archivo de registro con la información pertinente.
void WriteLog(int opcion, char* IP, char* Registro){
    int r;
    time_t t; 
    struct tm *tm;
    FILE *file;

    char* log = Malloc(256);
    char* date = Malloc(50);

    file = fopen("serverDogs.log", "a");
    if(file == NULL){
        perror("El archivo de registro no pudo ser abierto.\n");
        return;
    }

    t = time(NULL);
    tm = localtime(&t);
    strftime(date, 50, "%Y/%m/%d - %H:%M:%S", tm);

    strcat(log, "[Fecha ");
    strcat(log, date);
    strcat(log, "] Cliente[");
    strcat(log, IP);
    switch (opcion){
        case 1:{
            strcat(log, "] [Insersión] [");
            break;
        }
        case 2:{
            strcat(log, "] [Lectura] [");
            break;
        }
        case 3:{
            strcat(log, "] [Borrado] [");
            break;
        }
        case 4:{
            strcat(log, "] [Búsqueda] [");
            break;
        }
    }

    strcat(log, Registro);
    strcat(log, "]\n");

    r = fwrite(log, strlen(log), 1, file);
    if(r == 0){
        perror("El archivo de registros no pudo ser escrito.\n");
        return;
    }

    fclose(file);
    Free(log);
    Free(date);
}

// Dada una id, devuelve la ruta de la historia clinica asociada a esa id.
char* FilePath(long id){
    char* idchar = Malloc(5);
    char* filepath = Malloc(46);
    strcat(filepath, "historias/");
    sprintf(idchar, "%li", id);
    strcat(filepath, idchar);
    strcat(filepath, ".dat");
    Free(idchar);
    return filepath;
}

// Escribe la historia clinica de la estructura e id dadas.
bool CreateClinicHistory(long id, struct dogType* pet){
    FILE *file;
    char *data, *age, *height, *weight;
    file = fopen(FilePath(id), "w+");
    data = Malloc(200);
    age = Malloc(10);
    height = Malloc(10);
    weight = Malloc(10);

    sprintf(age, "%i", pet->age);
    sprintf(height, "%i", pet->height);
    sprintf(weight, "%f", pet->weight);

    strcat(data, "Historia Clínica.\n\nNombre: ");
    strcat(data, pet->name);
    strcat(data, "\nTipo: ");
    strcat(data, pet->type);
    strcat(data, "\nEdad: ");
    strcat(data, age);
    strcat(data, "\nRaza: ");
    strcat(data, pet->breed);
    strcat(data, "\nEstatura: ");
    strcat(data, height);
    strcat(data, "\nPeso: ");
    strcat(data, weight);
    strcat(data, "\nGénero: ");
    strcat(data, &pet->gender);
    fwrite(data, strlen(data), 1, file);
    Free(data);
    Free(age);
    Free(height);
    Free(weight);
    fclose(file);
    return true;
}

// Devuelve la estructura con id en el archivo dataDogs.dat
struct dogType* FindPetById(long id){
    FILE *dataDogs;
    struct dogType *pet;
    long idTemp;

    pet = Malloc(sizeof(struct dogType));
    idTemp = 0;
    dataDogs = fopen("dataDogs.dat", "r");

    while(!feof(dataDogs)){
        fread(&idTemp, sizeof(idTemp), 1, dataDogs);
        fread(pet, sizeof(struct dogType), 1, dataDogs);
        if(idTemp == id){
            break;
        }
    }
    fclose(dataDogs);
    return pet;
}

// Función bind que libera e inicializa un puerto.
void Bind(int fd, const struct sockaddr *addr, socklen_t len){
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0){   // Libera el puerto para que pueda ser utilizado.
        perror("Error liberando dirección Ip.");
        exit(EXIT_FAILURE);
    }
    if(bind(fd, addr, len) == -1){
        perror("La dirección IP no pudo ser asignada.\n");
        exit(EXIT_FAILURE);
    }
    return;
}

// Función que permite escuchar solicitudes entrantes de conexión, se evalua el error.
void Listen(int fd, int BACKLOG){
    if(listen(fd, BACKLOG) == -1){                                           // Se pone el servidor a la escucha de solicitudes entrantes.
        perror("El servidor no puede escuchar conexiones.\n");
        exit(EXIT_FAILURE);
    }
    return;
}

// Función que libera size bytes de memoria, estos bytes estan inicializados en 0.
void* Malloc(size_t size){
    void* pointer = malloc(size);
    if(pointer == NULL){
        perror("Error reservando memoria.");
    }
    bzero(pointer, size);
    return pointer;
}

// Equivalente a función recv, aqui se evalua el posible error.
void Recv(int fd, void *buf, size_t n, int flags){
    if(recv(fd, buf, n, flags) == -1){
        perror("Se ha producido un error recibiendo la información.\n");
    }
    return;
}

// Equivalente a función send, aqui se evalua el posible error.
void Send(int fd, void *buf, size_t n, int flags){
    if(send(fd, buf, n, flags) == -1){
        perror("Se ha producido un error enviando la información.\n");
    }
    return;
}

// Equivalente a función free, aqui al apuntador se asigna nulo.
void Free(void *ptr){
    free(ptr);
    ptr = NULL;
    return;
}

