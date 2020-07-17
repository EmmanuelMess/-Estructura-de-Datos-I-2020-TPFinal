#include <string.h>
#include <wchar.h>
#include "arbol_intervalos.h"
#include "deque.h"
#include "matematica.h"

typedef ArbolIntervalosNode* (Popper(Deque*)) ;

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
  itree_recorrer_fs(tree, (Accion*) free, (Popper*) deque_pop_back);
  free(tree);
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

void actualizar_alto_nodo(ArbolIntervalosNode* nodo) {
  if(nodo->izquierda && nodo->derecha) {
    nodo->alto = max(nodo->izquierda->alto, nodo->derecha->alto) + 1;
  } else if(nodo->izquierda) {
    nodo->alto = nodo->izquierda->alto + 1;
  } else if(nodo->derecha) {
    nodo->alto = nodo->derecha->alto + 1;
  } else {
    nodo->alto = 1;
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
  actualizar_alto_nodo(nuevoNietoDerecha);
  actualizar_max_nodo(nuevoHijo);
  actualizar_alto_nodo(nuevoHijo);
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
  actualizar_alto_nodo(nuevoNietoIzquierda);
  actualizar_max_nodo(nuevoHijo);
  actualizar_alto_nodo(nuevoHijo);
}

void rebalancear(
  Deque* dequeDireccion
) {
  while (!deque_vacio(dequeDireccion)) {
    ArbolIntervalosNode** posicionDelNodo = (ArbolIntervalosNode**) deque_pop_front(dequeDireccion);
    ArbolIntervalosNode* chequear = *posicionDelNodo;

    actualizar_max_nodo(chequear);

    actualizar_alto_nodo(chequear);

    if(-1 <= arbolintervalos_factor_equilibrio(chequear)
       && arbolintervalos_factor_equilibrio(chequear) <= 1) {
      continue;
    }

    if (arbolintervalos_factor_equilibrio(chequear->izquierda) < 0) {
      rotacion_simple_izquierda(posicionDelNodo, chequear);
      break;
    } else if (arbolintervalos_factor_equilibrio(chequear->derecha) > 0) {
      rotacion_simple_derecha(posicionDelNodo, chequear);
      break;
    } else if(arbolintervalos_factor_equilibrio(chequear->izquierda) > 0) {
      rotacion_simple_derecha(&(chequear->izquierda), chequear->izquierda);
      rotacion_simple_izquierda(posicionDelNodo, chequear);
      break;
    } else if(arbolintervalos_factor_equilibrio(chequear->derecha) < 0) {
      rotacion_simple_izquierda(&(chequear->derecha), chequear->derecha);
      rotacion_simple_derecha(posicionDelNodo, chequear);
      break;
    }
  }

  while (!deque_vacio(dequeDireccion)) {
    ArbolIntervalosNode** chequear = deque_pop_front(dequeDireccion);
    actualizar_max_nodo(*chequear);
    actualizar_alto_nodo(*chequear);
  }
}

bool arbolintervalos_insertar(ArbolIntervalos *arbol, Rango rango) {
  if(inexistente(rango)) {
    return false;
  }

  Rango rangoExtendido = {
    .a = rango.a > INT_MIN? rango.a - 1 : INT_MIN,
    .b = rango.b < INT_MAX? rango.b + 1 : INT_MAX,
  };
  Rango encontrado = arbolintervalos_intersectar(arbol, rangoExtendido);

  while (!inexistente(encontrado)) {
    arbolintervalos_eliminar(arbol, encontrado);
    rango = rango_union(rango, encontrado);
    rangoExtendido = (Rango) {
      .a = rango.a > INT_MIN? rango.a - 1 : INT_MIN,
      .b = rango.b < INT_MAX? rango.b + 1 : INT_MAX,
    };
    encontrado = arbolintervalos_intersectar(arbol, rangoExtendido);
  }

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

Rango arbolintervalos_intersectar(ArbolIntervalos *tree, Rango rango) {
  ArbolIntervalosNode* nodo = tree->arbolAvlNode;

  while (nodo != NULL) {
    if(rango_intersecan(nodo->rango, rango)) {
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

void arbolintervalos_imprimir(ArbolIntervalos *arbol) {
  if(arbol->arbolAvlNode == NULL) {
    wprintf(L"Vacio\n");
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
      wprintf(L" {    NULL    }");
      deque_push_front(deque, NULL);
      deque_push_front(deque, NULL);
    } else {
      wprintf(
        L" {m: %d, r: [%d, %d], a: %d}",
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
      wprintf(L"\n");
      assumedPos <<= 1;
      assumedPos += 1;
    }
  }

  for(;i+1 <= assumedPos; i++) {
    wprintf(L" {    NULL    }");
  }

  wprintf(L"\n");
}

int arbolintervalos_factor_equilibrio(ArbolIntervalosNode *nodo) {
  if(nodo == NULL) {
    return 0;
  }

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