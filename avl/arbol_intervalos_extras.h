#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_EXTRAS_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_EXTRAS_H

#include "arbol_intervalos.h"

/**
 * Alerta: Como no edita los arboles de entrada,
 * deben ser eliminados independientemente de el nuevo!
 *
 * Une dos arboles de intervalos, creando uno nuevo en el proceso.
 */
ArbolIntervalos * arbolintervalos_union(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB);

/**
 * Alerta: Como no edita los arboles de entrada,
 * deben ser eliminados independientemente de el nuevo!
 *
 * Inteseca dos arboles de intervalos, creando uno nuevo en el proceso.
 */
ArbolIntervalos * arbolintervalos_interseccion(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB);

/**
 * Alerta: Como no edita los arboles de entrada,
 * deben ser eliminados independientemente de el nuevo!
 *
 * Resta dos arboles de intervalos, creando uno nuevo en el proceso.
 */
ArbolIntervalos * arbolintervalos_resta(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB);

/**
 * Alerta: Como no edita el arbol de entrada,
 * deben ser eliminado independientemente de el nuevo!
 *
 * Calcula el complemento del arbol de intervalos,
 * creando uno nuevo en el proceso.
 */
ArbolIntervalos * arbolintervalos_complemento(ArbolIntervalos * arbol);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_EXTRAS_H
