#ifndef ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H
#define ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H

#include <limits.h>
#include <stdbool.h>

typedef struct {
  int a, b;
} Rango;

#define RANGO_INEXISTENTE ((Rango) {.a = INT_MAX, .b = INT_MIN});

bool inexistente(Rango rango);

typedef struct _ArbolAvlNode {
  Rango rango;
  int maxB;
  int alto;

  struct _ArbolAvlNode* derecha;
  struct _ArbolAvlNode* izquierda;
} ArbolAvlNode;

typedef struct {
  ArbolAvlNode* arbolAvlNode;
} ArbolAvl;

typedef void Accion(ArbolAvlNode*);

// OPERACIONES QUE NO EDITAN LA ENTRADA
ArbolAvl * itree_crear();

ArbolAvl * itree_copiar(ArbolAvl* arbol);

ArbolAvl * itree_union(ArbolAvl * arbolA, ArbolAvl * arbolB);

ArbolAvl * itree_interseccion(ArbolAvl * arbolA, ArbolAvl * arbolB);

ArbolAvl * itree_resta(ArbolAvl * arbolA, ArbolAvl * arbolB);

ArbolAvl * itree_complemento(ArbolAvl * arbol);

void itree_destruir(ArbolAvl* tree);

void itree_imprimir_arbol(ArbolAvl *arbol);

int itree_factor_de_equilibrio(ArbolAvlNode *nodo);

Rango itree_intersectar(ArbolAvl* tree, Rango rango);

// OPERACIONES QUE EDITAN LA ENTRADA
bool itree_insertar(ArbolAvl *arbol, Rango rango);

bool itree_eliminar(ArbolAvl* arbol, Rango rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H
