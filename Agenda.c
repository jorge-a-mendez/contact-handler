#include "Agenda.h"
#include "Pila.h"
#include <string.h>
//Jorge Mendez 10-10449
//Arturo D'amato 09-11275


Pila P=NULL;/*Se utiliza una pila para utilizar en las funciones de ver mensajes de un telefono y ver
llamadas de un telefono. La pila se creo con una clave apuntador a void, para usar en ambos casos.
La razon de esto es para almacenar alli los apuntadores a las llamadas/mensajes que ya se han visto.
Asi, si se ve una llamada y se va a la siguiente, la primera se almacena en la pila de llamadas, y
luego cuando se quiera ir a la anterior se vera el tope de la pla*/
ListaContactos L=NULL;/*Se crea una lista de contactos para la funcion de ver contactos. Ella contiene
a todos los contactos listados en orden, y se sera el orden que utilizara para imprimirlos en pantalla.
En la lista unicamente se encuentran los apuntadores a los contactos, para ahorrar memoria y poder eli-
minarlos con mayor facilidad del arbol en caso de que se seleccione la opcion de eliminar contacto.*/

//Inserta el contacto con telefono tel y nombre nom en el arbol de contactos de A
void insertarEnArb(int tel, char nom[20], Agenda *A){
   Contacto *p,*aux=A->con;
   int i=0;
   if((A->con)==NULL) {
       A->con=malloc(sizeof(Contacto));    //Si el arbol es vacio, inserta directamente
       if(A->con!=NULL){                   //el nodo con nombre nom y telefono tel al
          A->con->tel=tel;                 //principio del arbol.
          strcpy(A->con->nom,nom);
          while((A->con->nom)[i]!='\n' && A->con->nom[i]!='\0') i++;// Estas dos lineas eliminan el \n
          if((A->con->nom)[i]=='\n')(A->con->nom)[i]='\0';//al final del string nom
          A->con->der=NULL;
          A->con->izq=NULL;
          A->con->pad=NULL;
       }
   }
   else {
       while(aux!=NULL && aux->tel!=tel){
          p=aux;
          if(aux->tel>tel) aux=aux->izq;//Realiza la busqueda de la posicion donde
          else aux=aux->der;//deberia ir el contacto, si el arbol no es vacio
       }
       if(aux==NULL){
          aux=malloc(sizeof(Contacto));
          if(aux!=NULL){//Si no consigue el contacto, lo inserta en la posicion
            aux->tel=tel;//encontrada, y elimina luego el \n de su nombre.
            strcpy(aux->nom,nom);
            while((aux->nom)[i]!='\n' && aux->nom[i]!='\0') i++;
            if((aux->nom)[i]=='\n')(aux->nom)[i]='\0';
            aux->der=NULL;
            aux->izq=NULL;
            aux->pad=p;
            if(tel>p->tel) p->der=aux;
            else p->izq=aux;
          }
       }
   }
}

void leerContactos(char A_nom[], Agenda *A){
    char nomb[15];
    strcpy(nomb,A_nom);
    strcat(nomb,".con");
    FILE *p_con=fopen(nomb,"a+");
    A->con=NULL;
    int tel,aux_int;
    char nom[20];
    if(p_con!=NULL){
        fseek(p_con,0,SEEK_SET);
        while(!feof(p_con)){
            aux_int=fscanf(p_con,"%d ",&tel);//El entero aux_int es un centinela.
            fgets(nom,20,p_con);//En caso de haber error en la lectura, o que no
            if(aux_int>0) insertarEnArb(tel,nom,A);//se lea ningun elemento, no tratara de insertarlo al arbol.
        }
    fclose(p_con);
    }
}

void leerLlamadas(char A_nom[], Agenda *A){
    char nom[15];
    strcpy(nom,A_nom);
    strcat(nom,".lla");
    FILE *p_lla=fopen(nom,"a+");
    int aux_int,i=0;
    Llamada *a=malloc(sizeof(Llamada)),*ant=NULL;
    if(a!=NULL) A->lla=a;
    if(p_lla!=NULL){
        fseek(p_lla,0,SEEK_SET);
        while(!feof(p_lla)){
            if(a!=NULL){
                aux_int=fscanf(p_lla, "%d %d %d",&(a->tel), &(a->tip), &(a->dur));
                if(aux_int<=0) break;//De nuevo, si hay error en la lectura no inserta
                a->ant=ant;          //el elemento en la lista de llamadas
                ant=a;
                a->sig=malloc(sizeof(Llamada));
                a=a->sig;
                i++;
            }
        }
        if(a!=NULL){
            if(i>0)ant->sig=NULL;
            else A->lla=NULL;
            free(a);
        }
    }
    fclose(p_lla);
}

