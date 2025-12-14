#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

//1. Estructura del Nodo de FIFO Pedidos de Despacho
typedef struct NodoPedido {
    char nombre_destino[40];    
    int cantidad_solicitada;    
    struct NodoPedido *siguiente; 
} Pedido;

// Estructura para el Nodo del Árbol AVL/ABB Inventario/Lote por Fecha
typedef struct NodoAVL {
    int fecha_vencimiento;      
    int stock_total;            
    char nombre_producto[45];   

    // Conexión Híbrida con la Cola FIFO 
    Pedido *cabeza_pedidos;     
    struct NodoAVL *izquierda;  
    struct NodoAVL *derecha;    
    int altura;                 
} Inventario;

// 2. un soporte con AVL Y COLA

int obtenerAltura(Inventario *lote_actual) {
    if (lote_actual == NULL)
        return 0;
    return lote_actual->altura;
}
int obtenerFactorEquilibrio(Inventario *lote) {
    if (lote == NULL)
        return 0;
    return obtenerAltura(lote->izquierda) - obtenerAltura(lote->derecha);
}
Inventario* nuevoNodoInventario(int fecha, int unidades, const char* nombre) {
    Inventario* nodo = (Inventario*)malloc(sizeof(Inventario));
    
    if (nodo == NULL) {
        perror("Error de asignación de memoria para Inventario");
        exit(EXIT_FAILURE);
    }
    nodo->fecha_vencimiento = fecha;
    nodo->stock_total = unidades; 
    strcpy(nodo->nombre_producto, nombre); 
    
    nodo->izquierda = NULL;
    nodo->derecha = NULL;
    nodo->altura = 1; 
    
    nodo->cabeza_pedidos = NULL; 
    
    return nodo;
}
Inventario *buscarMasAntiguo(Inventario *nodo) {
    Inventario *actual = nodo;
    while (actual != NULL && actual->izquierda != NULL)
        actual = actual->izquierda;
    return actual;
}
int contarPedidos(Pedido *cabeza) {
    int contador = 0;
    Pedido *actual = cabeza;
    while (actual != NULL) {
        contador++;
        actual = actual->siguiente;
    }
    return contador;
}
// 3. ROTACIONES AVL 

Inventario *rotacionDerecha(Inventario *NodoDesbalanceado) {
    Inventario *NodoSuperior = NodoDesbalanceado->izquierda;
    Inventario *SubArbolTemporal = NodoSuperior->derecha;
    NodoSuperior->derecha = NodoDesbalanceado;
    NodoDesbalanceado->izquierda = SubArbolTemporal;

    if (obtenerAltura(NodoDesbalanceado->izquierda) > obtenerAltura(NodoDesbalanceado->derecha)) {
        NodoDesbalanceado->altura = obtenerAltura(NodoDesbalanceado->izquierda) + 1;
    } else {
        NodoDesbalanceado->altura = obtenerAltura(NodoDesbalanceado->derecha) + 1;
    }
    
    if (obtenerAltura(NodoSuperior->izquierda) > obtenerAltura(NodoSuperior->derecha)) {
        NodoSuperior->altura = obtenerAltura(NodoSuperior->izquierda) + 1;
    } else {
        NodoSuperior->altura = obtenerAltura(NodoSuperior->derecha) + 1;
    }
    return NodoSuperior; 
}

Inventario *rotacionIzquierda(Inventario *NodoDesbalanceado) {
    Inventario *NodoSuperior = NodoDesbalanceado->derecha;
    Inventario *SubArbolTemporal = NodoSuperior->izquierda;
    NodoSuperior->izquierda = NodoDesbalanceado;
    NodoDesbalanceado->derecha = SubArbolTemporal;
    if (obtenerAltura(NodoDesbalanceado->izquierda) > obtenerAltura(NodoDesbalanceado->derecha)) {
        NodoDesbalanceado->altura = obtenerAltura(NodoDesbalanceado->izquierda) + 1;
    } else {
        NodoDesbalanceado->altura = obtenerAltura(NodoDesbalanceado->derecha) + 1;
    }
    if (obtenerAltura(NodoSuperior->izquierda) > obtenerAltura(NodoSuperior->derecha)) {
        NodoSuperior->altura = obtenerAltura(NodoSuperior->izquierda) + 1;
    } else {
        NodoSuperior->altura = obtenerAltura(NodoSuperior->derecha) + 1;
    }

    return NodoSuperior; 
}
// 4.  REGISTRO DE MERCANCÍAS INSERCIÓN AVL

