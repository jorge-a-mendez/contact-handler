#ifndef AGENDA_H_INCLUDED
#define AGENDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
//Jorge Mendez 10-10449
//Arturo D'amato 09-11275


typedef struct CajaM {
   int tel;      // Clave de busqueda. Es entero positivo de 7 digitos.
   int tip;      // Valores: 0 recibido, 1 enviado.
   int leido;   // Indica si el mensaje ha sido leido. // leido = 1, no leido = 0.
   char tex[40]; // Texto del mensaje.
   struct CajaM *sig;
   struct CajaM *ant;
} Mensaje;

typedef struct CajaL {
   int tel;
   int tip;       // Valores: 0 recibida, 1 saliente, 2 perdida.
   int dur;       // Duracion de la llamada en minutos.
   struct CajaL *sig;
   struct CajaL *ant;
} Llamada;

typedef struct CajaC {
   int tel;       // Clave del contacto. Es un entero positivo de 7 digitos.
   char nom [20]; // Nombre de una persona.
   struct CajaC *der;
   struct CajaC *izq;
   struct CajaC *pad;
} Contacto;

typedef struct CajaCL{//Nodo para crear una lista de apuntadores a contacto para la funcion ver contactos
    Contacto *con;
    struct CajaCL *sig,*ant;
}ContList;

typedef ContList *ListaContactos;

typedef struct Caja {
   char nom [10];     // Nombre con el cual se almacenara la informacion.
   Llamada *lla;      // Apuntador a la lista de llamadas.
   Mensaje *men;      // Apuntador a la lista de mensajes.
   Contacto *con;     // Apuntador al arbol de contactos.
} Agenda;

//Pre: True. Post: agrega la lista de llamadas a la agenda A. Si no existe la agenda, crea el archivo .lla
void leerLlamadas(char A_nom[], Agenda *A);

//Pre: True. Post: agrega la lista de mensajes a la agenda A. Si no existe la agenda, crea el archivo .men
void leerMsjs(char A_nom[], Agenda *A);

//Pre: True. Post: agrega el arbol de contactos a la agenda A. Si no existe la agenda, crea el archibo .con
void leerContactos(char A_nom[], Agenda *A);

//Pre: True. Post: devuelva la agenda a utilizar.
Agenda selectAgenda();

//Pre: Agenda no nula y previamente seleccionada. Post: Agrega una llamada al inicio de la lista de llamadas de A.
void agregarLlamada(Agenda *A);

//Pre: Agenda no nula y previamente seleccionada. Post: Agrega un mensaje al inicio de la lista de mensajes de A.
void agregarMsj(Agenda *A);

//Pre: Agenda no nula y previamente seleccionada. Post: Agrega un contacto al arbol de contactos de A.
void agregarContacto(Agenda *A);

//Pre: **L es un apuntador al apuntador a la llamada a eliminar, no nula. Post: elimina L de la lista de
//llamadas.
void eliminaLlamada(Llamada **L, Agenda *A);

//Pre: El apuntador a L sera Agenda.lla. Post: Imprime en pantalla la ultima llamada, ademas del menu
// con opciones de: eliminar llamada, mostrar siguiente llamada, mostrar llamada anterior, volver al
//menu anterior.
void verLlamadas(Llamada **L,Agenda *A);

//Pre: **M es un apuntador al apuntador al mensaje a eliminar,no nulo. Post: elimina M de la lista de mensajes.
void eliminaMsj(Mensaje **M,Agenda *A);

//Pre: Agenda A no nula y previamente seleccionada. Post: agrega un mensaje enviado a la lista de mensajes,
//con el telefono tel.
void respMsj(Agenda *A, int tel);

//Pre: El apuntador a M sera Agenda.men. Post: Imprime en pantalla el ultimo mensaje enviado o recibido y
//el menu: eliminar mensaje, mostrar siguiente mensaje, mostar mensaje anterior, responder mensaje y volver
//al menu anterior
void verMsjs(Mensaje **M, Agenda *A);