void leerMsjs(char A_nom[], Agenda *A){
    char nom[15];
    strcpy(nom,A_nom);
    strcat(nom,".men");
    FILE *p_men=fopen(nom, "a+");
    int i=0,aux_int;
    Mensaje *a=malloc(sizeof(Mensaje)), *ant=NULL;
    if(a!=NULL) A->men=a;
    if(p_men!=NULL){
        fseek(p_men,0,SEEK_SET);
        while(!feof(p_men)){
            if(a!=NULL){
                aux_int=fscanf(p_men,"%d %d %d ", &(a->tel), &(a->tip), &(a->leido));
                fgets(a->tex,40,p_men);
                if(aux_int<0) break;//Utiliza aux_int como un centinela de nuevo.
                a->ant=ant;
                ant=a;
                a->sig=malloc(sizeof(Mensaje));
                a=a->sig;
                i++;
            }
        }
        if(a!=NULL){
            if(i!=0)ant->sig=NULL;
            else A->men=NULL;
            free(a);
        }
    }
    fclose(p_men);
}

Agenda selectAgenda(){
    Agenda A;
    printf("\nIntroduzca el nombre de la agenda (maximo 10 caracteres): ");
    fflush(stdin);
    gets(A.nom);
    leerContactos(A.nom,&A);
    leerLlamadas(A.nom,&A);
    leerMsjs(A.nom,&A);
    return A;
}

void agregarLlamada(Agenda *A){
    Llamada *L=malloc(sizeof(Llamada)),*aux=A->lla;
    if(L!=NULL){
        A->lla=L;
        L->ant=NULL;
        L->sig=aux;
        if (aux!=NULL) aux->ant=L;
        do{
            printf("\nIntroduzca el numero de telefono (7 digitos): ");
            scanf("%d",&(L->tel));
        }while((L->tel)<1000000 || (L->tel)>9999999);//Si el telefono no es de 7 digitos, lo pide de nuevo.
        do{
            printf("\nIntroduzca:\n0 para llamada recibida\n1 para llamada saliente\n2 para llamada perdida \n");
            scanf("%d",&(L->tip));
        }while ((L->tip)!=0 && (L->tip)!=1 && (L->tip)!=2);
        if(L->tip==2) L->dur=0;
        else{
            do{
                printf("\nIntroduzca la duracion, en minutos, de la llamada: ");
                scanf("%d", &(L->dur));
            }while(L->dur<0);
        }
        printf("\n");
    }
    else printf("No hay memoria para mas llamadas\n");
}

void agregarMsj(Agenda*A){
    Mensaje *M=malloc(sizeof(Mensaje)), *aux=A->men;
    if(M!=NULL){
        A->men=M;
        M->ant=NULL;
        M->sig=aux;
        if(aux!=NULL)aux->ant=M;
        do{
            printf("\nIntroduzca el numero de telefono (7 digitos): ");
            scanf("%d",&(M->tel));
        }while((M->tel)<1000000 || (M->tel)>9999999);
        do{
            printf("\nIntroduzca 0 para recibido y 1 para enviado: ");
            scanf("%d",&(M->tip));
        }while ((M->tip)!=0 && (M->tip)!=1);
        if(M->tip==1) M->leido=1;
        else {
            do{
            printf("\nIntroduzca 1 para leido y 0 para no leido: ");
            scanf("%d", &(M->leido));
            }while((M->leido)!=1 && (M->leido)!=0);
        }
        printf("\nIntroduzca el texto del mensaje: ");
        fflush(stdin);
        fgets(M->tex,40,stdin);
        printf("\n");
    }
    else printf("No hay memoria para mas mensajes\n");
}

void agregarContacto(Agenda *A){
    int tel;
    char nom[20];
    do{
        printf("\nIntroduzca el numero de telefono (7 digitos): ");
        scanf("%d",&tel);
    }while (tel<1000000 || tel>9999999);
    printf("\nIntroduzca el nombre del contacto: ");
    fflush(stdin);
    fgets(nom,20,stdin);
    printf("\n");
    insertarEnArb(tel,nom,A);
}

void eliminaLlamada(Llamada **L, Agenda *A){
    Llamada *aux=*L;
    if(*L!=NULL){
        if((*L)->sig!=NULL) aux->sig->ant=aux->ant;
        if((*L)->ant!=NULL) aux->ant->sig=aux->sig;
        else A->lla=aux->sig;
        *L=aux->sig;
        free(aux);
    }
}

