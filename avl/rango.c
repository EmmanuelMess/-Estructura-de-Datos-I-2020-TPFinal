#include "rango.h"
#include "matematica.h"

bool inexistente(Rango rango) {
  return rango.a > rango.b;
}

Rango rango_interseccion(Rango parteA, Rango parteB) {
  return (Rango) {.a = max(parteA.a, parteB.a), .b = min(parteA.b, parteB.b)};
}

void rango_resta(Rango parteA, Rango parteB, Rango *resultadoA, Rango *resultadoB) {
  if(parteA.a < parteB.a) {
    *resultadoA = (Rango) {.a = parteA.a, .b = min(parteA.b, parteB.a - 1)};
  }

  if(parteB.b < parteA.b) {
    *resultadoB = (Rango) {.a = max(parteA.a, parteB.b + 1), .b = parteA.b};
  }
}

Rango rango_union(Rango parteA, Rango parteB) {
  return (Rango) {.a = min(parteA.a, parteB.a), .b = max(parteA.b, parteB.b)};
}

bool rango_intersecan(Rango uno, Rango dos) {
  return (uno.b >= dos.a && dos.b >= uno.a);
}