//Pre: El apuntador a M sera Agenda.men. Post: Imprime en pantalla el ultimo mensaje no leido y
//el menu: eliminar mensaje, mostrar siguiente mensaje, mostar mensaje anterior, responder mensaje y volver
//al menu anterior
void verMsjsNoLeidos(Mensaje **M, Agenda *A);

//Pre: *L es el elemento de la lista que contiene el apuntador al contacto a eliminar. El entero a es una con-
//dicion que se pasara a eliminaContList y decidira si devuelve el siguiente o el anterior.Post: Elimina L->con
//del arbol de contactos y L de la lista de contactos..
void eliminaContacto(ListaContactos *L,Agenda *A, int a);

//Pre: a es 0 o 1. Post: Si a es 1, elimina el nodo L de la lista y devuelve el siguiente. Si a es 0, lo elimina
//y devuelve el anterior
void eliminaContList(ListaContactos *L, int a);

//Pre: La lista L contiene todos los contactos del arbol. Post: Imprime en pantalla el primer contacto en orden
//numerico y el menu: eliminar contacto, ver siguiente contacto, ver cntacto anterior y volver al menu anterior.
void verContactos(ListaContactos *L,Agenda *A);

//Pre: Agenda no nula y previamente seleccionada. Post: Imprime en pantalla el ultimo mensaje relacionado con un tlf
//y el menu: eliminar mensaje, mostrar siguiente mensaje, mostar mensaje anterior, responder mensaje y volver
//al menu anterior
void verMsjsTlf(Mensaje **M, Agenda *A, int tel,char contacto[20]);

////Pre: Agenda no nula y previamente seleccionada. Post: Imprime en pantalla la ultima llamada asociada a un tlf,
//ademas del menu con opciones de: eliminar llamada, mostrar siguiente llamada, mostrar llamada anterior, volver al
//menu anterior.
void verLlamadasTlf(Llamada **L, int tel,char contacto[20],Agenda *A);

//Pre: Agenda no nula y previamente seleccionada. Post: Imprime en pantalla las llamadas hechas,
//recibidas y perdidas de cada numero, agrupadas.
void listarLlamadas(Agenda A);

//Pre: Agenda no nula y previamente seleccionada. Post: devuelve la cantidad de minutos de las llamadas hechas.
int tiempLlamHech(Agenda A);

//Pre: Agenda no nula y previamente seleccionada. Post: devuelve la cantidad de minutos de las llamadas recibidas.
int tiempLlamRec(Agenda A);

//Pre: Agenda no nula y previamente seleccionada. Post: devuelve la cantidad de minutos del total de llamadas.
int tiempLlamTot(Agenda A);

//Pre: Agenda no nula y previamente seleccionada. Post: devuelve la cantidad de minutos de las llamadas hechas
//al telefono tel.
int tiempLlamHechTlf(Agenda A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: devuelve la cantidad de minutos de las llamadas recibidas
//del telefeono tel.
int tiempLlamRecTlf(Agenda A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: devuelva la cantidad de minutos de todas las llamadas
//asociadas al telefono tel.
int tiempLlamTotTlf(Agenda A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: elimina todas las llamadas hechas a tel.
void eliminaLlamHech(Agenda *A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: elimina todas las llamadas recibidas de tel.
void eliminaLlamRec(Agenda *A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: elimina todas las llamadas asociadas a tel.
void eliminaLlamTot(Agenda *A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: elimina todos los mensajes enviados a tel.
void eliminaMsjsEnv(Agenda *A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: elimina todos los mensajes recibidos de tel.
void eliminaMsjsRec(Agenda *A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: elimina todos los mensajes asociados a tel.
void eliminaMsjsTot(Agenda *A, int tel);

//Pre: Agenda no nula y previamente seleccionada. Post: guarda en los archivos .lla, .men y . con
//las llamadas, mensajes y contactos de la agneda.
void salvarAgenda(char A_nom[], Agenda A);

void menuVer(Agenda *A);

void menuEliminar(Agenda *A);

#endif // AGENDA_H_INCLUDED
