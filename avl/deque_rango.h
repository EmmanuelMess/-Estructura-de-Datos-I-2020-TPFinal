#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEQUE_RANGO_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEQUE_RANGO_H

#include <stdbool.h>
#include "rango.h"

//INICIO <-> <-> <-> FINAL
typedef struct _DequeRangoNode {
  Rango rango;
  struct _DequeRangoNode* siguente;
  struct _DequeRangoNode* anterior;
} DequeRangoNode;

typedef struct {
  DequeRangoNode* primerNodo;
  DequeRangoNode* ultimoNodo;
} DequeRango;

DequeRango* dequerango_crear();

void dequerango_destruir(DequeRango* deque);

bool dequerango_vacio(DequeRango *deque);

void dequerango_push_front(DequeRango* deque, Rango elemento);

Rango dequerango_peek_front(DequeRango *deque);

Rango dequerango_pop_front(DequeRango* deque);

Rango dequerango_pop_back(DequeRango* deque);

int dequerango_imprimir(DequeRango *deque);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEQUE_RANGO_H
