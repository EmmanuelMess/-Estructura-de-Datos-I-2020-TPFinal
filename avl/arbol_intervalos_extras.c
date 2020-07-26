#include <stddef.h>
#include "arbol_intervalos_extras.h"
#include "arbol_intervalos.h"
#include "deque.h"
#include "stack_rango.h"

ArbolIntervalos * arbolintervalos_union(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB) {
  ArbolIntervalos *base;
  ArbolIntervalos *aUnir;

  if (arbolA->arbolAvlNodo == NULL) return arbolintervalos_copiar(arbolB);
  if (arbolB->arbolAvlNodo == NULL) return arbolintervalos_copiar(arbolA);

  if (arbolA->arbolAvlNodo->alto > arbolB->arbolAvlNodo->alto) {
    base = arbolintervalos_copiar(arbolB);
    aUnir = arbolA;
  } else {
    base = arbolintervalos_copiar(arbolA);
    aUnir = arbolB;
  }

  //DFS del arbol aUnir (ver arbol_intervalos.c:recorrer_xfirstsearch)
  Deque *deque = deque_crear();
  deque_push_front(deque, aUnir->arbolAvlNodo);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNodo *nodo = deque_pop_front(deque);

    if (nodo->izquierda) deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha) deque_push_front(deque, nodo->derecha);

    arbolintervalos_insertar(base, nodo->rango);
  }

  deque_destruir(deque);

  return base;
}

ArbolIntervalos * arbolintervalos_interseccion(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB) {
  ArbolIntervalos * base = arbolintervalos_crear();
  ArbolIntervalos * aRecorrer;
  ArbolIntervalos * aIntersecar;

  if (arbolA->arbolAvlNodo == NULL) return arbolintervalos_copiar(arbolA);
  if (arbolB->arbolAvlNodo == NULL) return arbolintervalos_copiar(arbolB);

  if (arbolA->arbolAvlNodo->alto > arbolB->arbolAvlNodo->alto) {
    aRecorrer = arbolB;
    aIntersecar = arbolA;
  } else {
    aRecorrer = arbolA;
    aIntersecar = arbolB;
  }

  //DFS del arbol aRecorrer (ver arbol_intervalos.c:recorrer_xfirstsearch)
  Deque *deque = deque_crear();
  deque_push_front(deque, aRecorrer->arbolAvlNodo);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNodo *nodo = deque_pop_front(deque);

    if (nodo->izquierda) deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha) deque_push_front(deque, nodo->derecha);

    StackRango *dequeRangos = stackrango_crear();

    stackrango_push(dequeRangos, nodo->rango);
    // Si el nodo->rango interseca varias veces, el while lidia con eso,
    // usa la invariante de que los rangos son disjuntos
    while (!stackrango_vacio(dequeRangos)) {
      Rango rangoBase = stackrango_pop(dequeRangos);
      Rango interseccion = arbolintervalos_intersectar(aIntersecar, rangoBase);

      if (!inexistente(interseccion)) {
        arbolintervalos_insertar(base, rango_interseccion(rangoBase, interseccion));

        Rango antes = RANGO_INEXISTENTE;
        Rango despues = RANGO_INEXISTENTE;
        rango_resta(rangoBase, interseccion, &antes, &despues);

        if(!inexistente(antes))
          stackrango_push(dequeRangos, antes);

        if(!inexistente(despues))
          stackrango_push(dequeRangos, despues);
      }
    }

    stackrango_destruir(dequeRangos);
  }

  deque_destruir(deque);

  return base;
}

ArbolIntervalos * arbolintervalos_resta(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB) {
  if (arbolA->arbolAvlNodo == NULL) return arbolintervalos_crear();
  if (arbolB->arbolAvlNodo == NULL) return arbolintervalos_copiar(arbolA);

  ArbolIntervalos * base = arbolintervalos_copiar(arbolA);

  //DFS del arbol base (ver arbol_intervalos.c:recorrer_xfirstsearch)
  Deque *deque = deque_crear();
  deque_push_front(deque, base->arbolAvlNodo);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNodo *nodo = deque_pop_front(deque);

    if (nodo->izquierda) deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha) deque_push_front(deque, nodo->derecha);

    Rango rangoBase = nodo->rango;
    Rango interseccion = arbolintervalos_intersectar(arbolB, rangoBase);

    if(!inexistente(interseccion)) {
      arbolintervalos_eliminar(base, rangoBase);

      Rango resultadoA = RANGO_INEXISTENTE;
      Rango resultadoB = RANGO_INEXISTENTE;
      rango_resta(rangoBase, interseccion, &resultadoA, &resultadoB);

      if(!inexistente(resultadoA)) arbolintervalos_insertar(base, resultadoA);
      if(!inexistente(resultadoB)) arbolintervalos_insertar(base, resultadoB);
    } else arbolintervalos_insertar(base, rangoBase);
  }

  deque_destruir(deque);

  return base;
}

ArbolIntervalos * arbolintervalos_complemento(ArbolIntervalos *arbol) {
  ArbolIntervalos * base = arbolintervalos_crear();
  arbolintervalos_insertar(base, (Rango){.a = INT_MIN, .b = INT_MAX});

  if (arbol->arbolAvlNodo == NULL) return base;

  //DFS del arbol base (ver arbol_intervalos.c:recorrer_xfirstsearch)
  Deque *deque = deque_crear();
  deque_push_front(deque, base->arbolAvlNodo);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNodo *nodo = deque_pop_front(deque);

    if (nodo->izquierda) deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha) deque_push_front(deque, nodo->derecha);

    Rango rangoBase = nodo->rango;
    Rango interseccion = arbolintervalos_intersectar(arbol, rangoBase);

    if(!inexistente(interseccion)) {
      arbolintervalos_eliminar(base, rangoBase);

      Rango resultadoA = RANGO_INEXISTENTE;
      Rango resultadoB = RANGO_INEXISTENTE;
      rango_resta(rangoBase, interseccion, &resultadoA, &resultadoB);

      if(!inexistente(resultadoA)) arbolintervalos_insertar(base, resultadoA);
      if(!inexistente(resultadoB)) arbolintervalos_insertar(base, resultadoB);
    } else arbolintervalos_insertar(base, rangoBase);
  }

  deque_destruir(deque);

  return base;
}