//Devuelve 0 si no consigue el contacto. Si lo consigue, devuelve 1 y copia su nombre en nom.
int nombreContacto(Agenda A, char nom[20], int tel){
    Contacto *aux=A.con;
    while(aux!=NULL && aux->tel!=tel){
        if(tel>aux->tel) aux=aux->der;
        else aux=aux->izq;
    }
    if(aux!=NULL) {
        strcpy(nom,aux->nom);
        return 1;
    }
    else return 0;
}

void verLlamadas(Llamada **L,Agenda *A){
    if(*L!=NULL){
        char tipo[11], contacto[20];
        int op;
        Llamada *ant;
        if(nombreContacto(*A,contacto,(*L)->tel)==0) strcpy(contacto,"Desconocido");
        if ((*L)->tip==0) strcpy(tipo,"(recibida)");
        else if((*L)->tip==1) strcpy(tipo,"(saliente)");
        else strcpy(tipo,"(perdida)");
        if((*L)->tip!=2)printf("%d (%s) %s duracion: %d min\n\n",(*L)->tel,contacto,tipo, (*L)->dur);
        else printf("%d (%s) %s\n\n",(*L)->tel,contacto,tipo);
        printf("\n\n\t\tMenu Llamada\n\n\t1) Eliminar llamada\n\t2) Ver siguiente llamada\n\t3) Ver llamada anterior\n\t0) Volver al menu anterior\n\nIntroduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                eliminaLlamada(L,A);
                verLlamadas(L,A);
                break;
            case 2:
                verLlamadas(&((*L)->sig),A);
                break;
            case 3:
                ant=(*L)->ant;
                verLlamadas(&ant,A);
                break;
            case 0: break;
            default:
                printf("Opcion no valida, vuelva a intentar\n");
                verLlamadas(L,A);
        }
    }
    else printf("\n\n\t\tLa llamada que usted quiere ver no existe\n\n");
}

void eliminaMsj(Mensaje **M,Agenda *A){
    Mensaje *aux=*M;
    if(*M!=NULL){
        if((*M)->sig!=NULL) (*M)->sig->ant=(*M)->ant;
        if((*M)->ant!=NULL) (*M)->ant->sig=(*M)->sig;
        else A->men=aux->sig;
        *M=aux->sig;
        free(aux);
    }
}

void respMsj(Agenda *A, int tel){
    Mensaje *M=malloc(sizeof(Mensaje)), *aux=A->men;
    if(M!=NULL){
        A->men=M;
        M->ant=NULL;
        M->sig=aux;
        aux->ant=M;
        M->tel=tel;
        M->tip=1;
        M->leido=1;//Todos los mensajes enviados, se consideran leidos.
        printf("Introduzca el texto del mensaje: ");
        fflush(stdin);
        fgets(M->tex,40,stdin);
        printf("\n");
    }
    else printf("No hay memoria para mas mensajes\n");
}

void verMsjs(Mensaje **M, Agenda *A){
    if(*M!=NULL){
        char tipo[11], leido[11],contacto[20];
        int op;
        Mensaje *ant;
        if ((*M)->tip==0) strcpy(tipo,"(recibido)");
        else strcpy(tipo,"(enviado)");
        if ((*M)->leido==0) strcpy(leido,"(no leido)");
        else strcpy(leido,"(leido)");
        if(nombreContacto(*A,contacto,(*M)->tel)==0) strcpy(contacto,"Desconocido");
        if((*M)->tip==0)printf("%d (%s) %s %s %s\n\n",(*M)->tel,contacto, tipo,leido,(*M)->tex);
        else printf("%d (%s) %s %s\n\n",(*M)->tel,contacto, tipo,(*M)->tex);
        (*M)->leido=1;
        printf("\n\n\t\tMenu Mensaje\n\n\t1) Eliminar mensaje\n\t2) Ver siguiente mensaje\n\t3) Ver mensaje anterior\n\t4) Responder mensaje\n\t0) Volver al menu anterior\n\nIntroduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                eliminaMsj(M,A);
                verMsjs(M,A);
                break;
            case 2:
                verMsjs(&((*M)->sig),A);
                break;
            case 3:
                ant=(*M)->ant;
                verMsjs(&ant,A);
                break;
            case 4:
                respMsj(A,(*M)->tel);
                if((*M)->ant!=NULL)verMsjs(M,A);
                else verMsjs(&((*M)->sig),A);
                break;
            case 0: break;
            default:
                printf("Opcion no valida, vuelva a intentar\n");
                verMsjs(M,A);
        }
    }
    else printf("\n\n\t\tEl mensaje que usted quiere ver no existe\n\n");
}

