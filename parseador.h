#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H

#include "avl/arbol_intervalos.h"

typedef struct {
  //error
  int error;
  wchar_t * posError;

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

void remover_espacios(wchar_t * entrada);

Metadatos chequeador(wchar_t * entrada);

void procesar_asignacion(Metadatos metadatos, wchar_t * entrada, wchar_t * alias,
  int* enteros, Rango * rango);

void procesar_operacion(Metadatos metadatos, wchar_t* entrada, wchar_t * alias,
                        wchar_t aliasA[], wchar_t aliasB[]);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
