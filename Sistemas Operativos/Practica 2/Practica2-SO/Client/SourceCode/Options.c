#include <stdlib.h>
#include <strings.h>
#include <ncurses.h>
#include "Options.h"
#include "Functions.h"
#include "ShippingData.h"

struct dogType IngresarRegistro(){
    struct dogType new;
    bzero(&new, sizeof(struct dogType));
    int r = 0;
    printw("Ingresar registro:\n");                                         // Se piden los datos y se almacenan en la estructura.
    printw("Indique el nombre de la mascota: ");
    scanw("%s", &new.name[0]);
    printw("Indique el tipo de mascota: ");
    scanw("%s", &new.type[0]);
    printw("Indique la edad de la mascota: ");
    scanw("%i", &new.age);
    printw("Indique la raza de la mascota: ");
    scanw("%s", &new.breed[0]);
    printw("Indique la estatura (cm) de la mascota: ");
    scanw("%i", &new.height);
    printw("Indique el peso (kg) de la mascota: ");
    scanw("%f", &new.weight);
    printw("Indique el género (M/H) de la mascota: ");
    scanw("%s", &new.gender);
    return new;
}

long VerRegistro(){
    long id;
    printw("Por favor ingrese el id del registro que desea ver: ");
    scanw("%li", &id);
    printw("\n");
    return id;
}

long BorrarRegistro(){
    long id;
    printw("Por favor ingrese el id del registro que desea eliminar.\nIngrese -1 para cancelar la operación: ");
    scanw("%li", &id);
    printw("\n");
    return id;
}

char* BuscarRegistro(){
    char *nombre;
    nombre = Malloc(32);
    printw("Por favor ingrese el nombre de la mascota que desea buscar: ");
    scanw("%s", nombre);
    toUpperCase(nombre);
    printw("\n");
    return nombre;
}