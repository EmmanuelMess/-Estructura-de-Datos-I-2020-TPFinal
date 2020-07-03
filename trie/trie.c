#include <stdlib.h>
#include "trie.h"
#include "map.h"

Trie* trie_crear() {
  Trie* trie = calloc(1, sizeof(Trie));
  trie->map = mapa_crear();
  return trie;
}

void trie_destruir(Trie* trie) {
  mapa_destruir(trie->map);
  free(trie);
}

static unsigned char reasignar(unsigned char c) {
  switch (c) {
    case 164:
      return 26;
    case 160:
      return 27;
    case 130:
      return 28;
    case 161:
      return 29;
    case 162:
      return 30;
    case 163:
      return 31;
    case 129:
      return 32;
    case ' ':
      return 33;
    default:
      return c - 'a';
  }
}

static bool es_terminador(char c) {
  return c == '\n' || c == '\0' || c == '\r';
}

Trie* trie_hijo(Trie* trie, char pos) {
  return mapa_obtener(trie->map, reasignar(pos));
}

void trie_agregar(Trie* trie, char *palabra) {
  if (es_terminador(palabra[0])) {
    trie->isEnd = true;
  } else {
    Trie *child = trie_hijo(trie, palabra[0]);

    if (child == NULL) {
      mapa_insertar(trie->map, reasignar(palabra[0]), trie_crear());
      child = trie_hijo(trie, palabra[0]);
    }

    trie_agregar(child, palabra + 1);
  }
}

bool trie_chequear(Trie* trie, char *palabra) {
  if(es_terminador(palabra[0])) {
    return trie->isEnd;
  } else {
    Trie *next = trie_hijo(trie, palabra[0]);

    if(next == NULL) return false;
    else return trie_chequear(next, palabra + 1);
  }
}