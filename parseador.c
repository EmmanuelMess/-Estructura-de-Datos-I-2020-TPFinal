#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parseador.h"

#define METADATOS_ERROR_GENERAL(pos) METADATOS_ERROR(METADATOS_ERROR_NUM, pos)
#define METADATOS_ERROR_ARG_IMPRIMIR(pos) METADATOS_ERROR(METADATOS_ERROR_ARG_IMPRIMIR_NUM, pos)

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
 * Verifica que un conjunto sin espacios este bien formulado
 * Ej "{}", "{5,6}", "{x:80<=x<=30}"
 */
Metadatos chequear_conjunto_expicito(Metadatos metadatos, char * entrada) {
  if(*entrada != '{') return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada == '}' || es_numero(entrada))
    metadatos.esExtension = true;
  else if(*entrada == 'x')
    metadatos.esExtension = false;
  else return METADATOS_ERROR_GENERAL(entrada);


  if(metadatos.esExtension) {
    if (*entrada == '}') {
      entrada++;
      if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);

      metadatos.largo = 0;
      return metadatos;
    }

    while (es_numero(entrada) || *entrada == '-') {
      while (es_numero(entrada)) entrada++;

      if (*entrada != ',' && *entrada != '}') return METADATOS_ERROR_GENERAL(entrada);
      else entrada++;

      metadatos.largo++;
    }

    if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);

    return metadatos;
  }

  entrada++;

  if(*entrada != ':') return METADATOS_ERROR_GENERAL(entrada);
  else entrada++;

  while (es_numero(entrada)) entrada++;

  int len = strlen("<=x<=");

  if(strncmp(entrada, "<=x<=", len) != 0) return METADATOS_ERROR_GENERAL(entrada);
  else entrada += len;

  while (es_numero(entrada)) entrada++;

  if(*entrada != '}') return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);

  return metadatos;
}

Metadatos chequear_operacion(Metadatos metadatos, char * entrada) {
  if(!isalnum(*entrada)) return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada == '~') {
    metadatos.complemento = true;
  }

  while (isalnum(*entrada)) {
    metadatos.largoOperando1++;
    entrada++;
  }

  if(metadatos.complemento) {
    if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);
    return metadatos;
  }

  switch (*entrada) {
    case '|':
      metadatos.union_ = true;
      break;
    case '&':
      metadatos.interseccion = true;
      break;
    case '-':
      metadatos.resta = true;
      break;
    default:
      return METADATOS_ERROR_GENERAL(entrada);
  }

  entrada++;

  while (isalnum(*entrada)) {
    metadatos.largoOperando1++;
    entrada++;
  }

  if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);

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
      return METADATOS_ERROR_GENERAL(entrada);
    }

    Metadatos metadatos = { 0 };
    metadatos.salir = true;
    return metadatos;
  }

  const char* ordenImprimir = "imprimir";
  if(strncmp(ordenImprimir, entrada, strlen(ordenImprimir)) == 0) {
    if (strlen(entrada) == strlen(ordenImprimir)) {
      return METADATOS_ERROR_ARG_IMPRIMIR(entrada);
    }

    Metadatos metadatos = { 0 };
    metadatos.imprimir = true;
    return metadatos;
  }

  Metadatos metadatos = { 0 };

  while (isalnum(*entrada)) {
    metadatos.largoAlias++;
    entrada++;
  }

  if(*entrada != '=') return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada == '{') return chequear_conjunto_expicito(metadatos, entrada);
  else if(isalnum(*entrada)) return chequear_operacion(metadatos, entrada);
  else return METADATOS_ERROR_GENERAL(entrada);
}

void procesar_asignacion(Metadatos metadatos, char* entrada, char* alias, int* enteros, Rango * rango) {
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
}

void procesar_operacion(Metadatos metadatos, char *entrada, char *aliasA,
                        char *aliasB) {
  if(metadatos.complemento) {
    char *inicioAliasA = entrada + metadatos.largoAlias + strlen("=~");
    strncpy(aliasA, inicioAliasA, metadatos.largoOperando1);
    return;
  }

  char *inicioAliasA = entrada + metadatos.largoAlias + strlen("=");
  strncpy(aliasA, inicioAliasA, metadatos.largoOperando1);

  char *inicioAliasB = entrada + metadatos.largoAlias + strlen("=")
                       + metadatos.largoOperando1 + 1; //1 es el largo del operando en chars
  strncpy(aliasB, inicioAliasB, metadatos.largoOperando1);
}
