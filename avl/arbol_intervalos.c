#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "arbol_intervalos.h"
#include "deque.h"

bool inexistente(Rango rango) {
  return rango.a > rango.b;
}

typedef ArbolIntervalosNode* (Popper(Deque*)) ;

int max(int a, int b) {
  return a > b? a:b;
}

void itree_recorrer_fs(ArbolIntervalos *arbol, Accion actuar, Popper pop) {
  if (arbol->arbolAvlNode == NULL) {
    return;
  }

  Deque *deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = pop(deque);

    if (nodo->izquierda) {
      deque_push_front(deque, nodo->izquierda);
    }
    if (nodo->derecha) {
      deque_push_front(deque, nodo->derecha);
    }

    actuar(nodo);
  }

  deque_destruir(deque);
}

ArbolIntervalos *arbolintervalos_crear() {
  ArbolIntervalos* avl = calloc(1, sizeof(ArbolIntervalos));

  return avl;
}

ArbolIntervalos * arbolintervalos_copiar(ArbolIntervalos * arbol) {
  ArbolIntervalos * copia = arbolintervalos_crear();

  if(arbol->arbolAvlNode == NULL) {
    return copia;
  }

  Deque* deque = deque_crear();
  deque_push_front(deque, arbol->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode* nodo = deque_pop_front(deque);

    if(nodo->izquierda) {
      deque_push_front(deque, nodo->izquierda);
    }
    if(nodo->derecha) {
      deque_push_front(deque, nodo->derecha);
    }

    arbolintervalos_insertar(copia, nodo->rango);
  }

  deque_destruir(deque);

  return copia;
}

void arbolintervalos_destruir(ArbolIntervalos *tree) {
  itree_recorrer_fs(tree, free, deque_pop_back);
  free(tree);
}

ArbolIntervalos * arbolintervalos_union(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB) {
  ArbolIntervalos *base;
  ArbolIntervalos *aUnir;

  if (arbolA->arbolAvlNode->alto > arbolB->arbolAvlNode->alto) {
    base = arbolintervalos_copiar(arbolB);
    aUnir = arbolA;
  } else {
    base = arbolintervalos_copiar(arbolA);
    aUnir = arbolB;
  }

  if (aUnir->arbolAvlNode == NULL) return base;

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

ArbolIntervalos * arbolintervalos_interseccion(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB) {
  ArbolIntervalos *base;
  ArbolIntervalos *aIntersecar;

  if (arbolA->arbolAvlNode->alto > arbolB->arbolAvlNode->alto) {
    base = arbolintervalos_copiar(arbolB);
    aIntersecar = arbolA;
  } else {
    base = arbolintervalos_copiar(arbolA);
    aIntersecar = arbolB;
  }

  if (aIntersecar->arbolAvlNode == NULL) return base;

  Deque *deque = deque_crear();
  deque_push_front(deque, aIntersecar->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = deque_pop_front(deque);

    if (nodo->izquierda) deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha) deque_push_front(deque, nodo->derecha);

    Rango interseccion = arbolintervalos_intersectar(base, nodo->rango);

    if(!inexistente(interseccion)) {
      //TODO
    }
  }

  deque_destruir(deque);

  return base;
}

ArbolIntervalos * arbolintervalos_resta(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB) {
  //TODO
}

ArbolIntervalos *arbolintervalos_complemento(ArbolIntervalos *arbol) {
  //TODO
}

void actualizar_max_nodo(ArbolIntervalosNode* nodo) {
  if(nodo->izquierda == NULL && nodo->derecha == NULL) {
    nodo->maxB = nodo->rango.b;
  } else if(nodo->izquierda == NULL) {
    nodo->maxB = nodo->derecha->maxB;
  } else if (nodo->derecha == NULL) {
    nodo->maxB = nodo->izquierda->maxB;
  } else {
    nodo->maxB = nodo->izquierda->maxB > nodo->derecha->maxB ?
                 nodo->izquierda->maxB : nodo->derecha->maxB;
  }
}

void rotacion_simple_izquierda(
  ArbolIntervalosNode** posicionDelNodo,
  ArbolIntervalosNode* nodo
) {
  ArbolIntervalosNode* nuevoHijo = nodo->izquierda;
  ArbolIntervalosNode* nuevoNietoDerecha = nodo;
  ArbolIntervalosNode* nuevoIzquierdaNietoDerecha = nodo->izquierda->derecha;

  *posicionDelNodo = nuevoHijo;
  nuevoHijo->derecha = nuevoNietoDerecha;
  nuevoNietoDerecha->izquierda = nuevoIzquierdaNietoDerecha;

  nuevoNietoDerecha->alto -= 2;

  actualizar_max_nodo(nuevoNietoDerecha);
  actualizar_max_nodo(nuevoHijo);
}

