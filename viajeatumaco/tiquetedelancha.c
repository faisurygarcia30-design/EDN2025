#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pasajero {
    int documento;
    char destino[20];
    char tipo_pasaje[15];
    struct Pasajero* ida;
    struct Pasajero* regreso;
} Pasajero;

Pasajero *raiz = NULL;

void registrarPasajero();
Pasajero* crearPasajero();
void posicionar(Pasajero *actual, Pasajero *nuevo);

void listarInorden();
void inorden(Pasajero *nodo);

void listarPreorden();
void preorden(Pasajero *nodo);

void listarPostorden();
void postorden(Pasajero *nodo);

void contarPasajeros();
int contar(Pasajero *nodo);

void eliminarPasajero();
Pasajero* eliminar(Pasajero *nodo, int documento);
Pasajero* reemplazo(Pasajero *nodo);

int main(){
    int opc;
    do {
        printf("MENU PRINCIPAL\n");
        printf("MUELLE TURISTICO TUMACO TIMBIQUI JUANCHACO GUAPI\n");
        printf("1. Registrar Pasajero\n");
        printf("2. Listar Pasajeros (Inorden)\n");
        printf("3. Listar Pasajeros (Preorden)\n");
        printf("4. Listar Pasajeros (Postorden)\n");
        printf("5. Contar Total de Pasajeros\n");
        printf("6. Eliminar Pasajero\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opc);
        
        switch(opc) {
            case 1: registrarPasajero(); break;
            case 2: listarInorden(); break;
            case 3: listarPreorden(); break;
            case 4: listarPostorden(); break;
            case 5: contarPasajeros(); break;
            case 6: eliminarPasajero(); break;
            case 7: printf("Saliendo...\n"); break;
            default: printf("Opcion no valida!\n");
        }
        
    } while(opc != 7); 
    
    return 0;
}

void registrarPasajero(){
    Pasajero *nuevo = crearPasajero();

    if(raiz == NULL){
        raiz = nuevo;
    } else {
        posicionar(raiz, nuevo);
    }

    printf("\nPasajero registrado correctamente.\n");
}

Pasajero* crearPasajero(){
    Pasajero *nuevo = (Pasajero*) malloc(sizeof(Pasajero));

    printf("\nDigite el documento: ");
    scanf("%d", &nuevo->documento);

    printf("Digite el destino: ");
    scanf("%s", nuevo->destino);

    printf("Digite el tipo de pasaje: ");
    scanf("%s", nuevo->tipo_pasaje);

    nuevo->ida = nuevo->regreso = NULL;
    return nuevo;
}

void posicionar(Pasajero *actual, Pasajero *nuevo){
    if(nuevo->documento < actual->documento){
        if(actual->ida == NULL){
            actual->ida = nuevo;
        } else {
            posicionar(actual->ida, nuevo);
        }
    } else {
        if(actual->regreso == NULL){
            actual->regreso= nuevo;
        } else {
            posicionar(actual->regreso, nuevo);
        }
    }
}

void listarInorden(){
    printf("LISTADO INORDEN\n");
    inorden(raiz);
    printf("\n");
}

void inorden(Pasajero *nodo){
    if(nodo != NULL){
        inorden(nodo->ida);
        printf("%d - %s - %s\n", nodo->documento, nodo->destino, nodo->tipo_pasaje);
        inorden(nodo->regreso);
    }
}

void listarPreorden(){
    printf("LISTADO PREORDEN\n");
    preorden(raiz);
    printf("\n");
}

void preorden(Pasajero *nodo){
    if(nodo != NULL){
        printf("%d - %s - %s\n", nodo->documento, nodo->destino, nodo->tipo_pasaje);
        preorden(nodo->ida);
        preorden(nodo->regreso);
    }
}

void listarPostorden(){
    printf("LISTADO POSTORDEN\n");
    postorden(raiz);
    printf("\n");
}

void postorden(Pasajero *nodo){
    if(nodo != NULL){
        postorden(nodo->ida);
        postorden(nodo->regreso);
        printf("%d - %s - %s\n", nodo->documento, nodo->destino, nodo->tipo_pasaje);
    }
}

void contarPasajeros(){
    int total = contar(raiz);
    printf("\nTotal de pasajeros: %d\n", total);
}

int contar(Pasajero *nodo){
    if(nodo == NULL) return 0;
    return 1 + contar(nodo->ida) + contar(nodo->regreso);
}

void eliminarPasajero(){
    int doc;
    printf("\nDigite el documento a eliminar: ");
    scanf("%d", &doc);

    raiz = eliminar(raiz, doc);
}

Pasajero* eliminar(Pasajero *nodo, int documento){
    if(nodo == NULL){
        printf("No se encontro el pasajero.\n");
        return NULL;
    }

    if(documento < nodo->documento){
        nodo->ida = eliminar(nodo->ida, documento);

    } else if(documento > nodo->documento){
        nodo->regreso = eliminar(nodo->regreso, documento);

    } else {
        if(nodo->ida == NULL){
            Pasajero *aux = nodo->regreso;
            free(nodo);
            return aux;
        }
        else if(nodo->regreso == NULL){
            Pasajero *aux = nodo->ida;
            free(nodo);
            return aux;
        }
        else {
            Pasajero *aux = reemplazo(nodo->regreso);
            nodo->documento = aux->documento;
            strcpy(nodo->destino, aux->destino);
            strcpy(nodo->tipo_pasaje, aux->tipo_pasaje);
            nodo->regreso = eliminar(nodo->regreso, aux->documento);
        }
    }
    return nodo;
}

Pasajero* reemplazo(Pasajero *nodo){
    while(nodo->ida != NULL){
        nodo = nodo->ida;
    }
    return nodo;
}
