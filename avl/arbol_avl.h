#ifndef ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H
#define ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H

#include <limits.h>
#include <stdbool.h>

struct Rango {
  int a, b;
};

#define RANGO_INEXISTENTE ((struct Rango) {.a = INT_MAX, .b = INT_MIN});

bool inexistente(struct Rango rango);

typedef struct _ArbolAvlNode {
  struct Rango rango;
  int maxB;
  int alto;

  struct _ArbolAvlNode* derecha;
  struct _ArbolAvlNode* izquierda;
} ArbolAvlNode;

typedef struct {
  ArbolAvlNode* arbolAvlNode;
} ArbolAvl;

typedef void Accion(ArbolAvlNode*);

typedef void Impresion(ArbolAvlNode*);

ArbolAvl* itree_crear();

void itree_destruir(ArbolAvl* tree);

bool itree_insertar(ArbolAvl *arbol, struct Rango rango);

bool itree_eliminar(ArbolAvl* arbol, struct Rango rango);

struct Rango itree_intersectar(ArbolAvl* tree, struct Rango rango);

void itree_recorrer_dfs(ArbolAvl *arbol, Impresion impresion);

void itree_recorrer_bfs(ArbolAvl *arbol, Impresion impresion);

void itree_imprimir_arbol(ArbolAvl *arbol);

int itree_factor_de_equilibrio(ArbolAvlNode *nodo);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TP2_ARBOL_AVL_H
