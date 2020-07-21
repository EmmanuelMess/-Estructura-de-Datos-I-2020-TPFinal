#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_H
#include <limits.h>
#include <stdbool.h>
#include "rango.h"

typedef struct ArbolIntervalosNode_ {
  Rango rango;
  int maxB;
  int alto;

  struct ArbolIntervalosNode_* derecha;
  struct ArbolIntervalosNode_* izquierda;
} ArbolIntervalosNode;

typedef struct {
  ArbolIntervalosNode* arbolAvlNode;
} ArbolIntervalos;

typedef void Accion(ArbolIntervalosNode*);

// OPERACIONES QUE NO EDITAN LA ENTRADA
ArbolIntervalos * arbolintervalos_crear();

ArbolIntervalos * arbolintervalos_copiar(ArbolIntervalos* arbol);

void arbolintervalos_imprimir(ArbolIntervalos *arbol);

int arbolintervalos_factor_equilibrio(ArbolIntervalosNode *nodo);

Rango arbolintervalos_intersectar(ArbolIntervalos* tree, Rango rango);

// OPERACIONES QUE EDITAN LA ENTRADA
void arbolintervalos_destruir(ArbolIntervalos* tree);

bool arbolintervalos_insertar(ArbolIntervalos *arbol, Rango rango);

bool arbolintervalos_eliminar(ArbolIntervalos* arbol, Rango rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_H
