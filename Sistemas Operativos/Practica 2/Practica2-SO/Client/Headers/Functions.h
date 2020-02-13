#ifndef FUNCIONESH
#define FUNCIONESH

void PressToContinue();
void InitConsole();
void DisposeConsole();
void toUpperCase(char *word);
void* Malloc(size_t size);
void Recv(int fd, void *buf, size_t n, int flags);
void Send(int fd, void *buf, size_t n, int flags);
void Free(void *ptr);


#endif
