#include "tests.h"
#include <assert.h>
#include "tests.h"
#include "avl/arbol_intervalos.h"
#include "trie/trie.h"

void test_trie() {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, (Rango) {.a = 15, .b = 15});
  arbolintervalos_insertar(arbol, (Rango) {.a = -5, .b = 0});
  arbolintervalos_insertar(arbol, (Rango) {.a = 0, .b = 10});
  arbolintervalos_insertar(arbol, (Rango) {.a = 8, .b = 10});
  arbolintervalos_insertar(arbol, (Rango) {.a = 10, .b = 15});

  Trie *trie = trie_crear();
  trie_agregar(trie, "hola", arbol);
  assert(trie_obtener(trie, "hola"));
  assert(!trie_obtener(trie, "hol"));
  trie_destruir(trie);
}

ArbolIntervalos * crear_arbol_intervalo(Rango rango) {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, rango);
  return arbol;
}

void unir_en_trie(Trie * trie, char* alias, char* aliasA, char* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_union(arbolA, arbolB));
}

void intersectar_en_trie(Trie * trie, char* alias, char* aliasA, char* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_interseccion(arbolA, arbolB));
}

void resta_en_trie(Trie * trie, char* alias, char* aliasA, char* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_resta(arbolA, arbolB));
}

void complemento_en_trie(Trie * trie, char* alias, char* aliasA) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  trie_agregar(trie, alias, arbolintervalos_complemento(arbolA));
}

void main_tests() {
  test_trie();

  ArbolIntervalos * arbol1 = crear_arbol_intervalo((Rango) {.a = 15, .b = 15});
  ArbolIntervalos * arbol2 = crear_arbol_intervalo((Rango) {.a = -5, .b = 0});
  ArbolIntervalos * arbol3 = crear_arbol_intervalo((Rango) {.a = 0, .b = 10});
  ArbolIntervalos * arbol4 = crear_arbol_intervalo((Rango) {.a = 8, .b = 10});
  ArbolIntervalos * arbol5 = crear_arbol_intervalo((Rango) {.a = 10, .b = 15});
  ArbolIntervalos * resultado;

  Trie *trie = trie_crear();
  trie_agregar(trie, "hola1", arbol1);
  trie_agregar(trie, "hola2", arbol2);
  trie_agregar(trie, "hola3", arbol3);
  trie_agregar(trie, "hola4", arbol4);
  trie_agregar(trie, "hola5", arbol5);

  unir_en_trie(trie, "a", "hola1", "hola2");
  intersectar_en_trie(trie, "b", "hola1", "hola2");
  intersectar_en_trie(trie, "c", "hola2", "hola3");
  complemento_en_trie(trie, "d", "hola5");

  resultado = trie_obtener(trie, "a");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 15, .b = 15})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = -5, .b = 0})));

  resultado = trie_obtener(trie, "b");
  assert(!resultado->arbolAvlNode);

  resultado = trie_obtener(trie, "c");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 0, .b = 0})));

  resultado = trie_obtener(trie, "d");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = INT_MIN, .b = -1})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 11, .b = INT_MAX})));

  trie_destruir(trie);
}