void verMsjsNoLeidos(Mensaje **M, Agenda *A){
    Mensaje *aux=*M;
    while (aux!=NULL && aux->leido!=0) aux=aux->sig;
    if (aux!=NULL){
        int op;
        char contacto[20];
        if(nombreContacto(*A,contacto,(*M)->tel)==0) strcpy(contacto,"Desconocido");
        printf("%d (%s) %s\n\n",aux->tel,contacto,aux->tex);
        aux->leido=1;//Una vez que se lee el mensaje, lo coloca como mensaje leido.
        printf("\n\n\t\tMenu Mensaje\n\n\t1) Eliminar mensaje\n\t2) Ver siguiente mensaje\n\t3) Responder mensaje\n\t0) Volver al menu anterior\n\nIntroduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                eliminaMsj(&aux,A);
                verMsjsNoLeidos(&aux,A);
                break;
            case 2:
                verMsjsNoLeidos(&(aux->sig),A);
                break;
            case 3:
                respMsj(A,aux->tel);
                verMsjsNoLeidos(&aux,A);
            case 0: break;
            default:
                printf("Opcion no valida, vuelva a intentar\n");
                verMsjsNoLeidos(&aux,A);
        }
    }
    else printf("\n\n\t\tEl mensaje que usted quiere ver no existe\n\n");
}

void eliminaContList(ListaContactos *L, int a){
    ContList *aux=*L;
    if(a==1) *L=(*L)->sig;
    else *L=(*L)->ant;
    if(aux->ant!=NULL) aux->ant->sig=aux->sig;
    if(aux->sig!=NULL) aux->sig->ant=aux->ant;
    free(aux);//Elimina un nodo de la lista de contactos.
}

void eliminaContacto(ListaContactos *L, Agenda *A, int a){
    Contacto *C=(*L)->con;
    Contacto *fr,*aux,*min;
    int tel=C->tel;
    fr=C;
    aux=C->pad;
    if(C->izq ==NULL && C->der==NULL){
        if(C->pad!=NULL){
            if(C->pad->tel>tel) C->pad->izq=NULL;
            else C->pad->der=NULL;
        }
        else A->con=NULL;
        eliminaContList(L,a);
        free(fr);
    }
    else if(C->izq==NULL){
        if(C->pad!=NULL){
            if(C->pad->tel>tel) C->pad->izq=C->der;
            else C->pad->der=C->der;
        }
        else A->con=C->der;
        C=C->der;
        eliminaContList(L,a);
        free(fr);
        if(C!=NULL) C->pad=aux;
    }
    else if(C->der==NULL){
        if(C->pad!=NULL){
            if(C->pad->tel>tel) C->pad->izq=C->izq;
            else C->pad->der=C->der;
        }
        else A->con=C->izq;
        C=C->izq;
        eliminaContList(L,a);
        free(fr);
        if(C!=NULL)C->pad=aux;
    }
    else{
        min=((*L)->sig->con);//En caso de que el nodo a eliminar tenga dos hijos, se cambia su
        tel=C->tel;//informacion con la de su sucesor (el menor de su sub arbol derecho), y se
        strcpy(C->nom,min->nom);//elimina el sucesor. En la lista, el sucesor sera el siguiente
        C->tel=min->tel;
        if(min==C->der) min->tel=10000000;//Centinela que asegura que al eliminar, el telefono
        else min->tel=999999;//sea mayor que el del padre si es su hijo derecho, y menor si es el izquierdo.
        eliminaContacto(&((*L)->sig),A,0);
    }
}

//Inserta el apuntador a C al inicio de la lista de contactos. Se hace asi para hacerlo de manera eficiente.
void ConsContacto (ListaContactos *L,Contacto *C){
    ContList *aux=malloc(sizeof(ContList));
    if(aux!=NULL){
        aux->con=C;
        aux->ant=NULL;
        if(*L!=NULL){
            aux->sig=*L;
            aux->sig->ant=aux;
        }
        else aux->sig=NULL;
        *L=aux;
    }
}

//Pre: El arbol de contactos es no vacio. Post: enlista todos los apuntadores a los contactos en Orden
void listarContactos(Contacto *C){
    if(C!=NULL){
        listarContactos(C->der);//Se hace la llamada a partir del mayor elemento, para que lo inserte al principio
        ConsContacto(&L,C);//y haga lo mismo para los anteriores, quedando perfectamente ordenada la lista.
        listarContactos(C->izq);
    }
}

//Libera todos los espacios de memoria de la lista de contactos.
void destruirListaContactos(ListaContactos *L){
    ContList *aux,*a;
    ListaContactos laux;
    a=*L;
    while(a!=NULL){
        aux=a;
        a=a->sig;
        free(aux);
    }
    laux=*L;
    *L=NULL;
    free(laux);
}

