#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parseador.h"

const Metadatos METADATOS_ERROR = {.error = METADATOS_ERROR_NUM};
const Metadatos METADATOS_ERROR_ARG_IMPRIMIR = {.error = METADATOS_ERROR_ARG_IMPRIMIR_NUM};

bool procesar_asignacion(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango) {
  strncpy(alias, entrada, metadatos.largoAlias);
  entrada += metadatos.largoAlias;

  while(*entrada != '{') {
    entrada++;
  }

  int indiceEnteros = 0;

  if(metadatos.esExtension) {
    while (*entrada != '}') {
      entrada++;
      enteros[indiceEnteros++] = strtol(entrada, &entrada, 10);
    }
  } else {
    entrada += strlen("{x:");
    rango->a = strtol(entrada, &entrada, 10);
    entrada += strlen("<=x<=");
    rango->b = strtol(entrada, &entrada, 10);
  }

  return true;
}

void remover_espacios(char* entrada) {
  int i = 0;
  int j = 0;
  while (entrada[i] != '\n') {
    entrada[j++] = entrada[i++];
    while (isblank(entrada[i])) i++;
  }
  entrada[j] = '\0';
}

bool es_numero(char* entrada) {
  if(isdigit(*entrada)) return true;
  if(*entrada == '-' && isdigit(*(entrada+1))) return true;
  return false;
}

/**
 * Verifica que una asignacion sin espacios este bien formulada
 * Ej "a={}", "a={5,6}", "a={x:80<=x<=30}"
 */
Metadatos chequeador_asignacion(char * entrada) {
  Metadatos metadatos = { 0 };

  while (isalnum(*entrada)) {
    metadatos.largoAlias++;
    entrada++;
  }

  if(*entrada != '=') return METADATOS_ERROR;

  entrada++;

  if(*entrada != '{') return METADATOS_ERROR;

  entrada++;

  if(*entrada == '}' || es_numero(entrada)) {
    metadatos.esExtension = true;
  } else if(*entrada == 'x') {
    metadatos.esExtension = false;
  } else {
    metadatos.error = 1;
    return metadatos;
  }

  if(metadatos.esExtension) {
    if (*entrada == '}') {
      entrada++;
      if(*entrada != '\0') return METADATOS_ERROR;

      metadatos.largo = 0;
      return metadatos;
    }

    while (es_numero(entrada) || *entrada == '-') {
      while (es_numero(entrada)) entrada++;

      if (*entrada != ',' && *entrada != '}') return METADATOS_ERROR;
      else entrada++;

      metadatos.largo++;
    }

    if(*entrada != '\0') return METADATOS_ERROR;

    return metadatos;
  }

  entrada++;

  if(*entrada != ':') return METADATOS_ERROR;
  else entrada++;

  while (es_numero(entrada)) entrada++;

  int len = strlen("<=x<=");

  if(strncmp(entrada, "<=x<=", len) != 0) return METADATOS_ERROR;
  else entrada += len;

  while (es_numero(entrada)) entrada++;

  if(*entrada != '}') return METADATOS_ERROR;

  entrada++;

  if(*entrada != '\0') return METADATOS_ERROR;

  return metadatos;
}

/**
 * Crea infomacion que necesita la funcion procesar_asignacion.
 * Entrada no puede tener espacios o caracteres vacios
 */
Metadatos chequeador(char * entrada) {
  const char* ordenSalir = "salir";
  if(strncmp(ordenSalir, entrada, strlen(ordenSalir)) == 0) {
    if (strlen(entrada) != strlen(ordenSalir)) {
      return METADATOS_ERROR;
    }

    Metadatos metadatos = { 0 };
    metadatos.salir = true;
    return metadatos;
  }

  const char* ordenImprimir = "imprimir";
  if(strncmp(ordenImprimir, entrada, strlen(ordenImprimir)) == 0) {
    if (strlen(entrada) == strlen(ordenImprimir)) {
      return METADATOS_ERROR_ARG_IMPRIMIR;
    }

    Metadatos metadatos = { 0 };
    metadatos.imprimir = true;
    return metadatos;
  }

  return chequeador_asignacion(entrada);
}