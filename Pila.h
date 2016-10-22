#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
//Jorge Mendez 10-10449
//Arturo D'amato 09-11275

/*
Modelo de representaci�n:
Se define Nodo como un struct Caja y a Pila como un apuntador
a un Nodo. Esto es, una Pila es un apuntador a un struct Caja.
*/

typedef struct Cajita {
   void *val;
   struct Cajita *sig;
} Nodo;

typedef Nodo *Pila;

//Pre: True. Retorna una pila vac�a.
Pila vacia();

//Pre: true. Agrega el apuntador x al tope de la pila p.
void push(void *x, Pila *p);

//Pre: True. Retorna 1 si p es vac�a y 0 si no.
int esVacia(Pila p);

//Pre: p no es vac�a. Retorna el apuntador en el tope de la pila p.
void *top(Pila p);

//Pre: p no es vac�a. Elimina el apuntador en el tope de la pila p.
 void pop(Pila *p);

//Pre: True. Agrega el apuntador generico x al final de la pila P.
 void tramposo(void *x, Pila *P);

//Pre: True. Destruye la pila P.
 void destruirPila(Pila *P);

#endif // PILA_H_INCLUDED
