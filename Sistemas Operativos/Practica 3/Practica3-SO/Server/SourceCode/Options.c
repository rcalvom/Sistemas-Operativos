#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "Functions.h"
#include "Hash.h"
#include "ShippingData.h"

//Opción del menú Ingresar Registro.
bool IngresarRegistro(struct HashTable* tabla, struct dogType *new){
    long id;
    FILE *dataDogs;
    dataDogs = fopen("dataDogs.dat", "a");
    id = insertElement(tabla, new->name);
    fwrite(&id, sizeof(long), 1, dataDogs);                                     // Se escribe en el archivo la Id correspondiente.
    fwrite(new, sizeof(struct dogType), 1, dataDogs);                           // Se escriben los datos de la mascota anteriormente solicitados en el archivo.
    CreateClinicHistory(id, new);                                               // Se crea el archivo de historia clínica correspondiente.
    fclose(dataDogs);
    return true;
}

//Opción del menú Borrar Registro.
int BorrarRegistro(long id){
    long idTemp;
    FILE *file, *temp;
    struct dogType* registro = Malloc(sizeof(struct dogType));

    file = fopen("dataDogs.dat", "r");                                          // Se abre un archivo que contiene las estructuras..
    temp = fopen("temp.dat", "w+");                                             // Se crea un archivo temporal donde se guardaran las estructuras que no serán eliminadas.

    while(feof(file) == 0){
        fread(&idTemp, sizeof(long), 1, file);                                  // ..... Lea los datos .....
        fread(registro, sizeof(struct dogType), 1, file);
        if(idTemp == id){                                                       // ... y exceptuando el que se va a eliminnar...
            continue;
        }
        fwrite(&idTemp, sizeof(long), 1, temp);                                 // ... Escribirlos todos en el archivo temporal
        fwrite(registro, sizeof(struct dogType), 1, temp);
    }

    fclose(file);
    fclose(temp);
    remove("dataDogs.dat");                                                     // Se elimina el archivo viejo y ....
    rename("temp.dat", "dataDogs.dat");
    remove(FilePath(id));
    Free(registro);
    return 0;
}