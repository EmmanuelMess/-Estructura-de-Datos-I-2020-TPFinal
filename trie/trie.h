#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#include <stdbool.h>
#include "mapa.h"
#include "../avl/arbol_avl.h"

typedef struct _Trie {
  struct _Mapa* mapa;
  bool esFinal;
  ArbolAvl* conjunto;
} Trie;

Trie* trie_crear();

void trie_destruir(Trie* trie);

void trie_agregar(Trie *trie, char *palabra, ArbolAvl *conjunto);

ArbolAvl * trie_chequear(Trie *trie, char *palabra);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
