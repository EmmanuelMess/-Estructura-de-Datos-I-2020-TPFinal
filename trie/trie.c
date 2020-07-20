#include <stdlib.h>
#include <assert.h>
#include "trie.h"
#include "mapa.h"

Trie* trie_crear() {
  Trie* trie = calloc(1, sizeof(Trie));
  trie->mapa = mapa_crear();
  return trie;
}

void trie_destruir(Trie* trie) {
  if(trie->conjunto != NULL)
    arbolintervalos_destruir(trie->conjunto);

  if(trie->mapa != NULL)
    mapa_destruir(trie->mapa);

  free(trie);
}

static unsigned int reasignar(wchar_t c) {
  if('a' <= c && c <= 'z') return c - 'a';

  switch (c) {
    case L'ñ':
      return 26;
    case L'ü':
      return 27;
    case L'á':
      return 28;
    case L'é':
      return 29;
    case L'í':
      return 30;
    case L'ó':
      return 31;
    case L'ú':
      return 32;
    case L'Ñ':
      return 33;
    case L'Ü':
      return 34;
    case L'Á':
      return 35;
    case L'É':
      return 36;
    case L'Í':
      return 37;
    case L'Ó':
      return 38;
    case L'Ú':
      return 39;
  }

  if('A' <= c && c <= 'Z') return 40 + c - 'A';
  if('0' <= c && c <= '9') return 66 + c - '0';

  return 0;//mapeo el resto a 'a' para evitar crashes
}

static bool es_terminador(wchar_t c) {
  return c == L'\n' || c == L'\0' || c == L'\r';
}

Trie* trie_hijo(Trie* trie, wchar_t pos) {
  return mapa_obtener(trie->mapa, reasignar(pos));
}

void trie_agregar(Trie *trie, wchar_t *palabra, ArbolIntervalos *conjunto) {
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

ArbolIntervalos * trie_obtener(Trie* trie, wchar_t * palabra) {
  if(es_terminador(palabra[0])) {
    if(trie->esFinal) return trie->conjunto;
    else return NULL;
  } else {
    Trie *next = trie_hijo(trie, palabra[0]);

    if(next == NULL) return false;
    else return trie_obtener(next, palabra + 1);
  }
}