#ifndef ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H
#define ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H

#include <limits.h>
#include <stdbool.h>
#include "rango.h"

typedef struct _ArbolIntervalosNode {
  Rango rango;
  int maxB;
  int alto;

  struct _ArbolIntervalosNode* derecha;
  struct _ArbolIntervalosNode* izquierda;
} ArbolIntervalosNode;

typedef struct {
  ArbolIntervalosNode* arbolAvlNode;
} ArbolIntervalos;

typedef void Accion(ArbolIntervalosNode*);

// OPERACIONES QUE NO EDITAN LA ENTRADA
ArbolIntervalos * arbolintervalos_crear();

ArbolIntervalos * arbolintervalos_copiar(ArbolIntervalos* arbol);

ArbolIntervalos * arbolintervalos_union(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB);

ArbolIntervalos * arbolintervalos_interseccion(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB);

ArbolIntervalos * arbolintervalos_resta(ArbolIntervalos * arbolA, ArbolIntervalos * arbolB);

ArbolIntervalos * arbolintervalos_complemento(ArbolIntervalos * arbol);

void arbolintervalos_imprimir_arbol(ArbolIntervalos *arbol);

int arbolintervalos_factor_de_equilibrio(ArbolIntervalosNode *nodo);

Rango arbolintervalos_intersectar(ArbolIntervalos* tree, Rango rango);

// OPERACIONES QUE EDITAN LA ENTRADA
void arbolintervalos_destruir(ArbolIntervalos* tree);

bool arbolintervalos_insertar(ArbolIntervalos *arbol, Rango rango);

bool arbolintervalos_eliminar(ArbolIntervalos* arbol, Rango rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H
