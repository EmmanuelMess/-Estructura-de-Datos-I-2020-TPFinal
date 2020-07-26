#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_RANGO_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_RANGO_H

#include <stdbool.h>

/**
 * Un intervalo de enteros, simplemente representado como dos enteros,
 * el inicial y el final, ambos siempre pertenecen al rango
 */
typedef struct {
  int a, b;
} Rango;

/**
 * Un rango que se usa para representar la no existencia de un rango
 *
 * Por ejemplo, si la funcion devuelve un rango en un arbol,
 * pero el arbol esta vacio, pudiera devolver RANGO_INEXISTENTE
 *
 * Usar inexistente(Rango) para chequear si el rango devuelto es inexistente
 */
#define RANGO_INEXISTENTE ((Rango) {.a = INT_MAX, .b = INT_MIN});

/**
 * Ver RANGO_INEXISTENTE
 *
 * Dice si el rango pasado es RANGO_INEXISTENTE
 */
bool inexistente(Rango rango);

/**
 * El rango resultante de la interseccion de dos otros rangos.
 */
Rango rango_interseccion(Rango parteA, Rango parteB);

/**
 * El rango resultante de la resta de dos otros rangos.
 * Este resultado puede ser hasta dos rangos,
 * por lo que el resultado se devuelve modificado parteA y parteB,
 * en el caso de que solo un rango o ningun rango se necesite devolver,
 * la funcion no los cambia, es decir:
 *   Rango a = {.a = 3, .b = 4};
 *   Rango r1 = {.a = -1, .b = 3};
 *   Rango r2 = {.a = 5, .b = -5};
 *   rango_resta(a, a, &r1, &r2);
 * No produce cambios en r1 y r2.
 *
 * Un uso estandar es asignar a r1 y r2 RANGO_INEXISTENTE
 */
void rango_resta(Rango parteA, Rango parteB, Rango *resultadoA, Rango *resultadoB);

/**
 * el rango resultante de la union de dos otros rangos.
 */
Rango rango_union(Rango parteA, Rango parteB);

/**
 * true sii los rango se solapan.
 */
bool rango_intersecan(Rango uno, Rango dos);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_RANGO_H
