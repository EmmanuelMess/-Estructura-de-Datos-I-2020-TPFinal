#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H

#include "avl/arbol_avl.h"

typedef struct {
  //error
  int error;
  char* posError;

  //parseo de asignacion
  int largoAlias;
  bool esExtension;
  int largo;

  //ordenes
  bool imprimir;
  bool salir;

  //operaciones
  bool union_;//"union" esta reservada
  bool interseccion;
  bool resta;
  bool complemento;

  int largoOperando1;
  int largoOperando2;
} Metadatos;

/**
 * Error general de parseo
 */
#define METADATOS_ERROR_NUM 1

/**
 * Error en los argumentos de imprimir
 */
#define METADATOS_ERROR_ARG_IMPRIMIR_NUM 10

#define METADATOS_ERROR(codigoError, pos) ((Metadatos) {.error = codigoError, .posError = pos})

void remover_espacios(char* entrada);
Metadatos chequeador(char * entrada);
void procesar_asignacion(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
