#include <stddef.h>
#include <malloc.h>
#include "stack_rango.h"

StackRango *stackrango_crear() {
  return calloc(1, sizeof(StackRango));
}

void stackrango_destruir(StackRango *deque) {
  while (deque->primerNodo != NULL) {
    StackRangoNode* siguente = deque->primerNodo->siguente;
    free(deque->primerNodo);
    deque->primerNodo = siguente;
  }

  free(deque);
}

bool stackrango_vacio(StackRango *deque) {
  return deque->primerNodo == NULL;
}

Rango stackrango_pop(StackRango *deque) {
  StackRangoNode *tmp = deque->primerNodo;

  Rango elemento = deque->primerNodo->rango;
  deque->primerNodo = deque->primerNodo->siguente;

  if (deque->primerNodo != NULL)
    deque->primerNodo->anterior = NULL;

  free(tmp);
  return elemento;
}

void stackrango_push(StackRango *deque, Rango elemento) {
  StackRangoNode* node = calloc(1, sizeof(StackRangoNode));
  node->rango = elemento;
  node->siguente = deque->primerNodo;
  node->anterior = NULL;

  if(deque->primerNodo == NULL) {
    deque->primerNodo = node;
  } else {
    deque->primerNodo->anterior = node;
    deque->primerNodo = node;
  }
}
