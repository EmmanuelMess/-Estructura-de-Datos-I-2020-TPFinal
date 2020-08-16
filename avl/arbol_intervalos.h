#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_H
#include <limits.h>
#include <stdbool.h>
#include "rango.h"

/**
 * Todas las funciones mantinen la entrada intacta, exepto:
 * arbolintervalos_destruir
 * arbolintervalos_insertar
 * arbolintervalos_eliminar
 */

/**
 * Nodo interno del arbol, ver ArbolIntervalos
 */
typedef struct ArbolIntervalosNodo_ {
  Rango rango;
  int maximoFinalDeRango;
  int alto;

  struct ArbolIntervalosNodo_* derecha;
  struct ArbolIntervalosNodo_* izquierda;
} ArbolIntervalosNodo;

/**
 * Arbol Adelson-Velskii Landis, con invariantes extra:
 *   Los nodos son intervalos cerrados de enteros
 *   Los nodos guardan el maximo valor contenido en el subarbol (del nodo)
 *   Los intervalos son siempre disjuntos dentro del arbol
 *   Los nodos guardan tambien la altura
 */
typedef struct {
  ArbolIntervalosNodo* arbolAvlNodo;
} ArbolIntervalos;

/**
 * Simplemente crea un arbol de intervalos, vacio
 */
ArbolIntervalos * arbolintervalos_crear();

/**
 * Genera una copia exacta del arbol pasado
 */
ArbolIntervalos * arbolintervalos_copiar(ArbolIntervalos* arbol);

/**
 * Imprime una version facilmente legible del arbol,
 * no es de lo mas linda, pero muy util.
 * Los nodos se imprimen como "{m: <maximo>, r: [<rango inicio>, <final>], a: <altura>}"
 * Cada nivel del arbol esta en una linea nueva
 */
void arbolintervalos_imprimir(ArbolIntervalos *arbol);

/**
 * Calcula el factor de equilibrio de un nodo, garantizado O(1)
 */
int arbolintervalos_factor_equilibrio(ArbolIntervalosNodo *nodo);

/**
 * Alerta: puede no ser la unica interseccion!
 *
 * Devuelve un Rango en el arbol que interseca con el dado
 * Si ninguno interseca, devuelve rango.h:RANGO_INEXISTENTE
 */
Rango arbolintervalos_intersectar(ArbolIntervalos* arbol, Rango rango);

/**
 * Alerta: Edita la entrada.
 *
 * Destruye el arbol pasado.
 */
void arbolintervalos_destruir(ArbolIntervalos* arbol);

/**
 * Alerta: Edita la entrada.
 * Inserta un nuevo rango en el arbol.
 * Mantiene las invariantes
 *
 * Retorna el nodo insertado, no modificar.
 */
ArbolIntervalosNodo* arbolintervalos_insertar(ArbolIntervalos *arbol, Rango rango);

/**
 * Alerta: No resta rangos! El rango a eliminar debe coincidir _exactamente_
 * con uno en el arbol!
 * Alerta: Edita la entrada.
 *
 * Elimina un rango en el arbol.
 * Mantiene las invariantes
 */
void arbolintervalos_eliminar(ArbolIntervalos* arbol, Rango rango);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_H