Inventario *insertarNodoAVL(Inventario *nodo, int fecha, int unidades, const char* nombre) {
    // 1. Inserción ABB
    if (nodo == NULL) {
        printf("Lote %d de %s con %d uds. registrado exitosamente.\n", fecha, nombre, unidades);
        return nuevoNodoInventario(fecha, unidades, nombre); 
    }
    if (fecha < nodo->fecha_vencimiento) {
        nodo->izquierda = insertarNodoAVL(nodo->izquierda, fecha, unidades, nombre);
    } else if (fecha > nodo->fecha_vencimiento) {
        nodo->derecha = insertarNodoAVL(nodo->derecha, fecha, unidades, nombre);
    } else {
        printf("Error (Regla de Negocio): Ya existe un lote registrado con la fecha de vencimiento %d. No se puede procesar.\n", fecha);
        return nodo; 
    }
    if (obtenerAltura(nodo->izquierda) > obtenerAltura(nodo->derecha)) {
        nodo->altura = obtenerAltura(nodo->izquierda) + 1;
    } else {
        nodo->altura = obtenerAltura(nodo->derecha) + 1;
    }
    
    int balance = obtenerFactorEquilibrio(nodo);
    //  para mantener el AVL 
    if (balance > 1 && fecha < nodo->izquierda->fecha_vencimiento) 
        return rotacionDerecha(nodo);
    if (balance < -1 && fecha > nodo->derecha->fecha_vencimiento) 
        return rotacionIzquierda(nodo);
    if (balance > 1 && fecha > nodo->izquierda->fecha_vencimiento) { 
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }
    if (balance < -1 && fecha < nodo->derecha->fecha_vencimiento) { 
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}
// 5.  REGISTRAR PEDIDO DE DESPACHO ENCOLAR FIFO

Pedido *nuevoNodoPedido(const char *destino, int unidades) {
    Pedido *nuevo = (Pedido *)malloc(sizeof(Pedido));
    if (nuevo == NULL) {
        perror("Error de asignación de memoria para Pedido");
        exit(EXIT_FAILURE);
    }
    strcpy(nuevo->nombre_destino, destino); 
    nuevo->cantidad_solicitada = unidades; 
    nuevo->siguiente = NULL;
    return nuevo;
}
void encolarPedido(Inventario *lote_prioritario, const char *destino, int unidades) {
    
    if (lote_prioritario == NULL) {
        printf("Error: No hay inventario para registrar el pedido.\n");
        return;
    }
    if (unidades <= 0) {
        printf("Error: La cantidad solicitada debe ser positiva.\n");
        return;
    }
    if (unidades > lote_prioritario->stock_total) {
        printf("Error: Stock insuficiente. Se solicitan %d pero solo hay %d disponibles.\n", 
                 unidades, lote_prioritario->stock_total);
        return;
    }

    Pedido *nuevo = nuevoNodoPedido(destino, unidades);

    if (lote_prioritario->cabeza_pedidos == NULL) {
        lote_prioritario->cabeza_pedidos = nuevo;
    } else {
        Pedido *actual = lote_prioritario->cabeza_pedidos;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    lote_prioritario->stock_total -= unidades;
    printf("\n--- Pedido Registrado ---\n");
    printf("Prioridad: Fecha de Vencimiento %d\n", lote_prioritario->fecha_vencimiento);
    printf("Pedido de %d unidades de %s para %s encolado exitosamente.\n", 
             unidades, lote_prioritario->nombre_producto, destino); 
    printf("Nuevo Stock para este lote: %d\n", lote_prioritario->stock_total);
}
// 6. CANCELACIÓN de (ELIMINACIONES)

void liberarCola(Pedido *cabeza) {
    Pedido *actual = cabeza;
    Pedido *siguiente;
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual); 
        actual = siguiente;
    }
}
Inventario *valorMinimoNodo(Inventario *nodo) {
    Inventario *actual = nodo;
    while (actual->izquierda != NULL)
        actual = actual->izquierda;
    return actual;
}
Inventario *eliminarNodoAVL(Inventario *raiz, int fecha) {
    // Eliminación de  ABB
    if (raiz == NULL)
        return raiz;
    if (fecha < raiz->fecha_vencimiento)
        raiz->izquierda = eliminarNodoAVL(raiz->izquierda, fecha);
    else if (fecha > raiz->fecha_vencimiento)
        raiz->derecha = eliminarNodoAVL(raiz->derecha, fecha);
    else {

        // Nodo encontrado :Liberar la cola de pedidos antes de eliminar el nodo
        int pedidos_liberados = contarPedidos(raiz->cabeza_pedidos);
        liberarCola(raiz->cabeza_pedidos);
        printf("\n--- Baja de Producto Exitosa ---\n");
        printf("Lote %d de %s eliminado. Se liberó la memoria de %d pedidos pendientes.\n", 
                 fecha, raiz->nombre_producto, pedidos_liberados);
        
        Inventario *temp;
        
        // Caso 1 y 2: Nodo con 0 o 1 hijo
        if ((raiz->izquierda == NULL) || (raiz->derecha == NULL)) {
            temp = raiz->izquierda ? raiz->izquierda : raiz->derecha;
            if (temp == NULL) { 
                temp = raiz;
                raiz = NULL;
            } else { 
                *raiz = *temp; 
            }
            free(temp);
        } else {
            // Caso 3: Nodo con 2 hijos 
            temp = valorMinimoNodo(raiz->derecha);

            // Transferir del sucesor al nodo actual
            raiz->fecha_vencimiento = temp->fecha_vencimiento;
            raiz->stock_total = temp->stock_total;
            strcpy(raiz->nombre_producto, temp->nombre_producto);
            raiz->cabeza_pedidos = temp->cabeza_pedidos; 
            
            // Eliminar el sucesor
            raiz->derecha = eliminarNodoAVL(raiz->derecha, temp->fecha_vencimiento);
        }
    }
    if (raiz == NULL)
        return raiz;

    if (obtenerAltura(raiz->izquierda) > obtenerAltura(raiz->derecha)) {
        raiz->altura = obtenerAltura(raiz->izquierda) + 1;
    } else {
        raiz->altura = obtenerAltura(raiz->derecha) + 1;
    }
    int balance = obtenerFactorEquilibrio(raiz);

    if (balance > 1 && obtenerFactorEquilibrio(raiz->izquierda) >= 0)
        return rotacionDerecha(raiz);
    if (balance > 1 && obtenerFactorEquilibrio(raiz->izquierda) < 0) {
        raiz->izquierda = rotacionIzquierda(raiz->izquierda);
        return rotacionDerecha(raiz);
    }
    if (balance < -1 && obtenerFactorEquilibrio(raiz->derecha) <= 0)
        return rotacionIzquierda(raiz);
    if (balance < -1 && obtenerFactorEquilibrio(raiz->derecha) > 0) {
        raiz->derecha = rotacionDerecha(raiz->derecha);
        return rotacionIzquierda(raiz);
    }
    return raiz;
}
void cancelarPedido(Inventario *raiz, int fecha_lote, const char *destino_cancelar, int unidades_cancelar) {
    //  Busca el Lote Nodo AVL/ABB
    Inventario *lote = raiz;
    while (lote != NULL && lote->fecha_vencimiento != fecha_lote) {
        if (fecha_lote < lote->fecha_vencimiento) {
            lote = lote->izquierda;
        } else {
            lote = lote->derecha;
        }
    }
    if (lote == NULL) {
        printf("Error: No se encontró el lote con la fecha %d para cancelar el pedido.\n", fecha_lote);
        return;
    }
    //  Busca y elimina el Pedido en la Cola FIFO
    Pedido *actual = lote->cabeza_pedidos;
    Pedido *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->nombre_destino, destino_cancelar) == 0 && actual->cantidad_solicitada == unidades_cancelar) {
            
            if (anterior == NULL) {
                lote->cabeza_pedidos = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            // Restablecer el stock
            lote->stock_total += actual->cantidad_solicitada;
            
            printf("\n--- Cancelación de Pedido Exitosa ---\n");
            printf("Pedido de %d unidades para %s (Lote: %d) cancelado.\n", 
                     actual->cantidad_solicitada, destino_cancelar, fecha_lote);
            printf("Stock restablecido. Nuevo stock de lote de %s: %d\n", lote->nombre_producto, lote->stock_total);
            
            free(actual); 
            return;
        }

        anterior = actual;
        actual = actual->siguiente;
    }
    printf("Error: No se encontró un pedido idéntico (destino y cantidad) para el lote %d.\n", fecha_lote);
}
// 7.  REPORTE DE ESTADO RECORRIDO IN-ORDER

