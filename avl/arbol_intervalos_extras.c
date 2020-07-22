#include <stddef.h>
#include "arbol_intervalos_extras.h"
#include "arbol_intervalos.h"
#include "deque.h"
#include "deque_rango.h"

ArbolIntervalos * arbolintervalos_union(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB) {
  ArbolIntervalos *base;
  ArbolIntervalos *aUnir;

  if (arbolA->arbolAvlNode == NULL) return arbolintervalos_copiar(arbolB);
  if (arbolB->arbolAvlNode == NULL) return arbolintervalos_copiar(arbolA);

  if (arbolA->arbolAvlNode->alto > arbolB->arbolAvlNode->alto) {
    base = arbolintervalos_copiar(arbolB);
    aUnir = arbolA;
  } else {
    base = arbolintervalos_copiar(arbolA);
    aUnir = arbolB;
  }

  Deque *deque = deque_crear();
  deque_push_front(deque, aUnir->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = deque_pop_front(deque);

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

  if (arbolA->arbolAvlNode == NULL) return arbolintervalos_copiar(arbolA);
  if (arbolB->arbolAvlNode == NULL) return arbolintervalos_copiar(arbolB);

  if (arbolA->arbolAvlNode->alto > arbolB->arbolAvlNode->alto) {
    aRecorrer = arbolB;
    aIntersecar = arbolA;
  } else {
    aRecorrer = arbolA;
    aIntersecar = arbolB;
  }

  Deque *deque = deque_crear();
  deque_push_front(deque, aRecorrer->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = deque_pop_front(deque);

    if (nodo->izquierda) deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha) deque_push_front(deque, nodo->derecha);


    DequeRango *dequeRangos = dequerango_crear();

    dequerango_push_front(dequeRangos, nodo->rango);

    while (!dequerango_vacio(dequeRangos)) {
      Rango rangoBase = dequerango_pop_front(dequeRangos);
      Rango interseccion = arbolintervalos_intersectar(aIntersecar, rangoBase);

      if (!inexistente(interseccion)) {
        arbolintervalos_insertar(base, rango_interseccion(rangoBase, interseccion));

        Rango antes = RANGO_INEXISTENTE;
        Rango despues = RANGO_INEXISTENTE;
        rango_resta(rangoBase, interseccion, &antes, &despues);

        if(!inexistente(antes))
          dequerango_push_front(dequeRangos, antes);

        if(!inexistente(despues))
          dequerango_push_front(dequeRangos, despues);
      }
    }

    dequerango_destruir(dequeRangos);
  }

  deque_destruir(deque);

  return base;
}

ArbolIntervalos * arbolintervalos_resta(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB) {
  if (arbolA->arbolAvlNode == NULL) return arbolintervalos_crear();
  if (arbolB->arbolAvlNode == NULL) return arbolintervalos_copiar(arbolA);

  ArbolIntervalos * base = arbolintervalos_copiar(arbolA);

  Deque *deque = deque_crear();
  deque_push_front(deque, base->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = deque_pop_front(deque);

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

  if (arbol->arbolAvlNode == NULL) return base;

  Deque *deque = deque_crear();
  deque_push_front(deque, base->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = deque_pop_front(deque);

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