void verContactos(ListaContactos *L, Agenda *A){
    if(*L!=NULL){
        int op;
        ListaContactos ant;
        printf("\n\n%s: %d\n\n",((*L)->con)->nom,((*L)->con)->tel);
        printf("\n\n\t\tMenu Contacto\n\n\t1) Eliminar Contacto\n\t2) Ver Siguiente Contacto\n\t3) Ver Contacto Anterior\n\t0) Volver al Menu Anterior\n\nIntroduzca su Opcion: ");
        fflush(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                eliminaContacto(L,A,1);
                verContactos(L,A);
                break;
            case 2:
                verContactos(&((*L)->sig),A);
                break;
            case 3:
                ant=(*L)->ant;
                verContactos(&ant,A);
                break;
            case 0:
                break;
            default:
                printf("Opcion no valida, introduzca una de las opciones mostradas\n");
                verContactos(L,A);
        }
    }
    else printf("\n\nNo existe el contacto que usted quiere ver");

}

void verMsjsTlf(Mensaje **M, Agenda *A, int tel, char contacto[20]){
    Mensaje *aux=*M,*ant;
    while (aux!=NULL && aux->tel!=tel) aux=aux->sig;
    if (aux!=NULL){
        int op;
        char tipo[11], leido[11];
        if(aux->tip==0) strcpy(tipo,"(recibido)");
        else strcpy(tipo,"(enviado)");
        if(aux->leido==0) strcpy(leido,"(no leido)");
        else strcpy(leido,"(leido)");
        if(aux->tip==0)printf("(%s) %s %s %s\n\n",contacto,tipo,leido,aux->tex);
        else printf("(%s) %s %s\n\n",contacto,tipo, aux->tex);
        aux->leido=1;
        printf("\n\n\t\tMenu Mensaje\n\n\t1) Eliminar mensaje\n\t2) Ver siguiente mensaje\n\t3) Ver mensaje anterior\n\t4) Responder mensaje\n\t0) Volver al menu anterior\n\nIntroduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                eliminaMsj(&aux,A);
                verMsjsTlf(&aux,A,tel,contacto);
                break;
            case 2:
                push(aux,&P);
                verMsjsTlf(&(aux->sig),A,tel,contacto);
                break;
            case 3:
                if(esVacia(P))ant=NULL;
                else {
                    ant=top(P);
                    pop(&P);
                }
                verMsjsTlf(&ant,A,tel,contacto);
                break;
            case 4:
                respMsj(A,aux->tel);
                tramposo(A->men,&P);
                verMsjsTlf(&aux,A,tel,contacto);
                break;
            case 0: break;
            default:
                printf("Opcion no valida, vuelva a intentar\n");
                verMsjsTlf(&aux,A,tel,contacto);
        }
    }
    else {
        printf("\n\n\t\tEl mensaje que usted quiere ver no existe\n\n");
        destruirPila(&P);
    }
}

void verLlamadasTlf(Llamada **L, int tel,char contacto[20],Agenda *A){
    Llamada *aux=*L, *ant;
    while (aux!=NULL && aux->tel!=tel) aux=aux->sig;
    if (aux!=NULL){
        int op;
        char tipo[11];
        if(aux->tip==0) strcpy(tipo,"(recibida)");
        else if(aux->tip==1)strcpy(tipo,"(saliente)");
        else strcpy(tipo,"(perdida)");
        if(aux->tip!=2)printf("(%s) %s duracion: %d min\n\n",contacto,tipo,aux->dur);
        else printf("(%s) %s\n\n",contacto,tipo);
        printf("\n\n\t\tMenu Llamada\n\n\t1) Eliminar llamada\n\t2) Ver siguiente llamada\n\t3) Ver llamada anterior\n\t0) Volver al menu anterior\n\nIntroduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                eliminaLlamada(&aux,A);
                verLlamadasTlf(&aux,tel,contacto,A);
                break;
            case 2:
                push(aux,&P);
                verLlamadasTlf(&(aux->sig),tel,contacto,A);
                break;
            case 3:
                if(esVacia(P)) ant=NULL;
                else{
                    ant=top(P);
                    pop(&P);
                }
                verLlamadasTlf(&ant,tel,contacto,A);
            case 0: break;
            default:
                printf("Opcion no valida, vuelva a intentar\n");
                verLlamadasTlf(&aux,tel,contacto,A);
        }
    }
    else {
        printf("\n\n\t\tLa llamada que usted quiere ver no existe\n\n");
        destruirPila(&P);
    }
}

