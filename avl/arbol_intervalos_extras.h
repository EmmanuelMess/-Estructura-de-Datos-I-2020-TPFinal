#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_EXTRAS_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_EXTRAS_H

#include "arbol_intervalos.h"

ArbolIntervalos * arbolintervalos_union(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB);

ArbolIntervalos * arbolintervalos_interseccion(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB);

ArbolIntervalos * arbolintervalos_resta(ArbolIntervalos * arbolA,
  ArbolIntervalos * arbolB);

ArbolIntervalos * arbolintervalos_complemento(ArbolIntervalos * arbol);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_ARBOL_INTERVALOS_EXTRAS_H
