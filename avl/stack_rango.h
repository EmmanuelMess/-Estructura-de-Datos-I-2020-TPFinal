#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_STACK_RANGO_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_STACK_RANGO_H

#include <stdbool.h>
#include "rango.h"

/**
 * Nodo para el stack de Rango s,
 * solo tiene una referencia al siguiente nodo
 */
typedef struct StackRangoNodo_ {
  Rango rango;
  struct StackRangoNodo_* siguente;
} StackRangoNodo;

/**
 * Referencia glorificada al primer nodo de una lista simplemente enlazada,
 * que representa el stack de Rangos
 */
typedef struct {
  StackRangoNodo* primerNodo;
} StackRango;

/**
 * Crea un stack de Rangos, vacio
 */
StackRango* stackrango_crear();

/**
 * Destruye el stack de rangos
 */
void stackrango_destruir(StackRango* deque);

/**
 * Devuelve true sii el stack esta vacio
 */
bool stackrango_vacio(StackRango *deque);

/**
 * Agrega al stack
 */
void stackrango_push(StackRango* deque, Rango elemento);

/**
 * Saca del stack y devuelve
 */
Rango stackrango_pop(StackRango* deque);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_STACK_RANGO_H
