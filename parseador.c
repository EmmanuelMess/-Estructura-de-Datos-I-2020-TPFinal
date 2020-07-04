#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parseador.h"

bool procesar(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango) {
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
    rango->inicio = strtol(entrada, &entrada, 10);
    entrada += strlen("<=x<=");
    rango->final = strtol(entrada, &entrada, 10);
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

Metadatos chequeador(char * entrada) {
  Metadatos metadatos = { 0 };

  while (isalnum(*entrada)) {
    metadatos.largoAlias++;
    entrada++;
  }

  if(*entrada != '=') return ERROR;

  entrada++;

  if(*entrada != '{') return ERROR;

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
      if(*entrada != '\0') return ERROR;

      metadatos.largo = 0;
      return metadatos;
    }

    while (es_numero(entrada) || *entrada == '-') {
      while (es_numero(entrada)) entrada++;

      if (*entrada != ',' && *entrada != '}') return ERROR;
      else entrada++;

      metadatos.largo++;
    }

    if(*entrada != '\0') return ERROR;

    return metadatos;
  }

  entrada++;

  if(*entrada != ':') return ERROR;
  else entrada++;

  while (es_numero(entrada)) entrada++;

  int len = strlen("<=x<=");

  if(strncmp(entrada, "<=x<=", len) != 0) return ERROR;
  else entrada += len;

  while (es_numero(entrada)) entrada++;

  if(*entrada != '}') return ERROR;

  entrada++;

  if(*entrada != '\0') return ERROR;

  return metadatos;
}