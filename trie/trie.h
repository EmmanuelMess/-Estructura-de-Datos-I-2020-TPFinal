#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
#include <stdbool.h>
#include <wchar.h>
#include "mapa.h"
#include "../avl/arbol_intervalos.h"

/**
 * El trie en una estructura que guarda una relacion llave-contenido eficientemente
 * para llaves que son strings.
 *
 * Este trie soporta los caracteres alphanumericos (a a Z, ñ y Ñ, 0 a 9),
 * las vocales acentuadas con ´ y la ü, y las mayusculas correspondientes.
 * Para mas informacion ver mapa.c
 *
 * Este trie esta diseñado para funcionar con la estructura ArbolIntervalos,
 * es decir, guarda solo punteros a ArbolIntervalos como contenido
 */

/**
 * Estructura que guarda informacion sobre una letra de una palabra.
 * Esta estructura trabaja con la estructura Mapa para crear una cadena de
 * estructuras que permiten almacenar cadenas de caracteres de largo arbitrario
 */
typedef struct Trie_ {
  /**
   * Ver Mapa
   */
  struct Mapa_* mapa;
  /**
   * Indica que el caracter representado por esta estructura
   * es final para algun alias
   */
  bool esFinal;
  /**
   * Esto es asignado solo si esFinal es true, y representa el arbol almacenado
   * para la llave
   */
  ArbolIntervalos* conjunto;
} Trie;

/**
 * Crea un nuevo trie vacio, ver Trie para documentacion
 */
Trie* trie_crear();

/**
 * Aviso: Destruye el trie _y todos los arboles adentro_!
 */
void trie_destruir(Trie* trie);

/**
 * Aviso:  el arbol agregado no puede eliminarse:
 *   Se genera un dangling pointer
 *   El trie no se puede destruir hasta no reasignar cada dangling pointer con
 *   NULL o se genera un double free de los arboles (ver trie_destruir)
 *
 * Agrega un arbol al trie
 * Se puede agregar NULL
 */
void trie_agregar(Trie *trie, wchar_t *palabra, ArbolIntervalos *conjunto);

/**
 * Para un alias agregado con trie_agregar
 * devuelve su arbol correspondiente (puede ser NULL)
 * Para un alias no agregado con trie_agregar devuelve NULL
 */
ArbolIntervalos * trie_obtener(Trie *trie, wchar_t *palabra);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_TRIE_H
