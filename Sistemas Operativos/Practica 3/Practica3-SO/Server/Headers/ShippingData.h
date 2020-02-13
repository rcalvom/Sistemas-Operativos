#ifndef SHIPPINGDATAH
#define SHIPPINGDATAH

#include <arpa/inet.h>


// Estructura de almacenamiento de datos.
struct dogType{
    char name[32];
    char type[32];
    int age;
    char breed[16];
    int height;
    float weight;
    char gender;     
};

// Estructura que almacena los datos del cliente.
struct Client{
    int clientfd;
    pthread_t idThread;
    struct sockaddr_in Ip;
};

#endif