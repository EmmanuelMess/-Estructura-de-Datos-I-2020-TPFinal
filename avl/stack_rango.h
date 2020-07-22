#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_STACK_RANGO_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_STACK_RANGO_H

#include <stdbool.h>
#include "rango.h"

//INICIO <-> <-> <-> FINAL
typedef struct StackRangoNode_ {
  Rango rango;
  struct StackRangoNode_* siguente;
  struct StackRangoNode_* anterior;
} StackRangoNode;

typedef struct {
  StackRangoNode* primerNodo;
} StackRango;

StackRango* stackrango_crear();

void stackrango_destruir(StackRango* deque);

bool stackrango_vacio(StackRango *deque);

void stackrango_push(StackRango* deque, Rango elemento);

Rango stackrango_pop(StackRango* deque);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_STACK_RANGO_H
