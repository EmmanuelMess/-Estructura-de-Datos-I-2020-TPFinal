#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAPA_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAPA_H

#include <stdint.h>
#include <stdbool.h>
#include "trie.h"

//[a:z]+[á:ú]+ñ+ü+[Á:Ú]+Ñ+Ü+[A:Z]+[0:9]
#define CANTIDAD_LETRAS (26+5+1+1+5+1+1+26+10)

typedef struct _Mapa {
  struct _Trie *elementos[CANTIDAD_LETRAS];
} Mapa;

Mapa* mapa_crear();
void mapa_destruir(Mapa* mapa);
void mapa_insertar(Mapa *mapa, unsigned int llave, struct _Trie *valor);
struct _Trie* mapa_obtener(Mapa *mapa, unsigned int llave);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_MAPA_H
