#include <string.h>
#include <wchar.h>
#include <assert.h>
#include "arbol_intervalos.h"
#include "deque.h"
#include "matematica.h"

typedef ArbolIntervalosNodo* (Popper(Deque*)) ;
typedef void Accion(ArbolIntervalosNodo*);

/**
 * Alerta: si se modifica el arbol en actuar, la busqueda sigue sobre los nodos
 * no observados, tener cuidado.
 *
 * Recorre un arbol con via breath first search si se le pasa
 * deque_pop_back o depth first search si se le pasa deque_pop_front
 *
 * @param actuar funcion que actua en cada nodo
 * @param pop deque.c:deque_pop_back o deque.c:deque_pop_front
 */
static void recorrer_xfirstsearch(ArbolIntervalos *arbol, Accion actuar, Popper pop) {
  if (arbol->arbolAvlNodo == NULL) return;

  Deque *deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNodo);

  while (!deque_vacio(deque)) {
    ArbolIntervalosNodo *nodo = pop(deque);

    if (nodo->izquierda)
      deque_push_front(deque, nodo->izquierda);
    if (nodo->derecha)
      deque_push_front(deque, nodo->derecha);

    actuar(nodo);
  }

  deque_destruir(deque);
}

ArbolIntervalos *arbolintervalos_crear() {
  ArbolIntervalos* avl = calloc(1, sizeof(ArbolIntervalos));

  return avl;
}

/**
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Copiar no edita el arbol pasado, solo crea una copia exacta,
 * recorriendo nodo por nodo.
 */
static ArbolIntervalosNodo * copiar(ArbolIntervalosNodo * original) {
  if(original == NULL)
    return NULL;

  ArbolIntervalosNodo * copia = malloc(sizeof(ArbolIntervalosNodo));
  memcpy(copia, original, sizeof(ArbolIntervalosNodo));
  copia->izquierda = copiar(original->izquierda);
  copia->derecha = copiar(original->derecha);

  return copia;
}

ArbolIntervalos * arbolintervalos_copiar(ArbolIntervalos * arbol) {
  ArbolIntervalos * copia = arbolintervalos_crear();
  copia->arbolAvlNodo = copiar(arbol->arbolAvlNodo);
  return copia;
}

void arbolintervalos_destruir(ArbolIntervalos *arbol) {
  recorrer_xfirstsearch(arbol, (Accion *) free, (Popper *) deque_pop_back);
  free(arbol);
}


/**
 * Alarta: No es recursivo!
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Dado un nodo, actualiza el maximo final de rango
 */
static void actualizar_max_nodo(ArbolIntervalosNodo* nodo) {
  if (nodo->izquierda == NULL && nodo->derecha == NULL)
    nodo->maximoFinalDeRango = nodo->rango.b;
  else if (nodo->izquierda == NULL)
    nodo->maximoFinalDeRango = nodo->derecha->maximoFinalDeRango;
  else if (nodo->derecha == NULL)
    nodo->maximoFinalDeRango = nodo->izquierda->maximoFinalDeRango;
  else
    nodo->maximoFinalDeRango =
      nodo->izquierda->maximoFinalDeRango > nodo->derecha->maximoFinalDeRango ?
      nodo->izquierda->maximoFinalDeRango : nodo->derecha->maximoFinalDeRango;
}

/**
 * Alarta: No es recursivo!
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Dado un nodo, actualiza el alto del subarbol correspondiente
 */
static void actualizar_alto_nodo(ArbolIntervalosNodo* nodo) {
  if (nodo->izquierda && nodo->derecha)
    nodo->alto = max(nodo->izquierda->alto, nodo->derecha->alto) + 1;
  else if (nodo->izquierda)
    nodo->alto = nodo->izquierda->alto + 1;
  else if (nodo->derecha)
    nodo->alto = nodo->derecha->alto + 1;
  else
    nodo->alto = 1;
}

/**
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Dado un nodo, rota a izquierda (para mantener balanceo del arbol AVL),
 * actualiza los altos y los maximos finales de los nodos movidos
 */
