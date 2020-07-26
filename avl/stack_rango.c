#include <stddef.h>
#include <malloc.h>
#include "stack_rango.h"

StackRango *stackrango_crear() {
  return calloc(1, sizeof(StackRango));
}

void stackrango_destruir(StackRango *deque) {
  while (deque->primerNodo != NULL) {
    StackRangoNodo* siguente = deque->primerNodo->siguente;
    free(deque->primerNodo);
    deque->primerNodo = siguente;
  }

  free(deque);
}

bool stackrango_vacio(StackRango *deque) {
  return deque->primerNodo == NULL;
}

Rango stackrango_pop(StackRango *deque) {
  StackRangoNodo *tmp = deque->primerNodo;

  Rango elemento = deque->primerNodo->rango;
  deque->primerNodo = deque->primerNodo->siguente;

  free(tmp);
  return elemento;
}

void stackrango_push(StackRango *deque, Rango elemento) {
  StackRangoNodo* node = calloc(1, sizeof(StackRangoNodo));
  node->rango = elemento;
  node->siguente = deque->primerNodo;

  deque->primerNodo = node;
}
