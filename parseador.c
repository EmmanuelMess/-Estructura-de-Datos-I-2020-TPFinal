#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include "parseador.h"

#define METADATOS_ERROR_GENERAL(pos) METADATOS_ERROR(METADATOS_ERROR_NUM, pos)
#define METADATOS_ERROR_ARG_IMPRIMIR(pos) METADATOS_ERROR(METADATOS_ERROR_ARG_IMPRIMIR_NUM, pos)

void remover_espacios(wchar_t * entrada) {
  int i = 0;
  int j = 0;
  while (entrada[i] != '\n') {
    entrada[j++] = entrada[i++];
    while (isblank(entrada[i])) i++;
  }
  entrada[j] = '\0';
}

bool es_numero(const wchar_t* entrada) {
  if(iswdigit(*entrada)) return true;
  if(*entrada == '-' && iswdigit(*(entrada+1))) return true;
  return false;
}

/**
 * Verifica que un conjunto sin espacios este bien formulado
 * Ej "{}", "{5,6}", "{x:80<=x<=30}"
 */
Metadatos chequear_conjunto_expicito(Metadatos metadatos, wchar_t * entrada) {
  if(*entrada != '{') return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada == '}' || es_numero(entrada))
    metadatos.esExtension = true;
  else if(isalnum(*entrada))
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

  wchar_t * inicioAliasInterno = entrada;

  while(iswalnum(*entrada)) {
    metadatos.largoAliasInterno++;
    entrada++;
  }

  if(*entrada != ':') return METADATOS_ERROR_GENERAL(entrada);
  else entrada++;

  while (es_numero(entrada)) entrada++;

  int largoMenorOIgual = strlen("<=");

  if(wcsncmp(entrada, L"<=", largoMenorOIgual) != 0)
    return METADATOS_ERROR_GENERAL(entrada);
  else entrada += largoMenorOIgual;

  if(wcsncmp(entrada, inicioAliasInterno, metadatos.largoAliasInterno) != 0)
    return METADATOS_ERROR_GENERAL(entrada);
  else entrada += metadatos.largoAliasInterno;

  if(wcsncmp(entrada, L"<=", largoMenorOIgual) != 0)
    return METADATOS_ERROR_GENERAL(entrada);
  else entrada += largoMenorOIgual;

  while (es_numero(entrada)) entrada++;

  if(*entrada != '}') return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);

  return metadatos;
}

Metadatos chequear_operacion(Metadatos metadatos, wchar_t * entrada) {
  if(!iswalnum(*entrada) && *entrada != '~') return METADATOS_ERROR_GENERAL(entrada);

  if(*entrada == '~') {
    metadatos.complemento = true;
    entrada++;
  }

  while (iswalnum(*entrada)) {
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

  while (iswalnum(*entrada)) {
    metadatos.largoOperando2++;
    entrada++;
  }

  if(*entrada != '\0') return METADATOS_ERROR_GENERAL(entrada);

  return metadatos;
}

/**
 * Crea infomacion que necesita la funcion procesar_asignacion.
 * Entrada no puede tener espacios o caracteres vacios
 */
Metadatos chequeador(wchar_t * entrada) {
  const wchar_t * ordenSalir = L"salir";
  if(wcsncmp(ordenSalir, entrada, wcslen(ordenSalir)) == 0) {
    if (wcslen(entrada) != wcslen(ordenSalir))
      return METADATOS_ERROR_GENERAL(entrada);

    Metadatos metadatos = { 0 };
    metadatos.salir = true;
    return metadatos;
  }

  const wchar_t * ordenImprimir = L"imprimir";
  if(wcsncmp(ordenImprimir, entrada, wcslen(ordenImprimir)) == 0) {
    if (wcslen(entrada) == wcslen(ordenImprimir))
      return METADATOS_ERROR_ARG_IMPRIMIR(entrada);

    Metadatos metadatos = { 0 };
    metadatos.imprimir = true;
    return metadatos;
  }

  Metadatos metadatos = { 0 };

  while (iswalnum(*entrada)) {
    metadatos.largoAlias++;
    entrada++;
  }

  if(*entrada != '=') return METADATOS_ERROR_GENERAL(entrada);

  entrada++;

  if(*entrada == '{') return chequear_conjunto_expicito(metadatos, entrada);
  else if(iswalnum(*entrada) || *entrada == '~')
    return chequear_operacion(metadatos, entrada);
  else return METADATOS_ERROR_GENERAL(entrada);
}

void procesar_asignacion(Metadatos metadatos, wchar_t * entrada, wchar_t* alias, int* enteros, Rango * rango) {
  wcsncpy(alias, entrada, metadatos.largoAlias);
  alias[metadatos.largoAlias] = '\0';

  entrada += metadatos.largoAlias;

  while(*entrada != '{')entrada++;

  int indiceEnteros = 0;

  if(metadatos.esExtension) {
    while (*entrada != '}') {
      entrada++;
      enteros[indiceEnteros++] = wcstol(entrada, &entrada, 10);
    }
  } else {
    entrada += strlen("{") + metadatos.largoAliasInterno + strlen(":");
    rango->a = wcstol(entrada, &entrada, 10);
    entrada += strlen("<=") + metadatos.largoAliasInterno + strlen("<=");
    rango->b = wcstol(entrada, &entrada, 10);
  }
}

void procesar_operacion(Metadatos metadatos, wchar_t *entrada, wchar_t* alias,
                        wchar_t *aliasA, wchar_t *aliasB) {
  wcsncpy(alias, entrada, metadatos.largoAlias);
  alias[metadatos.largoAlias] = '\0';

  if(metadatos.complemento) {
    wchar_t *inicioAliasA = entrada + metadatos.largoAlias + strlen("=~");
    wcsncpy(aliasA, inicioAliasA, metadatos.largoOperando1);
    aliasA[metadatos.largoOperando1] = '\0';
    return;
  }

  wchar_t *inicioAliasA = entrada + metadatos.largoAlias + strlen("=");
  wcsncpy(aliasA, inicioAliasA, metadatos.largoOperando1);
  aliasA[metadatos.largoOperando1] = '\0';

  wchar_t *inicioAliasB = entrada + metadatos.largoAlias + strlen("=")
                       + metadatos.largoOperando1 + 1; //1 es el largo del operando en chars
  wcsncpy(aliasB, inicioAliasB, metadatos.largoOperando2);
  aliasB[metadatos.largoOperando2] = '\0';
}
