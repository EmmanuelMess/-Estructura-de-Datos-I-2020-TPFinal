#include <malloc.h>
#include <stdbool.h>
#include "map.h"

Mapa* mapa_crear() {
  return calloc(1, sizeof(Mapa));
}

void mapa_destruir(Mapa* mapa) {
  for (int i = 0; i < CANTIDAD_LETRAS; ++i) {
    trie_destruir(mapa->elementos[i]);
  }
  free(mapa);
}

void mapa_insertar(Mapa* mapa, int key, Trie *value) {
  mapa->elementos[key] = value;
}

bool mapa_contiene(Mapa* mapa, int key) {
  return mapa->elementos[key] != NULL;
}

Trie* mapa_obtener(Mapa* mapa, int pos) {
  return mapa->elementos[pos];
}