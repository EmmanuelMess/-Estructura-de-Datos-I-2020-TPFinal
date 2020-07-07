#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "arbol_avl.h"
#include "deque.h"

bool inexistente(Rango rango) {
  return rango.a > rango.b;
}

typedef ArbolAvlNode* (Popper(Deque*)) ;

int max(int a, int b) {
  return a > b? a:b;
}

void itree_recorrer_fs(
  ArbolAvl *arbol,
  Accion actuar,
  Popper pop
) {
  if(arbol->arbolAvlNode == NULL) {
    return;
  }

  Deque* deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolAvlNode* nodo = pop(deque);

    if(nodo->izquierda) {
      deque_push_front(deque, nodo->izquierda);
    }
    if(nodo->derecha) {
      deque_push_front(deque, nodo->derecha);
    }

    actuar(nodo);
  }

  deque_destruir(deque);
}

ArbolAvl *itree_crear() {
  ArbolAvl* avl = calloc(1, sizeof(ArbolAvl));

  return avl;
}

ArbolAvl * itree_copiar(ArbolAvl * arbolA) {
  ArbolAvl * copia = itree_crear();

  if(arbolA->arbolAvlNode == NULL) {
    return copia;
  }

  Deque* deque = deque_crear();
  deque_push_front(deque, arbolA->arbolAvlNode);

  while (!deque_vacio(deque)) {
    ArbolAvlNode* nodo = deque_pop_front(deque);

    if(nodo->izquierda) {
      deque_push_front(deque, nodo->izquierda);
    }
    if(nodo->derecha) {
      deque_push_front(deque, nodo->derecha);
    }

    itree_insertar(copia, nodo->rango);
  }

  deque_destruir(deque);

  return copia;
}

ArbolAvl * itree_union(ArbolAvl * arbolA, ArbolAvl * arbolB) {

}

void itree_destruir(ArbolAvl *tree) {
  itree_recorrer_fs(tree, free, deque_pop_back);
  free(tree);
}

void actualizar_max_nodo(ArbolAvlNode* nodo) {
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
  ArbolAvlNode** posicionDelNodo,
  ArbolAvlNode* nodo
) {
  ArbolAvlNode* nuevoHijo = nodo->izquierda;
  ArbolAvlNode* nuevoNietoDerecha = nodo;
  ArbolAvlNode* nuevoIzquierdaNietoDerecha = nodo->izquierda->derecha;

  *posicionDelNodo = nuevoHijo;
  nuevoHijo->derecha = nuevoNietoDerecha;
  nuevoNietoDerecha->izquierda = nuevoIzquierdaNietoDerecha;

  nuevoNietoDerecha->alto -= 2;

  actualizar_max_nodo(nuevoNietoDerecha);
  actualizar_max_nodo(nuevoHijo);
}

void rotacion_simple_derecha(
  ArbolAvlNode** posicionDelNodo,
  ArbolAvlNode* nodo
) {
  ArbolAvlNode* nuevoHijo = nodo->derecha;
  ArbolAvlNode* nuevoNietoIzquierda = nodo;
  ArbolAvlNode* nuevaDerechaNietoIzquierda = nodo->derecha->izquierda;

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
    ArbolAvlNode** posicionDelNodo = (ArbolAvlNode**) deque_pop_front(dequeDireccion);
    ArbolAvlNode* chequear = *posicionDelNodo;

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

    if(-1 <= itree_factor_de_equilibrio(chequear)
       && itree_factor_de_equilibrio(chequear) <= 1) {
      continue;
    }

    if (itree_factor_de_equilibrio(chequear->izquierda) < 0) {
      rotacion_simple_izquierda(posicionDelNodo, chequear);
      break;
    } else if (itree_factor_de_equilibrio(chequear->derecha) > 0) {
      rotacion_simple_derecha(posicionDelNodo, chequear);
      break;
    } else if(itree_factor_de_equilibrio(chequear->izquierda) > 0) {
      rotacion_simple_derecha(&(chequear->izquierda), chequear->izquierda);
      rotacion_simple_izquierda(posicionDelNodo, chequear);
      break;
    } else if(itree_factor_de_equilibrio(chequear->derecha) < 0) {
      rotacion_simple_izquierda(&(chequear->derecha), chequear->derecha);
      rotacion_simple_derecha(posicionDelNodo, chequear);
      break;
    }
  }

  while (!deque_vacio(dequeDireccion)) {
    ArbolAvlNode** chequear = deque_pop_front(dequeDireccion);
    actualizar_max_nodo(*chequear);
  }
}

bool itree_insertar(ArbolAvl *arbol, Rango rango) {
  if(inexistente(rango)) {
    return false;
  }

  //TODO optimizar arbol

  ArbolAvlNode* nodo = calloc(1, sizeof(ArbolAvlNode));
  nodo->rango = rango;
  nodo->maxB = rango.b;
  nodo->alto = 1;

  Deque* dequeDireccion = deque_crear();

  {
    ArbolAvlNode **pos = &(arbol->arbolAvlNode);

    while (*pos != NULL) {
      deque_push_front(dequeDireccion, pos);

      ArbolAvlNode *chequear = *pos;

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

bool itree_eliminar(ArbolAvl *arbol, Rango rango) {
  if(arbol->arbolAvlNode == NULL) {
    return false;
  }

  Deque *dequeDireccion = deque_crear();

  ArbolAvlNode **posicionDelNodoAEliminar;
  ArbolAvlNode *nodoAEliminar;

  {//Rastreo del nodo a eliminar
    ArbolAvlNode **pos = &(arbol->arbolAvlNode);

    while (*pos != NULL) {
      deque_push_front(dequeDireccion, pos);

      ArbolAvlNode *chequear = *pos;

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
      ArbolAvlNode **posicionDelNodoSacado;
      ArbolAvlNode *nuevoHijo;

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

bool existeInterseccion(Rango uno, Rango dos) {
  return (uno.b >= dos.a && dos.b >= uno.a);
}

Rango itree_intersectar(ArbolAvl *tree, Rango rango) {
  ArbolAvlNode* nodo = tree->arbolAvlNode;

  while (nodo != NULL) {
    if(existeInterseccion(nodo->rango, rango)) {
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

void itree_imprimir_arbol(ArbolAvl *arbol) {
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
    ArbolAvlNode* nodo = deque_pop_back(deque);

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

void itree_recorrer_fs_imprimir(
  ArbolAvl *arbol,
  Impresion impresion,
  Popper pop
) {
  itree_recorrer_fs(arbol, impresion, pop);
}

void itree_recorrer_dfs(ArbolAvl *arbol, Impresion impresion) {
  itree_recorrer_fs_imprimir(arbol, impresion, deque_pop_front);
}

void itree_recorrer_bfs(ArbolAvl *arbol, Impresion impresion) {
  itree_recorrer_fs_imprimir(arbol, impresion, deque_pop_back);
}

int itree_factor_de_equilibrio(ArbolAvlNode *nodo) {
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