#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAP_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAP_H

#include <stdint.h>
#include <stdbool.h>
#include "trie.h"

//[a:z]+[á:ú]+ñ+ü+' '
#define CANTIDAD_LETRAS 33

typedef struct _Mapa {
  struct _Trie *elementos[CANTIDAD_LETRAS];
} Mapa;

Mapa* mapa_crear();
void mapa_destruir(Mapa* mapa);
void mapa_insertar(Mapa *mapa, int key, struct _Trie *value);
struct _Trie* mapa_obtener(Mapa *mapa, int pos);
bool mapa_contiene(Mapa *mapa, int key);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAP_H
