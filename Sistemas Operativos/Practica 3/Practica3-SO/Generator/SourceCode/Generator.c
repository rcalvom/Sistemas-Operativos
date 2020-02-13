#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Hash.h"
#include "Functions.h"
#include "ShippingData.h"

#define SIZE 32
#define NAMES 1700


int main(int argc, char* argv[]){
    long NUMESTRUCTURAS, id;
    FILE *file, *names, *historia;
    char* AllNames;

    NUMESTRUCTURAS = atoi(argv[1]);
    AllNames = Malloc(SIZE * NAMES);
    
    srand(time(NULL));                                                              // Se elige una semilla aleatoria.
    struct HashTable *table = CreateTable();                                        // Se Inicializa la tabla hash.
    char* Breeds[] = {"Labrador", "Pug", "Bulldog", "Chihuahua", "Golden",          
                      "Dalmata", "Pastor", "Labrador", "San Bernardo"};             // Se crea arreglo con razas.
    char Genders[] = {'M', 'H'};                                                    // Se crea arreglo con géneros.
    file = fopen("../Server/dataDogs.dat", "w+");                                   // Se abre el archivo que contendrá las estructuras.
    names = fopen("nombresMascotas.txt", "r");                                      // Se abre el archivo que tiene los nombres.
    if(names == NULL){
        perror("No se encontro el archivo de nombres.\n");
        exit(EXIT_FAILURE);
    }
    system("mkdir ../Server/HashNodes");
    system("mkdir ../Server/historias");

    readFile(AllNames, names, SIZE, NAMES);
    fclose(names);
    struct dogType newRegister;
    
    printf("Generador de estructuras:\n\n");
    printf("Se van a crear %li estructuras...\n", NUMESTRUCTURAS);

    for(int i = 0; i < NUMESTRUCTURAS; i++){
        bzero(&newRegister, sizeof(struct dogType));                                // Crea una estructura con 0 en todos sus bits.

        int rnames = rand() % 1700;                                                 // Elije un número aleatorio entre 1 y 1000.
        char *name = AllNames + (rnames * 32);

        strcpy(newRegister.name, name);

        strcpy(newRegister.type, "Perro");
        newRegister.age = rand() % 14 + 1;                                          // Selecciona un número aleatorio entre 1 y 14.

        strcpy(newRegister.breed, Breeds[rand() % 8]);                              // Selecciona una raza aleatoria del arreglo Breeds.

        

        newRegister.height = rand() % 14 + 1;                                       // Elije una estatura aleatoria entre 1 y 14.

        newRegister.weight = rand() % 14 + 1;                                       // Elije un peso aleatorio entre 1 y 15.

        newRegister.gender = Genders[rand() % 2];                                   // Elije un género aleatorio entre 'M' y 'H'.
        id = insertElement(table, &newRegister.name[0]);                            // Se agrega el elemento a la tabla hash.
        fwrite(&id, sizeof(id), 1, file);                                           // Se escribe la id del elemento en la tabla hash.
        fwrite(&newRegister, sizeof(struct dogType), 1, file);                      // Se escriben los datos anteriormente solicitados en el archivo.

    }
    printf("Registros creados correctamente.\n\n");
    fclose(file);
    printf("%i registros en la tabla\n", table->Elements);
    printf("Guardando tabla... ");
    SaveTable(table);
    printf("Tabla guardada.\n");
    Free(AllNames);
    return 0;
}