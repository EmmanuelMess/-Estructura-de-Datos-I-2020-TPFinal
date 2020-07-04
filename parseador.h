#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H

#include "avl/arbol_avl.h"

typedef struct {
  int error;
  int largoAlias;
  bool esExtension;
  int largo;
  bool salir;
} Metadatos;

void remover_espacios(char* entrada);
Metadatos chequeador(char * entrada);
bool procesar(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
