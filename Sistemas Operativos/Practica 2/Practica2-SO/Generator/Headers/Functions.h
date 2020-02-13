#ifndef FUNCIONESH
#define FUNCIONESH

#define SIZE 32

#include <stdbool.h>
#include "ShippingData.h"

bool equals(char *String1, char *String2);
char* IntToString(unsigned int toConvert);
void toUpperCase(char *word);
void toLowerCase(char *word);
void CopyString(char *original, char *copy);
void* Malloc(size_t size);
void Free(void *ptr);
void readFile(char* name, FILE* file, int size, int names);


#endif