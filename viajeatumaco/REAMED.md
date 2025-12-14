ana faisury garcia anizaes


Este taller implementa un sistema de gestión de pasajeros del Muelles Turístico de Tumaco,tinbiqui juanchaco, guapi utilizando un árbol binario de búsqueda para organizar la información de manera eficiente.
 permite:
Registrar pasajeros con:
Documento
Destino
Tipo de pasaje
Organizar automáticamente cada pasajero dentro de un árbol binario, según su documento.
Mostrar los pasajeros en tres órdenes distintos:
Inorden
Preorden
Postorden
Contar el total de pasajeros registrados.
Eliminar un pasajero por documento.
Mantener la estructura del árbol siempre ordenada.
  está basado en:
 Árboles Binarios de Búsqueda (ABB)

Cada pasajero se acomoda en el árbol según su documento:

Si el documento es menor, va a la izquierda (ida).

Si el documento es mayor, va a la derecha (regreso).

Esto permite:

Búsquedas rápidas

Listados ordenados

Eliminaciones correctas

Inserciones organizadas

Se usa una estructura (struct) llamada Pasajero, que guarda:

Documento

Destino

Tipo de pasaje

Puntero a la izquierda (ida)

Puntero a la derecha (regreso)

El árbol se construye dinámicamente usando memoria asignada con malloc().

El objetivo es gestionar pasajeros de forma ordenada y eficiente, aplicando conceptos de:

Estructuras de datos

Árboles binarios

Recorridos (inorden, preorden, postorden)

Inserción y eliminación en ABB
    