//Hace un clon de la lista de llamadas
Llamada *clon(Llamada *L){
    Llamada *m=malloc(sizeof(Llamada)), *a=L,*ant=NULL,*ret=m;
    if (L!=NULL){
        while(a->sig!=NULL){
            if(m!=NULL){
                m->tel=a->tel;
                m->tip=a->tip;
                m->dur=a->dur;
                m->ant=ant;
                m->sig=malloc(sizeof(Llamada));
                a=a->sig;
                ant=m;
                m=m->sig;
            }
        }
        if(m!=NULL){
            m->sig=NULL;
            m->ant=ant;
            m->tel=a->tel;
            m->tip=a->tip;
            m->dur=a->dur;
        }
    }
    else {
        if(m!=NULL) free(m);
        return NULL;
    }
    return ret;
}

//Destruye una lista de llamadas
void destruirLla(Llamada **L){
    Llamada *aux,*a;
    Llamada **laux;
    a=*L;
    while(a!=NULL){
        aux=a;
        a=a->sig;
        free(aux);
    }
    laux=L;
    *L=NULL;
    free(laux);
}

void listarLlamadas(Agenda A){//Esta funcion crea un clon de la lista de llamadas, y luego toma dicho clon y toma el telefono
    Llamada *L=clon(A.lla),*aux;//de la primera llamada y busca todas las llamadas asociadas a el, eliminandolas del clon.
    int tel,rec=0,sal=0,per=0;//Luego, repite el proceso para las llamadas restantes, hasta quedar vacio el clon.
    char contacto[20];
    printf("\n");
    while(L!=NULL){
        tel=L->tel;
        while(L!=NULL && L->tel==tel){
            if(L->tip==0) rec++;
            else if(L->tip==1) sal++;
            else per++;
            eliminaLlamada(&L,&A);
        }
        aux=L;
        while(aux!=NULL){
            if(aux->tel==tel){
                if(aux->tip==0) rec++;
                else if(aux->tip==1) sal++;
                else per++;
                eliminaLlamada(&aux,&A);
            }
            else aux=aux->sig;
        }
        if(nombreContacto(A,contacto,tel)==0) strcpy(contacto,"Desconocido");
        printf("%d (%s)\n",tel,contacto);
        if(rec!=0) printf("\t(recibidas) (%d)\n",rec);
        if(sal!=0) printf("\t(salientes) (%d)\n",sal);
        if(per!=0) printf("\t(perdidas) (%d)\n",per);
        printf("\n");
        rec=0;sal=0;per=0;
    }
    printf("\n");
}

int tiempLlamHech(Agenda A){
    int tiemp=0;
    Llamada *aux=A.lla;
    while(aux!=NULL){
        if(aux->tip==1) tiemp+=aux->dur;
        aux=aux->sig;
    }
    return tiemp;
}

int tiempLlamRec(Agenda A){
    int tiemp=0;
    Llamada *aux=A.lla;
    while(aux!=NULL){
        if(aux->tip==0) tiemp+=aux->dur;
        aux=aux->sig;
    }
    return tiemp;
}

int tiempLlamTot(Agenda A){
    int tiemp=0;
    Llamada *aux=A.lla;
    while(aux!=NULL){
        tiemp+=aux->dur;
        aux=aux->sig;
    }
    return tiemp;
}

int tiempLlamHechTlf(Agenda A, int tel){
    int tiemp=0;
    Llamada *aux=A.lla;
    while(aux!=NULL){
        if(aux->tel==tel && aux->tip==1) tiemp+=aux->dur;
        aux=aux->sig;
    }
    return tiemp;
}

int tiempLlamRecTlf(Agenda A, int tel){
    int tiemp=0;
    Llamada *aux=A.lla;
    while(aux!=NULL){
        if(aux->tel==tel && aux->tip==0) tiemp+=aux->dur;
        aux=aux->sig;
    }
    return tiemp;
}

int tiempLlamTotTlf(Agenda A, int tel){
    int tiemp=0;
    Llamada *aux=A.lla;
    while(aux!=NULL){
        if(aux->tel==tel) tiemp+=aux->dur;
        aux=aux->sig;
    }
    return tiemp;
}

void eliminaLlamHech(Agenda *A, int tel){
    Llamada *L=A->lla;
    while(L!=NULL && (L->tel==tel) && (L->tip==1)){
        eliminaLlamada(&L,A);
        A->lla=L;
    }
    while(L!=NULL){
        if((L->tel==tel) && (L->tip==1)) eliminaLlamada(&L,A);
        else L=L->sig;
    }
}

