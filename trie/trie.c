#include <stdlib.h>
#include "trie.h"
#include "mapa.h"
#include "../avl/arbol_intervalos.h"

Trie* trie_crear() {
  Trie* trie = calloc(1, sizeof(Trie));
  trie->mapa = mapa_crear();
  return trie;
}

void trie_destruir(Trie* trie) {
  if(trie->conjunto != NULL) {
    arbolintervalos_destruir(trie->conjunto);
  }
  if(trie->mapa != NULL) {
    mapa_destruir(trie->mapa);
  }
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
      if('a' <= c && c <= 'z') return c - 'a';
      if('A' <= c && c <= 'Z') return 34 + c - 'A';
      if('0' <= c && c <= '9') return 60 + c - '0';
  }
}

static bool es_terminador(char c) {
  return c == '\n' || c == '\0' || c == '\r';
}

Trie* trie_hijo(Trie* trie, char pos) {
  return mapa_obtener(trie->mapa, reasignar(pos));
}

void trie_agregar(Trie *trie, char *palabra, ArbolIntervalos *conjunto) {
  if (es_terminador(palabra[0])) {
    trie->esFinal = true;
    trie->conjunto = conjunto;
  } else {
    Trie *hijo = trie_hijo(trie, palabra[0]);

    if (hijo == NULL) {
      mapa_insertar(trie->mapa, reasignar(palabra[0]), trie_crear());
      hijo = trie_hijo(trie, palabra[0]);
    }

    trie_agregar(hijo, palabra + 1, conjunto);
  }
}

ArbolIntervalos * trie_obtener(Trie* trie, char *palabra) {
  if(es_terminador(palabra[0])) {
    if(trie->esFinal) return trie->conjunto;
    else return NULL;
  } else {
    Trie *next = trie_hijo(trie, palabra[0]);

    if(next == NULL) return false;
    else return trie_obtener(next, palabra + 1);
  }
}