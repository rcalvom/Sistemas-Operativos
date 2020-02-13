#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>
#include "Functions.h"
#include "Hash.h"
#include "ShippingData.h"
#include "Options.h"

#define PORT 1234
#define BACKLOG 32

// Variables globales.
struct HashTable *Table;                        // Tabla Hash.
sem_t *semaphore_hash;                          // Semáforo para controlar acceso a la tabla hash.
pthread_mutex_t mutex_dataDogs;                 // Mutex para controlar acceso al archivo de estructuras.
pthread_mutex_t mutex_log;                      // Mutex para controlar acceso al archivo de registros.
pthread_mutex_t mutex_res;                      // Mutex para controlar acceso al archivo de resultados de búsqueda.
int pipefd[2], erroresPipe;                     // Tuberia para la sincronizacion de las historias clinicas
long HistoriasAbiertas[32];

// Método para el hilo que se encarga de recibir datos de los clientes y ejecutar las solicitudes.
void* ListenRequest(void* args){
    struct Client *Client = (struct Client*) args;

    while(true){    
        int option;                                  
        Recv(Client->clientfd, &option, sizeof(option), 0);                     // Recibe la opción del menú dada por el usuario.

        switch (option){
            case 1: {                                                           // Si la opción del cliente es Ingresar Registro.

                struct dogType *new = Malloc(sizeof(struct dogType));
                Recv(Client->clientfd, new, sizeof(struct dogType), 0);         // Recibe la estructura del cliente.

                pthread_mutex_lock(&mutex_dataDogs);                            // Bloquea el uso del recurso.
                sem_wait(semaphore_hash);                                       // Se señaliza el uso de la tabla hash.
                bool flag = IngresarRegistro(Table, new);                       // La ingresa al sistema (Archivo dataDogs.dat y historia.)
                pthread_mutex_unlock(&mutex_dataDogs);                          // Libera el uso del recurso.
                sem_post(semaphore_hash);                                       // Se señaliza la liberación de la tabla hash.

                Send(Client->clientfd, &flag, sizeof(flag), 0);                 // Envía confirmación al cliente si pudo ingresar el registro.
                if(flag){
                    pthread_mutex_lock(&mutex_log);                             // Bloquea el uso del recurso.
                    WriteLog(1, inet_ntoa(Client->Ip.sin_addr), new->name);     // Si se pudo añadir la historia correctamente, Se muestra lo dicho en el Log.
                    pthread_mutex_unlock(&mutex_log);                           // Libera el uso del recurso.
                }               
                Free(new);
                break;
            }

            case 2: {                                                           // Si la opción del cliente es Ver Registro.

                long idRegister;
                Recv(Client->clientfd, &idRegister, sizeof(idRegister), 0);     // Recibe el id del registro que va a buscar.
                sem_wait(semaphore_hash);                                       // Se señaliza el uso de la tabla hash.
                bool existFile = ExisteElElemento(idRegister);                  // Analiza si esa id existe en la tabla hash.
                sem_post(semaphore_hash);                                       // Se señaliza la liberación de la tabla hash.
                Send(Client->clientfd, &existFile, sizeof(existFile), 0);       // Envía al cliente si existe o no dicha id en la tabla hash.
                if(existFile){                                                  // Si exíste ...
                    bool answer;
                    Recv(Client->clientfd, &answer, sizeof(answer), 0);         // Recibe la respuesta de si el cliente quiere abrir el archivo.
                    if(answer){                                                 // Si la respuesta es afirmativa ...
                        FILE *file;
                        char* data, *id, a;

                        // TODO: Semaphore+ historia.dat
                        bool Disponible = true;
                        for(int i = 0; i<32; i++){
                            if(HistoriasAbiertas[i] == idRegister){
                                Disponible = false;
                            }
                        }
                        Send(Client->clientfd, &Disponible, sizeof(Disponible), 0);
                        if(Disponible){
                            erroresPipe = read(pipefd[0], &a, 1);
                            for(int i = 0; i<32; i++){
                                if(HistoriasAbiertas[i] == -1){
                                    HistoriasAbiertas[i] = idRegister;
                                    break;
                                }
                            }
                            erroresPipe = write(pipefd[1], &a, 1);
                            char *filePath = FilePath(idRegister);
                            file = fopen(filePath, "r");
                            if(file == NULL){                                       // Si la historia clínica no exite ...
                                pthread_mutex_lock(&mutex_dataDogs);                // Bloquea el uso del recurso.
                                struct dogType* pet = FindPetById(idRegister);
                                pthread_mutex_unlock(&mutex_dataDogs);              // Libera el uso del recurso.
                                CreateClinicHistory(idRegister, pet);               // Y cree el archivo de historia clínica.
                                Free(pet);
                                file = fopen(filePath, "r");
                            }
                            
                            fseek(file, 0L, SEEK_END);
                            long size = ftell(file);
                            rewind(file);
                            Send(Client->clientfd, &size, sizeof(size), 0);         // Envía el tamaño del archivo a recibir.
                            data = Malloc(size + 1);
                            fread(data, size, 1, file);
                            Send(Client->clientfd, data, size, 0);                  // Envía la historia clinica.
                            fclose(file);
                            Free(data); 
                            Recv(Client->clientfd, &size, sizeof(size), 0);         // Luego que el usuario edite la historia. Recibe el tamaño de la historia modificada.
                            data = Malloc(size + 1);
                            Recv(Client->clientfd, data, size, 0);                  // Recibe la historia nueva.
                            file = fopen(filePath, "w+");        
                            fwrite(data, size, 1, file);                            // La escribe en el archivo.
                            fclose(file);
                            
                            // TODO: Semaphore- historia.dat
                            erroresPipe = read(pipefd[0], &a, 1);
                            for(int i = 0; i<32; i++){
                                if(HistoriasAbiertas[i] == idRegister){
                                    HistoriasAbiertas[i] = -1;
                                    break;
                                }
                            }
                            erroresPipe = write(pipefd[1], &a, 1);

                            id = Malloc(10);
                            sprintf(id, "%li", idRegister);
                            pthread_mutex_lock(&mutex_log);                         // Bloquea el uso del recurso.
                            WriteLog(2, inet_ntoa(Client->Ip.sin_addr), id);        // Registra la busqueda en los Logs.                        
                            pthread_mutex_unlock(&mutex_log);                       // Libera el uso del recurso.
                            Free(id);
                            Free(data);
                            Free(filePath);
                        }
                    }
                }

                break;
            }
            case 3: {                                                                               // Si la opción del cliente es Borrar Registro.

                long NumRegisters;
                bool flag;

                NumRegisters = Table->Elements;
                Send(Client->clientfd, &NumRegisters, sizeof(NumRegisters), 0);                     // Envía al cliente la cantidad de registros en la tabla hash.
                Recv(Client->clientfd, &flag, sizeof(flag), 0);                                     // Recibe la confirmación del cliente si continuar con la operación.    
                if(flag){                                                                           // Si el usuario permite continuar con la operación.
                    long id, exist;
                    bool answer;

                    Recv(Client->clientfd, &id, sizeof(id), 0);                                     // Recibe la id del registro a eliminar.
                    sem_wait(semaphore_hash);                                                       // Se señaliza el uso de la tabla hash.
                    exist = borrar(Table, id);                                                      // Intenta borrar de la tabla hash.
                    sem_post(semaphore_hash);                                                       // Se señaliza la liberación de la tabla hash.

                    if(exist != -1){                                                                // Si el registro fue borrado de la tabla hash ...                     
                        char* idChar;
                        answer = true;

                        pthread_mutex_lock(&mutex_dataDogs);                                        // Bloquea el uso del recurso.
                        BorrarRegistro(id);                                                         // Borra el registro del archivo de estructuras.
                        pthread_mutex_unlock(&mutex_dataDogs);                                      // Libera el uso del recurso.
                        Send(Client->clientfd, &answer, sizeof(answer), 0);                        
                                                                               
                        idChar = Malloc(10);
                        sprintf(idChar, "%li", id);
                        pthread_mutex_lock(&mutex_log);                                             // Bloquea el uso del recurso.
                        WriteLog(3, inet_ntoa(Client->Ip.sin_addr), idChar);                        // Escribe el registro de la acción.
                        pthread_mutex_unlock(&mutex_log);                                           // Libera el uso del recurso.
                        Free(idChar);
                    }else{
                        answer = false;
                        send(Client->clientfd, &answer, sizeof(answer), 0);
                    }                                                                               
                }

                break;
            }
            case 4: {                                                               // Si la opción del cliente es Buscar Registro.

                FILE *file;
                char *name, *rName;
                long rId;

                name = Malloc(SIZE);
                rName = Malloc(SIZE);

                Recv(Client->clientfd, name, SIZE, 0);                              // Recibe el nombre de la mascota a buscar.
                sem_wait(semaphore_hash);                                           // Se señaliza el uso de la tabla hash.
                pthread_mutex_lock(&mutex_res);                                     // Bloquea el uso del recurso.
                long cant = buscarId(Table, name);
                sem_post(semaphore_hash);                                           // Se señaliza la liberación de la tabla hash.
                file = fopen("Res.dat", "r");
                Send(Client->clientfd, &cant, sizeof(cant), 0);

                for(long i = 0; i < cant; i++){                                     // Cada coincidencia encontrada.
                    fread(&rId, sizeof(long), 1, file);                             // Se lee y se envía.
                    Send(Client->clientfd, &rId, sizeof(long), 0);
                    fread(rName, SIZE, 1, file);
                    Send(Client->clientfd, rName, SIZE, 0);
                }
                if(file != NULL){
                    fclose(file);
                }
                remove("Res.dat");
                pthread_mutex_unlock(&mutex_res);                                   // Libera el uso del recurso.
                pthread_mutex_lock(&mutex_log);                                     // Bloquea el uso del recurso.
                WriteLog(4, inet_ntoa(Client->Ip.sin_addr), name);                  // Escribe la acción en el Log.
                pthread_mutex_unlock(&mutex_log);                                   // Libera el uso del recurso.
                Free(name);
                Free(rName);
                break;
            }
        }
        option = 0;                                                                 // Reinicia la opción recibida.
    }
}

