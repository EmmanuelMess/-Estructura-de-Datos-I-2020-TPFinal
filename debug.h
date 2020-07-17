#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEBUG_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEBUG_H

#include <stddef.h>
#include <stdlib.h>

#include "avl/arbol_intervalos.h"
#include "parseador.h"
#include "trie/trie.h"
#include "tests.h"

/*
 * Archivo de debugeo, contiene funciones que en la version final
 * no tienen ningun codigo.
 */

#define DEBUG true

void debug_main(int argc, char *argv[]) {
#if DEBUG
  if (argc == 2) {
    main_tests();

    exit(0);
  }
#endif
}

void debug0(ArbolIntervalos *arbol) {
#if DEBUG
  arbolintervalos_imprimir(arbol);
#endif
}

void debug1(wchar_t * alias, Metadatos metadatos, int* enteros) {
#if DEBUG
  wprintf(L"DEBUG INFO\n");
  wprintf(L"Parser: %s ", alias);

  for (int i = 0; i < metadatos.largo; ++i) {
    wprintf(L"%d ", enteros[i]);
  }
#endif
}

void debug2(wchar_t * alias, Rango rango) {
#if DEBUG
  wprintf(L"DEBUG INFO\n");
  wprintf(L"Parser: %s ", alias);

  wprintf(L"%d:%d", rango.a, rango.b);
#endif
}

void debug3(wchar_t * alias, Trie * trie) {
#if DEBUG
  wprintf(L"\n");

  wprintf(L"Trie: %ls: %ls\n", alias,
          trie_obtener(trie, alias) != NULL ? L"encotrado" : L"error");
#endif
}

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_DEBUG_H
