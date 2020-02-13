#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>

void PressToContinue(){
    printw("\nPulse cualquier tecla para continuar...\n");                        // Espera por una tecla para continuar.
    noecho();                                                                     // Impide que se muestre el caracter en consola.
    getch();                                                                      // Obtiene el caracter.
    echo();                                                                       // Permite de nuevo el ingreso de caracteres.
    clear();
}

// Inicializa la consola de ncurses.
void InitConsole(){
    initscr();                                                                   // Inicializa la consola para uso de la libreria ncurses.
    clear();                                                                     // Limpia la consola.
    refresh();                                                                   // Refresca la consola para poder ser escrita.
}

// Elimina la configuración de la consola de ncurses.
void DisposeConsole(){
    clear();
    endwin();                                                                   // Antes de finalizar, elimina la configuración hecha sobre la consola por parte de la libreria ncurses.
}

//Pasar cada caracter de una cadena a mayúsculas.
void toUpperCase(char *word){
    for(int i = 0; i < 32; i++){
        if(*(word + i) >= 'a' && *(word + i)<='z')
            *(word + i) = (char)(*(word + i) - 32);
    }
}

// Función que libera size bytes de memoria, estos bytes estan inicializados en 0.
void* Malloc(size_t size){
    
    void* pointer = malloc(size);
    if(pointer == NULL){
        printw("Error reservando memoria.");
    }
    bzero(pointer, size);
    return pointer;
}

// Equivalente a función recv, aqui se evalua el posible error.
void Recv(int fd, void *buf, size_t n, int flags){
    if(recv(fd, buf, n, flags) == -1){
        printw("Se ha producido un error recibiendo la información.\n");
    }
    return;
}

// Equivalente a función send, aqui se evalua el posible error.
void Send(int fd, void *buf, size_t n, int flags){
    if(send(fd, buf, n, flags) == -1){
        printw("Se ha producido un error enviando la información.\n");
    }
    return;
}

// Equivalente a función free, aqui al apuntador se asigna nulo.
void Free(void *ptr){
    free(ptr);
    ptr = NULL;
    return;
}

