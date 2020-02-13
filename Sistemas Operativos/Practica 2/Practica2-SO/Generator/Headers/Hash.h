#ifndef HASHH
#define HASHH
#include <stdio.h>
#include <stdbool.h>

#define TAMANOTABLA 1500

struct HashTable{
    int Elements;
    long LastIndex[TAMANOTABLA];
};

struct String {
    int length;
    char *string;
};

int hash(char *name);                                                       //Funcion de hash
bool ExisteElElemento(long id);                                             //Comprueba si el id esta registrado en la tabla
struct String* buscarId(struct HashTable *table, char *nombre);              //Busca los elementos que correspondan con el nombre y los devuelve en una lista encadenada
long borrar(struct HashTable *table, long id);                              //Elimina el elemento que corresponda con el ID
long insertElement(struct HashTable *table, char *nombre);                  //Obtiene el index de un elemento y llamar a la funcion insertar
struct HashTable* CreateTable();                                            //Crea una tabla vacía
void SaveTable(struct HashTable *table);                                    //Guarda la tabla en disco

#endif