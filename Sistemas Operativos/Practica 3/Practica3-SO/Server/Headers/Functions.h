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
void WriteLog(int opcion, char* IP, char* Registro);
char* FilePath(long id); 
bool ExisteRegistro(long id);
bool CreateClinicHistory(long id, struct dogType* pet);
struct dogType* FindPetById(long id);
void Bind(int fd, const struct sockaddr *addr, socklen_t len);
void Listen(int fd, int BACKLOG);
void* Malloc(size_t size);
void Recv(int fd, void *buf, size_t n, int flags);
void Send(int fd, void *buf, size_t n, int flags);
void Free(void *ptr);


#endif