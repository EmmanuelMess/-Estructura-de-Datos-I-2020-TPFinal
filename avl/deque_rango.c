#include <stddef.h>
#include <malloc.h>
#include "deque_rango.h"

DequeRango *dequerango_crear() {
  return calloc(1, sizeof(DequeRango));
}

void dequerango_destruir(DequeRango *deque) {
  while (deque->primerNodo != NULL) {
    DequeRangoNode* siguente = deque->primerNodo->siguente;
    free(deque->primerNodo);
    deque->primerNodo = siguente;
  }

  free(deque);
}

bool dequerango_vacio(DequeRango *deque) {
  return deque->ultimoNodo == NULL;
}

Rango dequerango_pop_back(DequeRango *deque) {
  DequeRangoNode *tmp = deque->ultimoNodo;

  Rango elemento = deque->ultimoNodo->rango;
  deque->ultimoNodo = deque->ultimoNodo->anterior;

  if (deque->ultimoNodo != NULL)
    deque->ultimoNodo->siguente = NULL;
  else
    deque->primerNodo = NULL;


  free(tmp);
  return elemento;
}

Rango dequerango_pop_front(DequeRango *deque) {
  DequeRangoNode *tmp = deque->primerNodo;

  Rango elemento = deque->primerNodo->rango;
  deque->primerNodo = deque->primerNodo->siguente;

  if (deque->primerNodo != NULL)
    deque->primerNodo->anterior = NULL;
  else
    deque->ultimoNodo = NULL;

  free(tmp);
  return elemento;
}

Rango dequerango_peek_front(DequeRango *deque) {
  return deque->primerNodo->rango;
}

void dequerango_push_front(DequeRango *deque, Rango elemento) {
  DequeRangoNode* node = calloc(1, sizeof(DequeRangoNode));
  node->rango = elemento;
  node->siguente = deque->primerNodo;
  node->anterior = NULL;

  if(deque->primerNodo == NULL) {
    deque->primerNodo = node;
    deque->ultimoNodo = node;
  } else {
    deque->primerNodo->anterior = node;
    deque->primerNodo = node;
  }
}
