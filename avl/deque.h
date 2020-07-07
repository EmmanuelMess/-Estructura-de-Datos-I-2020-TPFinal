#ifndef ESTRUCTURA_DE_DATOS_I_2020_TP2_DEQUE_H
#define ESTRUCTURA_DE_DATOS_I_2020_TP2_DEQUE_H

#include <malloc.h>
#include "arbol_intervalos.h"

//INICIO <-> <-> <-> FINAL
typedef struct _DequeNode {
  void* arbolAvl;
  struct _DequeNode* siguente;
  struct _DequeNode* anterior;
} DequeNode;

typedef struct {
  DequeNode* primerNodo;
  DequeNode* ultimoNodo;
} Deque;

Deque* deque_crear();

void deque_destruir(Deque* deque);

bool deque_vacio(Deque *deque);

void deque_push_front(Deque* deque, void *elemento);

void * deque_peek_front(Deque *deque);

void * deque_pop_front(Deque* deque);

void * deque_pop_back(Deque* deque);

int deque_largo(Deque *deque);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TP2_DEQUE_H
