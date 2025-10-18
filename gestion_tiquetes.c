#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Pasajero {
    char apellido[30];
    char genero[15];
    int abordado; // 0: No abordado, 1: Abordado
    struct Pasajero *next;
} Pasajero;


void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    
    int capacidad = 0; 
    int max_tiquetes = 0; 
    int abordaje_iniciado = 0;
    
    // CORRECCIÓN: Se agrega la variable 'vendidos'
    int vendidos = 0; 

    Pasajero *inicio = NULL, *fin = NULL, *temp;
    int opcion;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Establecer capacidad\n");
        printf("2. Vender tiquete\n");
        printf("3. Iniciar abordaje\n");
        printf("4. Ver abordados\n");
        printf("5. Ver no abordados\n");
        printf("6. Salir\n");
        printf("Opcion: ");
        
        
        if (scanf("%d", &opcion) != 1) {
            limpiar_buffer();
            opcion = 0; 
            continue;
        }
        limpiar_buffer(); 
        if (opcion == 1) {
            if (capacidad == 0) {
                printf("Ingrese la capacidad del avion: ");
                scanf("%d", &capacidad);
                limpiar_buffer();
                
                
                max_tiquetes = capacidad + (capacidad / 10); 
                printf("Capacidad establecida: %d (+10%% sobreventa: %d total)\n", capacidad, max_tiquetes);
            } else {
                printf("La capacidad ya fue establecida.\n");
            }
        }

        else if (opcion == 2) {
            if (abordaje_iniciado) {
                printf("ERROR: No se pueden vender tiquetes despues de iniciar abordaje.\n");
                continue;
            }
            if (capacidad == 0) {
                printf("ERROR: Debe establecer la capacidad primero (Opcion 1).\n");
                continue;
            }
            if (vendidos >= max_tiquetes) {
                printf("ATENCION: Se alcanzo el limite de sobreventa (%d tiquetes).\n", max_tiquetes);
                continue;
            }

            Pasajero *nuevo = (Pasajero*)malloc(sizeof(Pasajero)); 
            
            if (nuevo == NULL) {
                printf("ERROR: No hay suficiente memoria.\n");
                continue;
            }

            printf("Ingrese primer apellido: ");
            scanf("%s", nuevo->apellido);
            limpiar_buffer(); 
            int g;
            printf("Genero (1.Femenino 2.Masculino 3.No binario): ");
            scanf("%d", &g);
            limpiar_buffer();

            if (g == 1) strcpy(nuevo->genero, "Femenino");
            else if (g == 2) strcpy(nuevo->genero, "Masculino");
            else strcpy(nuevo->genero, "No binario");
            
            nuevo->abordado = 0; 
            nuevo->next = NULL;

            if (inicio == NULL) {
                inicio = nuevo;
                fin = nuevo; // Asegurar que 'fin' apunte al primer nodo
            } else {
                fin->next = nuevo;
                fin = nuevo; // Mover 'fin' al nuevo nodo
            }
            vendidos++;
            printf("Tiquete vendido (#%d)\n", vendidos);
        }

        else if (opcion == 3) {
            if (abordaje_iniciado) {
                printf("El abordaje ya fue iniciado.\n");
                continue;
            }
            if (vendidos == 0) {
                printf("No hay pasajeros para abordar.\n");
                continue;
            }

            abordaje_iniciado = 1; 
            printf("\nIniciando abordaje...\n");
            
            int count = 0;
            for (temp = inicio; temp != NULL && count < capacidad; temp = temp->next) {
                temp->abordado = 1;
                count++;
            }
            printf("Abordaron %d pasajeros (Capacidad Maxima del Avion).\n", count);
        }
        else if (opcion == 4) {
            printf("\n--- Pasajeros Abordados ---\n");
            int i = 1;
            for (temp = inicio; temp != NULL; temp = temp->next) {
                if (temp->abordado) {
                    printf("%d. %s (%s)\n", i++, temp->apellido, temp->genero);
                }
            }
        }

        else if (opcion == 5) {
            printf("\n--- Pasajeros No Abordados ---\n");
            int i = 1;
            for (temp = inicio; temp != NULL; temp = temp->next) {
                if (!temp->abordado) {
                    printf("%d. %s (%s)\n", i++, temp->apellido, temp->genero);
                }
            }
        }
        
        else if (opcion == 6) {
             printf("Saliendo del programa...\n");
        }
        
        else {
            printf("Opcion invalida. Intente de nuevo.\n");
        }

    } while (opcion != 6);

    // Liberación de memoria dinámica
    while (inicio != NULL) {
        temp = inicio;
        inicio = inicio->next;
        free(temp); 
    }

    return 0;
}