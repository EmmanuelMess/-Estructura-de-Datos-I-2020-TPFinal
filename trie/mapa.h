#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAPA_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAPA_H

#include <stdint.h>
#include <stdbool.h>
#include "trie.h"

/**
 * ver Mapa (abajo)
 * Cantidad de Trie almacenables en cada Mapa,
 * [a:z]+[á:ú]+ñ+ü+[Á:Ú]+Ñ+Ü+[A:Z]+[0:9]
 */
#define CANTIDAD_LETRAS (26+5+1+1+5+1+1+26+10)

/**
 * Es el pegamento que une Trie s (ver Trie en trie.h)
 * Es una relacion llave-valor, para el cual la llave es un entero
 * (generado con trie.c:reasignar) y los valores son Trie
 */
typedef struct Mapa_ {
  struct Trie_ *elementos[CANTIDAD_LETRAS];
} Mapa;

/**
 * Crea un nuevo mapa, vacio
 */
Mapa* mapa_crear();

/**
 * Alerta: no usar fuera de trie.c, puede generar inconsistencias graves
 *
 * Destruye un mapa
 */
void mapa_destruir(Mapa* mapa);

/**
 * Inserta un par llave-valor al mapa, ver Mapa
 */
void mapa_insertar(Mapa *mapa, unsigned int llave, struct Trie_ *valor);

/**
 * Obtiene el Trie para un valor, ver Mapa
 */
struct Trie_* mapa_obtener(Mapa *mapa, unsigned int llave);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAPA_H
