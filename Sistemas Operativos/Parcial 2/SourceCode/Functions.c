#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/stat.h>

// Función que libera size bytes de memoria, estos bytes estan inicializados en 0.
void* Malloc(size_t size){
    void* pointer = malloc(size);
    if(pointer == NULL){
        perror("Error reservando memoria.\n");
    }
    bzero(pointer, size);
    return pointer;
}

int Stat(const char *pathname, struct stat *statbuf){
    int r = stat(pathname, statbuf);
    if(r == -1){
        perror("Error leyendo la información del archivo.\n");
    }
    return r;
}