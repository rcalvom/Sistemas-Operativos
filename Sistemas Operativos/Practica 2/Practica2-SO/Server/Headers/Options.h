#ifndef OPCIONESH
#define OPCIONESH
#include "Functions.h"
#include "Hash.h"
#include "ShippingData.h"
#include <stdlib.h>

//Funciones correspondientes a las opciones indicadas.
bool IngresarRegistro(struct HashTable* tabla, struct dogType *new);
int BorrarRegistro(long id);

#endif