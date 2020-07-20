#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#include <stdbool.h>
#include <wchar.h>
#include "mapa.h"
#include "../avl/arbol_intervalos.h"

typedef struct Trie_ {
  struct Mapa_* mapa;
  bool esFinal;
  ArbolIntervalos* conjunto;
} Trie;

Trie* trie_crear();

void trie_destruir(Trie* trie);

void trie_agregar(Trie *trie, wchar_t *palabra, ArbolIntervalos *conjunto);

ArbolIntervalos * trie_obtener(Trie *trie, wchar_t *palabra);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
