
#include "Pila.h"
//Jorge Mendez 10-10449
//Arturo D'amato 09-11275

//Pre: True. Retorna una pila vacía.
Pila vacia(){
   Pila p = NULL;
   return p;
}

//Pre: true. Agrega el entero x al tope de la pila p.
void push(void *x, Pila *p){
    Nodo *a=malloc(sizeof(Nodo));
    a->val=x;
    a->sig=*p;
    *p=a;
}

//Pre: True. Retorna 1 si p es vacía y 0 si no.
int esVacia(Pila p){
   return (p==NULL);
}

//Pre: p no es vacía. Retorna el entero en el tope de la pila p.
void *top(Pila p){
   return p->val;
}

//Pre: p no es vacía. elimina el entero en el tope de la pila p.
void pop(Pila *p){
   Pila aux=*p;
   *p=(*p)->sig;
   free(aux);
}

void tramposo(void *x, Pila *P){//Esta funcion se utiliza para el caso en que se esten viendo los mensajes asociados a un
    Pila aux=*P,ant;//telefono. Si se responde un mensaje, este debe ser introducido a la pila para quedar como el primero
    if(aux!=NULL){//en la lista, de manera que si se va al anterior hasta llegar al principio, se vera el mensaje respondido,
        while(aux->sig!=NULL) {//ya que es el mas reciente.
            ant=aux;
            aux=aux->sig;
        }
        aux=malloc(sizeof(Nodo));
        aux->sig=NULL;
        aux->val=x;
        ant->sig=aux;
    }
    else push(x,P);
}

void destruirPila(Pila *P){
    while(!esVacia(*P)) pop(P);
}