void eliminaLlamRec(Agenda *A, int tel){
    Llamada *L=A->lla;
    while(L!=NULL && (L->tel==tel) && (L->tip!=1)){
        eliminaLlamada(&L,A);
        A->lla=L;
    }
    while(L!=NULL){
        if((L->tel==tel) && (L->tip!=1)) eliminaLlamada(&L,A);
        else L=L->sig;
    }
}

void eliminaLlamTot(Agenda *A, int tel){
    Llamada *L=A->lla;
    while(L!=NULL && (L->tel==tel)){
        eliminaLlamada(&L,A);
        A->lla=L;
    }
    while(L!=NULL){
        if(L->tel==tel) eliminaLlamada(&L,A);
        else L=L->sig;
    }
}

void eliminaMsjsEnv(Agenda *A,int tel){
    Mensaje *M=A->men;
    while(M!=NULL && (M->tel==tel) && (M->tip==1)){
        eliminaMsj(&M,A);
        A->men=M;
    }
    while(M!=NULL){
        if((M->tel==tel) && (M->tip==1)) eliminaMsj(&M,A);
        else M=M->sig;
    }
}

void eliminaMsjsRec(Agenda *A,int tel){
    Mensaje *M=A->men;
    while(M!=NULL && (M->tel==tel) && (M->tip==0)){
        eliminaMsj(&M,A);
        A->men=M;
    }
    while(M!=NULL){
        if((M->tel==tel) && (M->tip==0)) eliminaMsj(&M,A);
        else M=M->sig;
    }
}

void eliminaMsjsTot(Agenda *A, int tel){
    Mensaje *M=A->men;
    while(M!=NULL && (M->tel==tel)){
        eliminaMsj(&M,A);
        A->men=M;
    }
    while(M!=NULL){
        if(M->tel==tel) eliminaMsj(&M,A);
        else M=M->sig;
    }
}

//Imprime en el archivo p_con los contactos en preorden (padre, hijo izquierdo, hijo derecho)
void preOrden(FILE *p_con, Contacto *C){
    if(C!=NULL){
        fprintf(p_con,"%d %s\n",C->tel, C->nom);
        preOrden(p_con,C->izq);
        preOrden(p_con,C->der);
    }
}

void salvarAgenda(char nom[],Agenda A){//Imprime en los archivos correspondientes todos los mensajes, llamadas y contactos.
    FILE *p_lla,*p_men,*p_con;
    char nom_lla[15],nom_men[15],nom_con[15];
    strcpy(nom_lla,nom); strcat(nom_lla,".lla");
    strcpy(nom_men,nom); strcat(nom_men,".men");
    strcpy(nom_con,nom); strcat(nom_con,".con");
    Llamada *lla=A.lla;
    Mensaje *men=A.men;
    Contacto *con=A.con;
    p_lla=fopen(nom_lla,"w");
    if(p_lla!=NULL){
        if(lla!=NULL){
            while(lla->sig!=NULL){
                fprintf(p_lla,"%d %d %d\n",lla->tel,lla->tip,lla->dur);
                lla=lla->sig;
            }
            fprintf(p_lla,"%d %d %d",lla->tel,lla->tip,lla->dur);
        }
    }
    p_men=fopen(nom_men,"w");
    if(p_men!=NULL){
        while(men!=NULL){
        fprintf(p_men,"%d %d %d %s",men->tel, men->tip,men->leido,men->tex);
        men=men->sig;
        }
    }
    p_con=fopen(nom_con,"w");
    if(p_con!=NULL) preOrden(p_con,con);
}