static void rotacion_simple_izquierda(
  ArbolIntervalosNodo** posicionDelNodo,
  ArbolIntervalosNodo* nodo
) {
  ArbolIntervalosNodo* nuevoHijo = nodo->izquierda;
  ArbolIntervalosNodo* nuevoNietoDerecha = nodo;
  ArbolIntervalosNodo* nuevoIzquierdaNietoDerecha = nodo->izquierda->derecha;

  *posicionDelNodo = nuevoHijo;
  nuevoHijo->derecha = nuevoNietoDerecha;
  nuevoNietoDerecha->izquierda = nuevoIzquierdaNietoDerecha;

  nuevoNietoDerecha->alto -= 2;

  actualizar_max_nodo(nuevoNietoDerecha);
  actualizar_alto_nodo(nuevoNietoDerecha);
  actualizar_max_nodo(nuevoHijo);
  actualizar_alto_nodo(nuevoHijo);
}

/**
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Dado un nodo, rota a derecha (para mantener balanceo del arbol AVL),
 * actualiza los altos y los maximos finales de los nodos movidos
 */
static void rotacion_simple_derecha(
  ArbolIntervalosNodo** posicionDelNodo,
  ArbolIntervalosNodo* nodo
) {
  ArbolIntervalosNodo* nuevoHijo = nodo->derecha;
  ArbolIntervalosNodo* nuevoNietoIzquierda = nodo;
  ArbolIntervalosNodo* nuevaDerechaNietoIzquierda = nodo->derecha->izquierda;

  *posicionDelNodo = nuevoHijo;
  nuevoHijo->izquierda = nuevoNietoIzquierda;
  nuevoNietoIzquierda->derecha = nuevaDerechaNietoIzquierda;

  nuevoNietoIzquierda->alto -= 2;

  actualizar_max_nodo(nuevoNietoIzquierda);
  actualizar_alto_nodo(nuevoNietoIzquierda);
  actualizar_max_nodo(nuevoHijo);
  actualizar_alto_nodo(nuevoHijo);
}

/**
 * Alerta: Chequear que en el deque de entrada no haya NULL ni punteros a NULL!
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Dado un Deque con punteros a puntero a nodos en un camino desde la raiz
 * hasta un nodo modificado, se encarga de que los subarboles del camino
 * esten balanceados.
 */
static void rebalancear(
  Deque* dequeDireccion
) {
  bool hizoRotacion = false;

  while (!deque_vacio(dequeDireccion) && !hizoRotacion) {
    ArbolIntervalosNodo** posicionDelNodo = (ArbolIntervalosNodo**) deque_pop_front(dequeDireccion);
    ArbolIntervalosNodo* chequear = *posicionDelNodo;

    actualizar_max_nodo(chequear);

    actualizar_alto_nodo(chequear);

    if (arbolintervalos_factor_equilibrio(chequear) < -1 ||
        1 < arbolintervalos_factor_equilibrio(chequear)) {
      if (arbolintervalos_factor_equilibrio(chequear->izquierda) < 0) {
        rotacion_simple_izquierda(posicionDelNodo, chequear);
        hizoRotacion = true;
      } else if (arbolintervalos_factor_equilibrio(chequear->derecha) > 0) {
        rotacion_simple_derecha(posicionDelNodo, chequear);
        hizoRotacion = true;
      } else if (arbolintervalos_factor_equilibrio(chequear->izquierda) > 0) {
        rotacion_simple_derecha(&(chequear->izquierda), chequear->izquierda);
        rotacion_simple_izquierda(posicionDelNodo, chequear);
        hizoRotacion = true;
      } else if (arbolintervalos_factor_equilibrio(chequear->derecha) < 0) {
        rotacion_simple_izquierda(&(chequear->derecha), chequear->derecha);
        rotacion_simple_derecha(posicionDelNodo, chequear);
        hizoRotacion = true;
      }
    }
  }

  while (!deque_vacio(dequeDireccion)) {
    ArbolIntervalosNodo** chequear = deque_pop_front(dequeDireccion);
    actualizar_max_nodo(*chequear);
    actualizar_alto_nodo(*chequear);
  }
}

