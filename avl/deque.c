#include <wchar.h>
#include "deque.h"

Deque *deque_crear() {
  return calloc(1, sizeof(Deque));
}

void deque_destruir(Deque *deque) {
  while (deque->primerNodo != NULL) {
    DequeNodo* siguente = deque->primerNodo->siguente;
    free(deque->primerNodo);
    deque->primerNodo = siguente;
  }

  free(deque);
}

bool deque_vacio(Deque *deque) {
  return deque->ultimoNodo == NULL;
}

void * deque_pop_back(Deque *deque) {
  DequeNodo *tmp = deque->ultimoNodo;

  ArbolIntervalosNodo *elemento = deque->ultimoNodo->nodo;
  deque->ultimoNodo = deque->ultimoNodo->anterior;

  if (deque->ultimoNodo != NULL)
    deque->ultimoNodo->siguente = NULL;
  else
    deque->primerNodo = NULL;


  free(tmp);
  return elemento;
}

void * deque_pop_front(Deque *deque) {
  DequeNodo *tmp = deque->primerNodo;

  ArbolIntervalosNodo *elemento = deque->primerNodo->nodo;
  deque->primerNodo = deque->primerNodo->siguente;

  if (deque->primerNodo != NULL)
    deque->primerNodo->anterior = NULL;
  else
    deque->ultimoNodo = NULL;

  free(tmp);
  return elemento;
}

void deque_push_front(Deque *deque, void *elemento) {
  DequeNodo* node = calloc(1, sizeof(DequeNodo));
  node->nodo = elemento;
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

int deque_imprimir(Deque *deque) {
  if(deque->primerNodo == NULL) {
    wprintf(L"Vacio\n");
    return 0;
  }

  for(DequeNodo* inicio = deque->primerNodo; inicio != deque->ultimoNodo;
  inicio = inicio->siguente) {
    ArbolIntervalosNodo *nodo = *((ArbolIntervalosNodo **) inicio->nodo);
    if (nodo == NULL)
      wprintf(L"NULL");
    else
      wprintf(
        L" {m: %d, r: [%d, %d], a: %d}",
        nodo->maximoFinalDeRango,
        nodo->rango.a,
        nodo->rango.b,
        nodo->alto
      );
  }

  ArbolIntervalosNodo *nodo = *((ArbolIntervalosNodo **) deque->ultimoNodo->nodo);

  if (nodo == NULL)
    wprintf(L"NULL");
  else
    wprintf(
      L" {m: %d, r: [%d, %d], a: %d}",
      nodo->maximoFinalDeRango,
      nodo->rango.a,
      nodo->rango.b,
      nodo->alto
    );

  wprintf(L"\n");
  return 0;
}