//Método para hilo que se encarga de cerrar el proceso si el usuario lo solicita.
void *ListenExit(void *client){
    char *exitKey = Malloc(5);
    while(true){
        scanf("%s", exitKey);                                                       // Lee del teclado una cadena.
        toUpperCase(exitKey);
        if(equals(exitKey, "EXIT")){                                                // Si la cadena es "EXIT".
            sem_close(semaphore_hash);                                              // Libera recursos.
            sem_unlink("S");
            pthread_mutex_destroy(&mutex_dataDogs);
            pthread_mutex_destroy(&mutex_log);
            pthread_mutex_destroy(&mutex_res);
            SaveTable(Table);
            exit(EXIT_SUCCESS);                                                     // Sale del programa sin enviar error.
        }
    }
}

// Método principal. Punto de partida inicial de la practica.
int main(){
    int serverfd, CurrentUsers;
    struct sockaddr_in server; 
    socklen_t len;                                 
    pthread_t ListenThread;
    struct Client *clientsConnected[BACKLOG];
    pipe(pipefd);                                                                   //1 es escritura 0 es lectura 
    if(pipefd == NULL){
        perror("No se pudo crear la tuberia.");
        exit(-1);
    }
    erroresPipe = write(pipefd[1], "a", 1);
    for(int i = 0; i<32; i++){
        HistoriasAbiertas[i] = -1;
    }
    sem_close(semaphore_hash);                                                          // Se eliminan posibles instancias del semáforo.
    sem_unlink("S");
    Table = CreateTable();                                                              // Se crea o carga la tabla hash.
    len = sizeof(struct sockaddr_in);

    for(int i = 0; i < BACKLOG; i++){
        clientsConnected[i] = Malloc(sizeof(struct Client)); 
    }

    serverfd = socket(AF_INET, SOCK_STREAM, 0);                                         // Se configura y se crea el socket.
    if(serverfd == -1){                                                                 // Verificación de error.
        perror("El socket no pudo ser creado.\n");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8); 

    Bind(serverfd, (struct sockaddr*) &server, len);                                    // Inicializa el puerto y la Ip.
    Listen(serverfd, BACKLOG);                                                          // Se pone el servidor a la escucha de solicitudes entrantes.

    pthread_create(&ListenThread, NULL, ListenExit, NULL);                              // Se crea un hilo que se encarga de esperar salida por parte del usuario.
    semaphore_hash = sem_open("S", O_CREAT, 0700, 1);                                   // Inicializa el semáforo que ayudará a evitar conciciones de carrera en los archivos de datos.
    pthread_mutex_init(&mutex_dataDogs, NULL);                                          // Inicializa el mutex del archivo de estructuras.
    pthread_mutex_init(&mutex_log, NULL);                                               // Inicializa el mutex del archivo de registros.
    pthread_mutex_init(&mutex_res, NULL);                                               // Inicializa el mutex del archivo de resultados de búsqueda.

    CurrentUsers = 0;

    while(true){
        int clientfd = accept(serverfd, (struct sockaddr*) &clientsConnected[CurrentUsers]->Ip, &len);        // Se acepta una solicitud de conexión entrante.
        clientsConnected[CurrentUsers]->clientfd = clientfd;
        if(clientfd != -1){
            pthread_create(&clientsConnected[CurrentUsers]->idThread, NULL, ListenRequest, clientsConnected[CurrentUsers]);  // Se crea un hilo que se encarga de recibir datos entrantes.
            CurrentUsers++;
        }
    } 

    return 0;
}
