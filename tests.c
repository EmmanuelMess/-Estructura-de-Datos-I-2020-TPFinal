#include "tests.h"
#include <assert.h>
#include <malloc.h>
#include "tests.h"
#include "avl/arbol_intervalos_extras.h"
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
  trie_agregar(trie, L"hola", arbol);
  assert(trie_obtener(trie, L"hola"));
  assert(!trie_obtener(trie, L"hol"));
  trie_destruir(trie);
}

ArbolIntervalos * crear_arbol_intervalo(Rango rango) {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, rango);
  return arbol;
}

ArbolIntervalos * crear_arbol_elemento(int elem) {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, (Rango) {.a = elem, .b = elem});
  return arbol;
}

void unir_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA, wchar_t* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_union(arbolA, arbolB));
}

void intersectar_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA, wchar_t* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_interseccion(arbolA, arbolB));
}

void resta_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA, wchar_t* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_resta(arbolA, arbolB));
}

void complemento_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  trie_agregar(trie, alias, arbolintervalos_complemento(arbolA));
}

void uniones_tests_completo() {
  ArbolIntervalos * arbolPares = arbolintervalos_crear();

  for (int i = -1000; i <= 1000; i += 2) {
    ArbolIntervalos * arbol1 = crear_arbol_elemento(i);
    ArbolIntervalos * tmp = arbolintervalos_union(arbolPares, arbol1);
    arbolintervalos_destruir(arbolPares);
    arbolintervalos_destruir(arbol1);
    arbolPares = tmp;
  }

  ArbolIntervalos * arbolImpares = arbolintervalos_crear();

  for (int i = -999; i <= 1000; i += 2) {
    ArbolIntervalos * arbol1 = crear_arbol_elemento(i);
    ArbolIntervalos * tmp = arbolintervalos_union(arbolImpares, arbol1);
    arbolintervalos_destruir(arbolImpares);
    arbolintervalos_destruir(arbol1);
    arbolImpares = tmp;
  }

  ArbolIntervalos * todo = arbolintervalos_union(arbolImpares, arbolPares);
  arbolintervalos_destruir(arbolPares);
  arbolintervalos_destruir(arbolImpares);

  assert(todo->arbolAvlNode->rango.a == -1000);
  assert(todo->arbolAvlNode->rango.b == 1000);
  assert(todo->arbolAvlNode->derecha == NULL);
  assert(todo->arbolAvlNode->izquierda == NULL);

  arbolintervalos_destruir(todo);
}

void uniones_tests() {
  ArbolIntervalos * arbol1 = crear_arbol_elemento(15);
  ArbolIntervalos * arbol2 = crear_arbol_elemento(17);
  ArbolIntervalos * arbol3 = crear_arbol_elemento(19);
  ArbolIntervalos * arbol4 = crear_arbol_elemento(21);
  ArbolIntervalos * arbol5 = crear_arbol_elemento(23);

  ArbolIntervalos * arbol = arbolintervalos_crear();
  ArbolIntervalos * arbolTmp1 = arbolintervalos_union(arbol, arbol1);
  ArbolIntervalos * arbolTmp2 = arbolintervalos_union(arbolTmp1, arbol2);
  ArbolIntervalos * arbolTmp3 = arbolintervalos_union(arbolTmp2, arbol3);
  ArbolIntervalos * arbolTmp4 = arbolintervalos_union(arbolTmp3, arbol4);
  ArbolIntervalos * arbolTmp5 = arbolintervalos_union(arbolTmp4, arbol5);

  arbolintervalos_destruir(arbol);
  arbolintervalos_destruir(arbol1);
  arbolintervalos_destruir(arbol2);
  arbolintervalos_destruir(arbol3);
  arbolintervalos_destruir(arbol4);
  arbolintervalos_destruir(arbol5);
  arbolintervalos_destruir(arbolTmp1);
  arbolintervalos_destruir(arbolTmp2);
  arbolintervalos_destruir(arbolTmp3);
  arbolintervalos_destruir(arbolTmp4);
  arbolintervalos_destruir(arbolTmp5);
}

Rango elem(int elemento) {
  return (Rango) {.a = elemento, .b = elemento};
}

void agregar_tests() {
  ArbolIntervalos * arbol = crear_arbol_elemento(15);
  arbolintervalos_insertar(arbol, elem(17));
  arbolintervalos_insertar(arbol, elem(19));
  arbolintervalos_insertar(arbol, elem(21));
  arbolintervalos_insertar(arbol, elem(23));

  arbolintervalos_destruir(arbol);
}

void eliminar_tests() {
  ArbolIntervalos * arbol = crear_arbol_elemento(15);
  arbolintervalos_insertar(arbol, elem(17));
  arbolintervalos_insertar(arbol, elem(19));
  arbolintervalos_insertar(arbol, elem(21));
  arbolintervalos_insertar(arbol, elem(23));

  arbolintervalos_eliminar(arbol, elem(21));
  arbolintervalos_eliminar(arbol, elem(17));
  arbolintervalos_eliminar(arbol, elem(19));
  arbolintervalos_eliminar(arbol, elem(23));
  arbolintervalos_eliminar(arbol, elem(15));
  assert(arbol->arbolAvlNode == NULL);

  arbolintervalos_destruir(arbol);
}

void main_tests() {
  //basicos arbol
  agregar_tests();
  eliminar_tests();

  //extras arbol
  uniones_tests();
  uniones_tests_completo();

  //trie
  test_trie();

  //completo
  ArbolIntervalos * arbol1 = crear_arbol_intervalo((Rango) {.a = 15, .b = 15});
  ArbolIntervalos * arbol2 = crear_arbol_intervalo((Rango) {.a = -5, .b = 0});
  ArbolIntervalos * arbol3 = crear_arbol_intervalo((Rango) {.a = 0, .b = 10});
  ArbolIntervalos * arbol4 = crear_arbol_intervalo((Rango) {.a = 8, .b = 10});
  ArbolIntervalos * arbol5 = crear_arbol_intervalo((Rango) {.a = 10, .b = 15});
  ArbolIntervalos * resultado;

  Trie *trie = trie_crear();
  trie_agregar(trie, L"hola1", arbol1);
  trie_agregar(trie, L"hola2", arbol2);
  trie_agregar(trie, L"hola3", arbol3);
  trie_agregar(trie, L"hola4", arbol4);
  trie_agregar(trie, L"hola5", arbol5);

  unir_en_trie(trie, L"a", L"hola1", L"hola2");
  intersectar_en_trie(trie, L"b", L"hola1", L"hola2");
  intersectar_en_trie(trie, L"c", L"hola2", L"hola3");
  complemento_en_trie(trie, L"d", L"hola5");

  resultado = trie_obtener(trie, L"a");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 15, .b = 15})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = -5, .b = 0})));

  resultado = trie_obtener(trie, L"b");
  assert(!resultado->arbolAvlNode);

  resultado = trie_obtener(trie, L"c");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 0, .b = 0})));

  resultado = trie_obtener(trie, L"d");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = INT_MIN, .b = -1})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 11, .b = INT_MAX})));

  trie_destruir(trie);


}
