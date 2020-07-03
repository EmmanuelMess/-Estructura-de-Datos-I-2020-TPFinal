#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#include <stdbool.h>
#include "map.h"

typedef struct _Trie {
  struct _Mapa* map;
  bool isEnd;
} Trie;

Trie* trie_crear();
void trie_destruir(Trie* trie);
void trie_agregar(Trie *trie, char *palabra);
bool trie_chequear(Trie *trie, char *palabra);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