ArbolIntervalosNodo* arbolintervalos_insertar(ArbolIntervalos *arbol, Rango rango) {
  if(inexistente(rango))
    return NULL;

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

  ArbolIntervalosNodo* nodo = calloc(1, sizeof(ArbolIntervalosNodo));
  nodo->rango = rango;
  nodo->maximoFinalDeRango = rango.b;
  nodo->alto = 1;

  Deque* dequeDireccion = deque_crear();

  {
    ArbolIntervalosNodo **pos = &(arbol->arbolAvlNodo);

    while (*pos != NULL) {
      deque_push_front(dequeDireccion, pos);

      ArbolIntervalosNodo *chequear = *pos;

      if (rango.a < chequear->rango.a
          || (chequear->rango.a == rango.a && rango.b < chequear->rango.b))
        pos = &((*pos)->izquierda);
      else if (chequear->rango.a < rango.a
               || (chequear->rango.a == rango.a &&
                   chequear->rango.b < rango.b))
        pos = &((*pos)->derecha);
      else {
        free(nodo);
        deque_destruir(dequeDireccion);

        return NULL;
      }
    }

    *pos = nodo;
  }

  rebalancear(dequeDireccion);

  deque_destruir(dequeDireccion);

  return nodo;
}

/**
 * Estructura particular para rastrear_nodo, ver documentacion ahi.
 */
struct NodoAEliminar {
  ArbolIntervalosNodo **posicionDelNodoAEliminar;
  ArbolIntervalosNodo *nodoAEliminar;
};

/**
 * Alerta: Funcion para uso en la eliminacion de nodos,
 * para otras cosas probablemente se necesite una version mas simple.
 * Alerta: Funcion interna, puede no mantener las invariantes
 *
 * Dado un arbol y un rango, lo encuentra en el arbol agregando el camino
 * al {@param dequeDireccion} pasado,
 * en la forma de punteros a punteros de nodos, esto es decir,
 * para cada en el camino, se tiene el puntero al puntero en el padre.
 *
 * Cuando encuentra el nodo, devuelve un puntero a puntero del nodo,
 * y un puntero al nodo, para facilitar el uso para elminar nodos.
 *
 * Si la funcion falla, devuelve false.
 */
static bool rastrear_nodo(struct NodoAEliminar * x, Deque *dequeDireccion, ArbolIntervalos *arbol, Rango rango) {
  if(arbol->arbolAvlNodo == NULL)
    return false;

  ArbolIntervalosNodo **pos = &(arbol->arbolAvlNodo);

  while (*pos != NULL) {
    deque_push_front(dequeDireccion, pos);

    ArbolIntervalosNodo *chequear = *pos;

    if (rango.a < chequear->rango.a
        || (chequear->rango.a == rango.a && rango.b < chequear->rango.b))
      pos = &((*pos)->izquierda);
    else if (chequear->rango.a < rango.a
             || (chequear->rango.a == rango.a &&
                 chequear->rango.b < rango.b))
      pos = &((*pos)->derecha);
    else if (rango.a == (*pos)->rango.a && rango.b == (*pos)->rango.b) {
      x->nodoAEliminar = *pos;
      x->posicionDelNodoAEliminar = pos;
      return true;
    } else {
      return false;
    }
  }

  return false;
}

