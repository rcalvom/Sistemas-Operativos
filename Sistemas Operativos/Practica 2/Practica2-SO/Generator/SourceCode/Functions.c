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

// Lee un nombre de un archivo.
void readFile(char* name, FILE* file, int size, int names){
    for(int j = 0; j < names; j++){
        int i = 0;
        char *a;
        do{
            a = (name + i + (size * j));
            fread(a, 1, 1, file);
            i++;
        }while(*a != '\n' && i < size);
        *a = '\0';
    }
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

// Equivalente a función free, aqui al apuntador se asigna nulo.
void Free(void *ptr){
    free(ptr);
    ptr = NULL;
    return;
}

