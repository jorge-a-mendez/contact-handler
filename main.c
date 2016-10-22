#include "Agenda.h"
//Jorge Mendez 10-10449
//Arturo D'amato 09-11275


int main()
{
   int op;
   Agenda A;
      printf("\t\t\t\tAGENDA\n\n");
      printf("Seleccionar Agenda:\n");
      A=selectAgenda();
   do{
      printf("\n\n\t\tMenu Programa Agenda \n\n");
      printf("\t1) Agregar Llamada\n");
      printf("\t2) Agregar Mensaje\n");
      printf("\t3) Agregar Contacto\n");
      printf("\t4) Ver...\n");
      printf("\t5) Eliminar de un telefono...\n");
      printf("\t0) Salir y guardar\n\n");
      printf("Introduzca su opcion: ");
      scanf("%d", &op);
      switch(op) {
         case 1:
            agregarLlamada(&A);
            break;
         case 2:
            agregarMsj(&A);
            break;
        case 3:
            agregarContacto(&A);
            break;
        case 4:
            menuVer(&A);
            break;
        case 5:
            menuEliminar(&A);
         case 0:
            salvarAgenda(A.nom,A);
            break;
         default:
            printf("\nOpcion no valida. Introduzca una de las opciones mostradas:\n\n");
      }
    }while (op != 0);
    return 0;
}
