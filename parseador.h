#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H

#include "avl/arbol_avl.h"

typedef struct {
  //general
  int error;

  //parseo de asignacion
  int largoAlias;
  bool esExtension;
  int largo;

  //ordenes
  bool imprimir;
  bool salir;
} Metadatos;

/**
 * Error general de parseo
 */
#define METADATOS_ERROR_NUM 1

/**
 * Error en los argumentos de imprimir
 */
#define METADATOS_ERROR_ARG_IMPRIMIR_NUM 10

void remover_espacios(char* entrada);
Metadatos chequeador(char * entrada);
bool procesar_asignacion(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
