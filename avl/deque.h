#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEQUE_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEQUE_H

#include <malloc.h>
#include "arbol_intervalos.h"

/**
 * Nodo de una lista doblemente enlazada
 * Esta lista se usa para el arbol,
 * es usada con ArbolIntervalosNodo* o ArbolIntervalosNodo**,
 * en el primer caso es para representar caminos
 * y en el segundo para representar caminos donde los nodos son reemplazables,
 * ver arbol_intervalos.h:arbolintervalos_eliminar
 */
typedef struct DequeNodo_ {
  /**
   * Puede ser un puntero a nodo o puntero a puntero a nodo,
   * dependiente del uso
   */
  void* nodo;
  struct DequeNodo_* siguente;
  struct DequeNodo_* anterior;
} DequeNodo;

/**
 * Lista doblemente enlazada
 * Diseñada para poder usarla de pila y cola
 */
typedef struct {
  DequeNodo* primerNodo;
  DequeNodo* ultimoNodo;
} Deque;

/**
 * Crea un nuevo deque vacio
 */
Deque* deque_crear();

/**
 * Destruir deque, no libera los punteros en los nodos
 */
void deque_destruir(Deque* deque);

/**
 * Devuelve true sii el deque esta vacio
 */
bool deque_vacio(Deque *deque);

/**
 * Agrega un elemento adelante del deque
 */
void deque_push_front(Deque* deque, void *elemento);

/**
 * Elimina el primer elemento del deque
 */
void * deque_pop_front(Deque* deque);

/**
 * Elimina el ultimo elemento del deque
 */
void * deque_pop_back(Deque* deque);

/**
 * Alerta: solo funciona si los punteros son a ArbolIntervalosNodo
 *
 * Muesta el deque en orden
 */
int deque_imprimir(Deque *deque);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEQUE_H
