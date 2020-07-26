#include <malloc.h>
#include "mapa.h"

Mapa* mapa_crear() {
  return calloc(1, sizeof(Mapa));
}

void mapa_destruir(Mapa* mapa) {
  for (int i = 0; i < CANTIDAD_LETRAS; i++)
    if(mapa->elementos[i] != NULL)
      trie_destruir(mapa->elementos[i]);
  free(mapa);
}

void mapa_insertar(Mapa* mapa, unsigned int llave, Trie *valor) {
  mapa->elementos[llave] = valor;
}

Trie* mapa_obtener(Mapa* mapa, unsigned int llave) {
  return mapa->elementos[llave];
}