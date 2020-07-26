#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H

#include "avl/arbol_intervalos.h"

/**
 * Define una estructura que contiene informacion sobre la cadena parseada
 * por parser.
 *
 *
 *
 */
typedef struct {
  /**
   * Error es 0 si no hay errores, y un codigo de error si lo hay,
   * ver codigos mas abajo.
   */
  int error;
  /**
   * Si error es no 0, posError contiene el ultimo caracter
   * parseado antes de caer en el error.
   */
  wchar_t * posError;

  /**
   * El largo del alias en la entrada, si es que hay uno
   */
  int largoAlias;
  /**
   * Solo valido si hay un conjunto explicitado en la entrada
   * Si el conjunto en la entrada es por extension, si esto es false,
   * el conjunto es por comprension
   */
  bool esExtension;
  /**
   * El largo del alias interno para conjuntos por compresion
   */
  int largoAliasInterno;
  /**
   * La cantidad de elementos en el conjunto por extension
   */
  int cantidadDeEnteros;

  /**
   * Si hay un comando imprimir valido en la entrada,
   * el largo alias esta dado por largoAlias
   */
  bool imprimir;
  /**
   * Si hay un comando salir valido en la entrada
   */
  bool salir;

  /**
   * Si hay una operacion union en la entrada
   */
  bool union_;//"union" esta reservada
  /**
   * Si hay una operacion interseccion en la entrada
   */
  bool interseccion;
  /**
   * Si hay una operacion resta en la entrada
   */
  bool resta;
  /**
   * Si hay una operacion complemento en la entrada
   */
  bool complemento;

  /**
   * Largo del primer operando,
   * si hay una operacion de uno o dos operandos en la entrada
   */
  int largoOperando1;
  /**
   * Largo del segundo operando,
   * si hay una operacion de dos operandos en la entrada
   */
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

/**
 * Metadatos correspondiente a un error en una posicion.
 */
#define METADATOS_ERROR(codigoError, pos) ((Metadatos) {.error = codigoError, .posError = pos})

/**
 * Elimina los espacion de una cadena de caracteres.
 */
void remover_espacios(wchar_t * entrada);

/**
 * Genera los metadatos para una entrada, no es destructivo,
 * necesita que la entrada haya sido pasada por remover_espacios() primero
 * ver Metadatos
 */
Metadatos parser(wchar_t * entrada);

/**
 * Dado una estructura Metadatos,
 * y un puntero a una cadena de la forma "<alias>={[k1],[k2],[...]}",
 * devuelve un alias y enteros.
 * Enteros debe ser un puntero a un bloque de memoria como minimo de largo
 * Metadatos.cantidadDeEnteros.
 */
void procesar_asignacion_extension(Metadatos metadatos, wchar_t *entrada,
                                   wchar_t *alias, int *enteros);

/**
 * Dado una estructura Metadatos,
 * y un puntero a una cadena de la forma "<alias>={<x>:<k1><=<x><=<k2>}",
 * devuelve un alias y un rango.
 */
void procesar_asignacion_compresion(Metadatos metadatos, wchar_t *entrada,
                                    wchar_t *alias, Rango *rango);

/**
 * Dado una estructura Metadatos,
 * y un puntero a una cadena de la forma "<alias>=<alias1><'&' | '|' | '-'> <alias2>"
 * o de la forma "<alias>=~<alias1>"
 * Devuelve los aliases en la cadena.
 *
 * Alerta: los punteros pasados deben tener largo minimo dado por metadatos.largoAlias
 * metadatos.largoOperando1, metadatos.largoOperando2 respectivamente
 */
void procesar_operacion(Metadatos metadatos, wchar_t* entrada, wchar_t * alias,
                        wchar_t aliasA[], wchar_t aliasB[]);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_PARSEADOR_H