void arbolintervalos_eliminar(ArbolIntervalos *arbol, Rango rango) {
  if(arbol->arbolAvlNodo == NULL) return;

  Deque *dequeDireccion = deque_crear();

  struct NodoAEliminar aEliminar;

  bool resultado = rastrear_nodo(&aEliminar, dequeDireccion, arbol, rango);

  if(!resultado) {
    deque_destruir(dequeDireccion);
    return;
  }

  ArbolIntervalosNodo ** posicionDelNodoAEliminar = aEliminar.posicionDelNodoAEliminar;
  ArbolIntervalosNodo * nodoAEliminar = aEliminar.nodoAEliminar;

  {//Rastreo del nodo que lo reemplaza
    if (nodoAEliminar->izquierda != NULL && nodoAEliminar->derecha != NULL) {
      ArbolIntervalosNodo **posicionDelNodoSacado;
      ArbolIntervalosNodo *nuevoHijo;

      nuevoHijo = nodoAEliminar->derecha;
      posicionDelNodoSacado = &(nodoAEliminar->derecha);

      deque_push_front(dequeDireccion, posicionDelNodoSacado);

      while (nuevoHijo->izquierda != NULL) {
        posicionDelNodoSacado = &(nuevoHijo->izquierda);
        nuevoHijo = nuevoHijo->izquierda;

        deque_push_front(dequeDireccion, posicionDelNodoSacado);
      }

      nodoAEliminar->rango = nuevoHijo->rango;
      nodoAEliminar->alto = nuevoHijo->alto;
      nodoAEliminar->maximoFinalDeRango = nuevoHijo->maximoFinalDeRango;

      *posicionDelNodoSacado = nuevoHijo->derecha;

      nodoAEliminar = nuevoHijo;
    } else if (nodoAEliminar->izquierda != NULL)
      *posicionDelNodoAEliminar = nodoAEliminar->izquierda;
    else if (nodoAEliminar->derecha != NULL)
      *posicionDelNodoAEliminar = nodoAEliminar->derecha;
    else
      *posicionDelNodoAEliminar = NULL;

    deque_pop_front(dequeDireccion);

    free(nodoAEliminar);
  }

  rebalancear(dequeDireccion);

  deque_destruir(dequeDireccion);
}

Rango arbolintervalos_intersectar(ArbolIntervalos *arbol, Rango rango) {
  ArbolIntervalosNodo *nodo = arbol->arbolAvlNodo;

  while (nodo != NULL) {
    if (rango_intersecan(nodo->rango, rango))
      return nodo->rango;

    if (nodo->maximoFinalDeRango < rango.a)
      return RANGO_INEXISTENTE;

    if (rango.a <= nodo->rango.a) {
      if (nodo->izquierda && rango.a <= nodo->izquierda->maximoFinalDeRango)
        nodo = nodo->izquierda;
      else
        return RANGO_INEXISTENTE;
    } else {
      if (nodo->derecha && rango.a <= nodo->derecha->maximoFinalDeRango)
        nodo = nodo->derecha;
      else
        return RANGO_INEXISTENTE;
    }
  }

  return RANGO_INEXISTENTE;
}

void arbolintervalos_imprimir(ArbolIntervalos *arbol) {
  if(arbol->arbolAvlNodo == NULL) {
    wprintf(L"Vacio\n");
    return;
  }

  unsigned int assumedPos = 1;
  unsigned int nodosEnDeque = 0;

  Deque* deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNodo);
  nodosEnDeque++;

  // Alerta: no es el mejor codigo,
  // pero basicamente usa el hecho de que el arbol es binaria para imprimir
  // cada nivel en una linea nueva.
  unsigned int i = 0;
  for (; nodosEnDeque > 0; i++) {
    ArbolIntervalosNodo* nodo = deque_pop_back(deque);

    if(!nodo) {
      wprintf(L" {    NULL    }");
      deque_push_front(deque, NULL);
      deque_push_front(deque, NULL);
    } else {
      wprintf(
        L" {m: %d, r: [%d, %d], a: %d}",
        nodo->maximoFinalDeRango,
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

int arbolintervalos_factor_equilibrio(ArbolIntervalosNodo *nodo) {
  if (nodo == NULL || (nodo->derecha == NULL && nodo->izquierda == NULL))
    return 0;
  else if (nodo->derecha && nodo->izquierda)
    return nodo->derecha->alto - nodo->izquierda->alto;
  else if (nodo->derecha)
    return nodo->derecha->alto;
  else if (nodo->izquierda)
    return -nodo->izquierda->alto;

  assert(false);
}