void recorridoInOrder(Inventario *raiz) {
    if (raiz != NULL) {
        recorridoInOrder(raiz->izquierda); 
        int num_pedidos = contarPedidos(raiz->cabeza_pedidos);
        printf("| %d | %-15s | %-10d | %-8d |\n", 
                 raiz->fecha_vencimiento, 
                 raiz->nombre_producto, 
                 raiz->stock_total, 
                 num_pedidos);
        recorridoInOrder(raiz->derecha); 
    }
}
// 8. MAIN Y MENÚ DEL PROYECTO FINAL

void mostrarMenu() {
    printf("  SISTEMA LOGÍSTICO PUERTO DE BUENAVENTURA\n");
    printf(" DESTINOS: Timbiqui, Guapi, Nuqui, Bahia_Solano\n");
    printf("1. Recepción de Mercancía (Insertar en AVL)\n");
    printf("2. Registrar Pedido de Despacho (Encolar en FIFO)\n");
    printf("3. Cancelación / Eliminación\n");
    printf("  3a. Baja de Producto (Eliminar Lote completo)\n");
    printf("  3b. Cancelar Pedido (Eliminar de la Cola)\n");
    printf("4. Reporte de Estado (Listado In-Order)\n");
    printf("5. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    Inventario *raiz = NULL;
    int opcion;
    int fecha;
    int unidades; 
    char nombre[45], destino[40]; 
    // Carga de Datos
    raiz = insertarNodoAVL(raiz, 20251110, 500, "Leche"); 
    raiz = insertarNodoAVL(raiz, 20260315, 1200, "Granos");
    raiz = insertarNodoAVL(raiz, 20251125, 300, "Pescado");
    raiz = insertarNodoAVL(raiz, 20260105, 850, "Atun");
    raiz = insertarNodoAVL(raiz, 20251215, 400, "Tuberculos"); 
    
    printf("\n--- Inventario Inicial Cargado \n");
    printf("FECHA VENC.  PRODUCTO      STOCK DISP  PEDIDOS \n");
    recorridoInOrder(raiz);

    // Registro de Pedidos FIFO 
    Inventario *prioritario = buscarMasAntiguo(raiz); 

    //4 destinos Guapi, Timbiqui, Nuqui, Bahia_Solano
    encolarPedido(prioritario, "Guapi", 75);
    encolarPedido(prioritario, "Timbiqui", 75);
    encolarPedido(prioritario, "Nuqui", 40);
    encolarPedido(prioritario, "Bahia Solano", 40);
    
    printf("\n Pedidos Iniciales Cargados (Separados) \n");
    printf(" FECHA VENC. PRODUCTO    STOCK DISP PEDIDOS \n");
    recorridoInOrder(raiz);

    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada inválida. Por favor, ingrese un número.\n");
            while (getchar() != '\n'); 
            opcion = 0;
            continue;
        } 
        switch (opcion) {
            case 1: 
                printf("\n[1. RECEPCIÓN DE MERCANCÍA]\n");
                printf("Ingrese Fecha Vencimiento ");
                scanf("%d", &fecha);
                printf("Ingrese Nombre del Producto : ");
                scanf("%s", nombre);
                printf("Ingrese Cantidad de Unidades "); 
                scanf("%d", &unidades); 
                raiz = insertarNodoAVL(raiz, fecha, unidades, nombre);
                break;

            case 2: { 
                printf("\n[2. REGISTRAR PEDIDO DE DESPACHO]\n");
                Inventario *prioritario = buscarMasAntiguo(raiz);
                
                if (prioritario == NULL) {
                    printf("Error: No hay inventario para despachar.\n");
                    break;
                }
                printf("Lote Prioritario Encontrado: Fecha %d (%s). Stock: %d\n", 
                         prioritario->fecha_vencimiento, prioritario->nombre_producto, prioritario->stock_total);       
                printf("Ingrese Destino ");
                scanf("%s", destino);
                printf("Ingrese Unidades Solicitadas: "); 
                scanf("%d", &unidades); 
                
                encolarPedido(prioritario, destino, unidades);
                break;
            }
            case 3:
                printf("\n[3. CANCELACIÓN / ELIMINACIÓN]\n");
                printf("Ingrese 3a para Baja de Producto (Lote) o 3b para Cancelar Pedido (Cola): ");
                char sub_opcion[3];
                scanf("%s", sub_opcion);
                
                if (strcmp(sub_opcion, "3a") == 0) {
                    printf("Ingrese Fecha Vencimiento del lote a dar de baja");
                    scanf("%d", &fecha);
                    raiz = eliminarNodoAVL(raiz, fecha);
                } else if (strcmp(sub_opcion, "3b") == 0) {
                    printf("Ingrese Fecha Vencimiento del lote del pedido a cancelar ");
                    scanf("%d", &fecha);
                    printf("Ingrese Destino del Pedido a Cancelar  ");
                    scanf("%s", destino);
                    printf("Ingrese Unidades del Pedido a Cancelar  "); 
                    scanf("%d", &unidades); 
                    cancelarPedido(raiz, fecha, destino, unidades);
                } else {
                    printf("Opción de cancelación inválida.\n");
                }
                break;
            
            case 4: 
                printf("\n 4. REPORTE DE ESTADO (ORDENADO POR PRIORIDAD DE VENCIMIENTO \n");
                printf(" FECHA VENC.  PRODUCTO      STOCK DISP  PEDIDOS \n");
                recorridoInOrder(raiz);
                break;
            case 5:
                printf("Saliendo del sistema.\n");
                break;

            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (opcion != 5);

    return 0;
}