void rotacion_simple_derecha(
  ArbolIntervalosNode** posicionDelNodo,
  ArbolIntervalosNode* nodo
) {
  ArbolIntervalosNode* nuevoHijo = nodo->derecha;
  ArbolIntervalosNode* nuevoNietoIzquierda = nodo;
  ArbolIntervalosNode* nuevaDerechaNietoIzquierda = nodo->derecha->izquierda;

  *posicionDelNodo = nuevoHijo;
  nuevoHijo->izquierda = nuevoNietoIzquierda;
  nuevoNietoIzquierda->derecha = nuevaDerechaNietoIzquierda;

  nuevoNietoIzquierda->alto -= 2;

  actualizar_max_nodo(nuevoNietoIzquierda);
  actualizar_max_nodo(nuevoHijo);
}

void rebalancear(
  Deque* dequeDireccion
) {
  while (!deque_vacio(dequeDireccion)) {
    ArbolIntervalosNode** posicionDelNodo = (ArbolIntervalosNode**) deque_pop_front(dequeDireccion);
    ArbolIntervalosNode* chequear = *posicionDelNodo;

    actualizar_max_nodo(chequear);

    if(chequear->izquierda && chequear->derecha) {
      chequear->alto = max(chequear->izquierda->alto, chequear->derecha->alto) + 1;
    } else if(chequear->izquierda) {
      chequear->alto = chequear->izquierda->alto + 1;
    } else if(chequear->derecha) {
      chequear->alto = chequear->derecha->alto + 1;
    } else {
      chequear->alto = 1;
    }

    if(-1 <= arbolintervalos_factor_de_equilibrio(chequear)
       && arbolintervalos_factor_de_equilibrio(chequear) <= 1) {
      continue;
    }

    if (arbolintervalos_factor_de_equilibrio(chequear->izquierda) < 0) {
      rotacion_simple_izquierda(posicionDelNodo, chequear);
      break;
    } else if (arbolintervalos_factor_de_equilibrio(chequear->derecha) > 0) {
      rotacion_simple_derecha(posicionDelNodo, chequear);
      break;
    } else if(arbolintervalos_factor_de_equilibrio(chequear->izquierda) > 0) {
      rotacion_simple_derecha(&(chequear->izquierda), chequear->izquierda);
      rotacion_simple_izquierda(posicionDelNodo, chequear);
      break;
    } else if(arbolintervalos_factor_de_equilibrio(chequear->derecha) < 0) {
      rotacion_simple_izquierda(&(chequear->derecha), chequear->derecha);
      rotacion_simple_derecha(posicionDelNodo, chequear);
      break;
    }
  }

  while (!deque_vacio(dequeDireccion)) {
    ArbolIntervalosNode** chequear = deque_pop_front(dequeDireccion);
    actualizar_max_nodo(*chequear);
  }
}

bool arbolintervalos_insertar(ArbolIntervalos *arbol, Rango rango) {
  if(inexistente(rango)) {
    return false;
  }

  //TODO optimizar arbol

  ArbolIntervalosNode* nodo = calloc(1, sizeof(ArbolIntervalosNode));
  nodo->rango = rango;
  nodo->maxB = rango.b;
  nodo->alto = 1;

  Deque* dequeDireccion = deque_crear();

  {
    ArbolIntervalosNode **pos = &(arbol->arbolAvlNode);

    while (*pos != NULL) {
      deque_push_front(dequeDireccion, pos);

      ArbolIntervalosNode *chequear = *pos;

      if (rango.a < chequear->rango.a
          || (chequear->rango.a == rango.a && rango.b < chequear->rango.b)) {
        pos = &((*pos)->izquierda);
      } else if (chequear->rango.a < rango.a
                 || (chequear->rango.a == rango.a &&
                     chequear->rango.b < rango.b)) {
        pos = &((*pos)->derecha);
      } else {
        deque_destruir(dequeDireccion);

        return false;
      }
    }

    *pos = nodo;
  }

  rebalancear(dequeDireccion);

  deque_destruir(dequeDireccion);

  return true;
}

