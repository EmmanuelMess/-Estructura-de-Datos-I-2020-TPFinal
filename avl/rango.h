#ifndef ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_RANGO_H
#define ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_RANGO_H

#include <stdbool.h>

typedef struct {
  int a, b;
} Rango;

#define RANGO_INEXISTENTE ((Rango) {.a = INT_MAX, .b = INT_MIN});

bool inexistente(Rango rango);

Rango rango_interseccion(Rango parteA, Rango parteB);

void rango_resta(Rango parteA, Rango parteB, Rango *resultadoA, Rango *resultadoB);

Rango rango_union(Rango parteA, Rango parteB);

bool rango_intersecan(Rango uno, Rango dos);

#endif //ESTRUCTURA_DE_DATOS_I_2020_TPFINAL_RANGO_H