void menuVer(Agenda *A){
    int op;
    char contacto[10];
    int tel;
    printf("\n\n\t\tMenu Ver...\n\n\t1) Ver Llamadas\n\t2) Ver Mensajes\n\t3) Ver Mensajes No Leidos\n\t4) Ver Contactos\n\t5) Ver Mensajes de un Telefono\n\t6) Ver Llamadas de Un telefono\n");
    printf("\t7) Listar Todas las Llamadas\n\t8) Ver Tiempo de Llamadas Hechas\n\t9) Ver Tiempo de Llamadas Recibidas\n\t10) Ver Tiempo de Llamadas Total\n\t11) Ver Tiempo de Llamadas Hechas ");
    printf("a un Telefono\n\t12) Ver Tiempo de Llamadas Recibidas de Un Telefono\n\t13) Ver Tiempo de Llamadas Totales a un Telefono\n\t0) Volver al Menu Anterior\n\n");
    do{
        printf("Introduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
    }while(op<0 || op>13);
    switch(op){
        case 1:
            verLlamadas(&(A->lla),A);
            menuVer(A);
            break;
        case 2:
            verMsjs(&(A->men),A);
            menuVer(A);
            break;
        case 3:
            verMsjsNoLeidos(&(A->men),A);
            menuVer(A);
            break;
        case 4:
            listarContactos(A->con);
            verContactos(&L,A);
            destruirListaContactos(&L);
            menuVer(A);
            break;
        case 5:
            do{
                printf("\nIntroduzca el numero de telefono cuyos mensajes quiere ver (7 digitos): ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            if(nombreContacto(*A,contacto,tel)==0) strcpy(contacto,"Desconocido");
            verMsjsTlf(&(A->men),A,tel,contacto);
            menuVer(A);
            break;
        case 6:
            do{
                printf("\nIntroduzca el numero de telefono cuyas llamadas quiere ver (7 digitos): ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            if(nombreContacto(*A,contacto,tel)==0) strcpy(contacto,"Desconocido");
            verLlamadasTlf(&(A->lla),tel,contacto,A);
            menuVer(A);
            break;
        case 7:
            listarLlamadas(*A);
            menuVer(A);
            break;
        case 8:
            printf("\n\n\t\t\tDuracion de llamadas realizadas: %d min\n\n",tiempLlamHech(*A));
            menuVer(A);
            break;
        case 9:
            printf("\n\n\t\t\tDuracion de llamadas recibidas: %d min\n\n",tiempLlamRec(*A));
            menuVer(A);
            break;
        case 10:
            printf("\n\n\t\t\tDuracion de llamadas total: %d min\n\n", tiempLlamTot(*A));
            menuVer(A);
            break;
        case 11:
            do{
                printf("\nIntroduzca el numero de telefono (7 digitos): ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            if(nombreContacto(*A,contacto,tel)==0) strcpy(contacto,"Desconocido");
            printf("\n\n\tDuracion de llamadas realizadas a %d (%s): %d min\n\n",tel,contacto,tiempLlamHechTlf(*A,tel));
            menuVer(A);
            break;
        case 12:
            do{
                printf("\nIntroduzca el numero de telefono (7 digitos): ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            if(nombreContacto(*A,contacto,tel)==0) strcpy(contacto,"Desconocido");
            printf("\n\n\tDuracion de llamadas recibidas de %d (%s): %d min\n\n",tel,contacto,tiempLlamRecTlf(*A,tel));
            menuVer(A);
            break;
        case 13:
            do{
                printf("\nIntroduzca el numero de telefono (7 digitos): ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            if(nombreContacto(*A,contacto,tel)==0) strcpy(contacto,"Desconocido");
            printf("\n\n\tDuracion de llamadas totales a %d (%s): %d min\n\n",tel,contacto,tiempLlamTotTlf(*A,tel));
            menuVer(A);
            break;
        case 0:
            break;
        default:
            printf("Opcion no valida, vuelva a intentar\n");
            menuVer(A);
    }
}

void menuEliminar(Agenda *A){
    int op, tel;
    printf("\n\n\t\tMenu Eliminar de un Telefono...\n\n\t1) Eliminar Llamadas Hechas a El\n\t2) Eliminar Llamadas Recibidas de El\n\t3) Eliminar Todas las Llamadas Asociadas a El\n");
    printf("\t4) Eliminar Mensajes Enviados a El\n\t5) Eliminar Mensajes Recibidos de El\n\t6) Eliminar Todos los Mensajes Asociados a El\n\t0) Volver al Menu Anterior\n\n");
    do{
        printf("Introduzca su opcion: ");
        fflush(stdin);
        scanf("%d",&op);
    }while(op<0 || op>6);
    switch(op){
        case 1:
            do{
                printf("Introduzca el numero de telefono: ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            eliminaLlamHech(A,tel);
            menuEliminar(A);
            break;
        case 2:
            do{
                printf("Introduzca el numero de telefono: ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            eliminaLlamRec(A,tel);
            menuEliminar(A);
            break;
        case 3:
            do{
                printf("Introduzca el numero de telefono: ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            eliminaLlamTot(A,tel);
            menuEliminar(A);
            break;
        case 4:
            do{
                printf("Introduzca el numero de telefono: ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            eliminaMsjsEnv(A,tel);
            menuEliminar(A);
            break;
        case 5:
            do{
                printf("Introduzca el numero de telefono: ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            eliminaMsjsRec(A,tel);
            menuEliminar(A);
            break;
        case 6:
            do{
                printf("Introduzca el numero de telefono: ");
                scanf("%d",&tel);
            }while(tel<1000000 || tel>9999999);
            eliminaMsjsTot(A,tel);
            menuEliminar(A);
            break;
        case 0:
            break;
        default:
            printf("Opcion no valida, vuelva a intentar\n");
            menuEliminar(A);
    }
}