bool arbolintervalos_eliminar(ArbolIntervalos *arbol, Rango rango) {
  if(arbol->arbolAvlNode == NULL) {
    return false;
  }

  Deque *dequeDireccion = deque_crear();

  ArbolIntervalosNode **posicionDelNodoAEliminar;
  ArbolIntervalosNode *nodoAEliminar;

  {//Rastreo del nodo a eliminar
    ArbolIntervalosNode **pos = &(arbol->arbolAvlNode);

    while (*pos != NULL) {
      deque_push_front(dequeDireccion, pos);

      ArbolIntervalosNode *chequear = *pos;

      if (rango.a < chequear->rango.a
          || (chequear->rango.a == rango.a && rango.b < chequear->rango.b)) {
        pos = &((*pos)->izquierda);
      } else if (chequear->rango.a < rango.a
                 || (chequear->rango.a == rango.a &&
                     chequear->rango.b < rango.b)) {
        pos = &((*pos)->derecha);
      } else if (rango.a == (*pos)->rango.a && rango.b == (*pos)->rango.b) {
        nodoAEliminar = *pos;
        posicionDelNodoAEliminar = pos;
        *pos = NULL;
      } else {
        deque_destruir(dequeDireccion);
        return false;
      }
    }
  }

  {//Rastreo del nodo que lo reemplaza
    if (nodoAEliminar->izquierda != NULL && nodoAEliminar->derecha != NULL) {
      ArbolIntervalosNode **posicionDelNodoSacado;
      ArbolIntervalosNode *nuevoHijo;

      nuevoHijo = nodoAEliminar->derecha;
      posicionDelNodoSacado = &(nodoAEliminar->derecha);

      deque_push_front(dequeDireccion, posicionDelNodoSacado);

      while (nuevoHijo->izquierda != NULL) {
        nuevoHijo = nuevoHijo->izquierda;
        posicionDelNodoSacado = &(nodoAEliminar->derecha);

        deque_push_front(dequeDireccion, posicionDelNodoSacado);
      }

      *posicionDelNodoSacado = NULL;

      deque_pop_front(dequeDireccion);

      nuevoHijo->izquierda = nodoAEliminar->izquierda;
      nuevoHijo->derecha = nodoAEliminar->derecha;

      *posicionDelNodoAEliminar = nuevoHijo;
    } else if (nodoAEliminar->izquierda != NULL) {
      *posicionDelNodoAEliminar = nodoAEliminar->izquierda;
      if (!deque_vacio(dequeDireccion)) {
        deque_pop_front(dequeDireccion);
      }
    } else if (nodoAEliminar->derecha != NULL) {
      *posicionDelNodoAEliminar = nodoAEliminar->derecha;
      if (!deque_vacio(dequeDireccion)) {
        deque_pop_front(dequeDireccion);
      }
    } else {
      *posicionDelNodoAEliminar = NULL;
      if (!deque_vacio(dequeDireccion)) {
        deque_pop_front(dequeDireccion);
      }
    }

    free(nodoAEliminar);
  }

  rebalancear(dequeDireccion);

  deque_destruir(dequeDireccion);

  return true;
}

bool existe_interseccion(Rango uno, Rango dos) {
  return (uno.b >= dos.a && dos.b >= uno.a);
}

Rango arbolintervalos_intersectar(ArbolIntervalos *tree, Rango rango) {
  ArbolIntervalosNode* nodo = tree->arbolAvlNode;

  while (nodo != NULL) {
    if(existe_interseccion(nodo->rango, rango)) {
      return nodo->rango;
    }

    if(nodo->maxB < rango.a) {
      return RANGO_INEXISTENTE;
    }

    if(rango.a <= nodo->rango.a) {
      if (nodo->izquierda && rango.a <= nodo->izquierda->maxB) {
        nodo = nodo->izquierda;
      } else {
        return RANGO_INEXISTENTE;
      }
    } else {
      if (nodo->derecha && rango.a <= nodo->derecha->maxB) {
        nodo = nodo->derecha;
      } else {
        return RANGO_INEXISTENTE;
      }
    }
  }

  return RANGO_INEXISTENTE;
}

void arbolintervalos_imprimir_arbol(ArbolIntervalos *arbol) {
  if(arbol->arbolAvlNode == NULL) {
    printf("Vacio\n");
    return;
  }

  unsigned int assumedPos = 1;
  unsigned int nodosEnDeque = 0;

  Deque* deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNode);
  nodosEnDeque++;

  int i = 0;
  for (; nodosEnDeque > 0; i++) {
    ArbolIntervalosNode* nodo = deque_pop_back(deque);

    if(!nodo) {
      printf(" {    NULL    }");
      deque_push_front(deque, NULL);
      deque_push_front(deque, NULL);
    } else {
      printf(
        " {m: %d, r: [%d, %d], a: %d}",
        nodo->maxB,
        nodo->rango.a,
        nodo->rango.b,
        nodo->alto
        );
      nodosEnDeque--;

      deque_push_front(deque, nodo->izquierda);
      nodosEnDeque += (nodo->izquierda != NULL ? 1 : 0);
      deque_push_front(deque, nodo->derecha);
      nodosEnDeque += (nodo->derecha != NULL ? 1 : 0);
    }

    if(assumedPos == i+1 && nodosEnDeque > 0) {
      printf("\n");
      assumedPos <<= 1;
      assumedPos += 1;
    }
  }

  for(;i+1 <= assumedPos; i++) {
    printf(" {    NULL    }");
  }

  printf("\n");
}

int arbolintervalos_factor_de_equilibrio(ArbolIntervalosNode *nodo) {
  if(nodo->derecha && nodo->izquierda) {
    return nodo->derecha->alto - nodo->izquierda->alto;
  } else if(nodo->derecha) {
    return nodo->derecha->alto;
  } else if(nodo->izquierda) {
    return -nodo->izquierda->alto;
  } else {
    return 0;
  }
}