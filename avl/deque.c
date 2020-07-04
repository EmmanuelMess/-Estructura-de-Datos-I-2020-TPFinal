#include "deque.h"

Deque *deque_crear() {
  return calloc(1, sizeof(Deque));
}

void deque_destruir(Deque *deque) {
  while (deque->primerNodo != NULL) {
    DequeNode* siguente = deque->primerNodo->siguente;
    free(deque->primerNodo);
    deque->primerNodo = siguente;
  }

  free(deque);
}

bool deque_vacio(Deque *deque) {
  return deque->ultimoNodo == NULL;
}

void * deque_pop_back(Deque *deque) {
  DequeNode* tmp = deque->ultimoNodo;

  ArbolAvlNode* elemento = deque->ultimoNodo->arbolAvl;
  deque->ultimoNodo = deque->ultimoNodo->anterior;

  if(deque->ultimoNodo != NULL) {
    deque->ultimoNodo->siguente = NULL;
  } else {
    deque->primerNodo = NULL;
  }

  free(tmp);
  return elemento;
}

void * deque_pop_front(Deque *deque) {
  DequeNode* tmp = deque->primerNodo;

  ArbolAvlNode* elemento = deque->primerNodo->arbolAvl;
  deque->primerNodo = deque->primerNodo->siguente;

  if(deque->primerNodo != NULL) {
    deque->primerNodo->anterior = NULL;
  } else {
    deque->ultimoNodo = NULL;
  }

  free(tmp);
  return elemento;
}

void * deque_peek_front(Deque *deque) {
  return deque->primerNodo->arbolAvl;
}

void deque_push_front(Deque *deque, void *elemento) {
  DequeNode* node = calloc(1, sizeof(DequeNode));
  node->arbolAvl = elemento;
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

int deque_largo(Deque *deque) {
  if(deque->primerNodo == NULL) {
    return 0;
  }

  int resultado = 1;
  for(DequeNode* inicio = deque->primerNodo; inicio != deque->ultimoNodo; inicio = inicio->siguente) {
    resultado++;
  }
  return resultado;
}