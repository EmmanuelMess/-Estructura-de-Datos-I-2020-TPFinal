#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tests.h"
#define DEBUG true

typedef struct {
  int error;
  int largoAlias;
  bool esExtension;
  int largo;
} Metadatos;

const Metadatos ERROR = {.error = 1};

typedef struct {
  int inicio;
  int final;
} Rango;

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

  if(*entrada == '}' || isdigit(*entrada)) {
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

    while (isdigit(*entrada)) {
      while (isdigit(*entrada)) entrada++;

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

  while (isdigit(*entrada)) entrada++;

  int len = strlen("<=x<=");

  if(strncmp(entrada, "<=x<=", len) != 0) return ERROR;
  else entrada += len;

  while (isdigit(*entrada)) entrada++;

  if(*entrada != '}') return ERROR;

  entrada++;

  if(*entrada != '\0') return ERROR;

  return metadatos;
}

int main(int argc, char *argv[]) {
#if DEBUG
  if (argc == 2) {
    main_tests();

    return 0;
  }
#endif

  bool sigue = true;

  while (sigue) {
    printf("> ");
    char *entrada = malloc(1024 * sizeof(char));
    entrada = fgets(entrada, 1024, stdin);
    if (entrada != NULL) {
      remover_espacios(entrada);

      Metadatos metadatos = chequeador(entrada);

      if (metadatos.error == 1) {
        printf("Error!\n");
        sigue = true;
      } else {
        char *alias = calloc(metadatos.largoAlias + 1, sizeof(char));
        int *enteros = malloc(metadatos.largo * sizeof(int));
        Rango rango;

        procesar(metadatos, entrada, alias, enteros, &rango);

#if DEBUG
        printf("%s ", alias);

        if (metadatos.esExtension) {
          for (int i = 0; i < metadatos.largo; ++i) {
            printf("%d ", enteros[i]);
          }
        } else {
          printf("%d:%d", rango.inicio, rango.final);
        }

        printf("\n");
#endif
      }

      free(entrada);
    }
  }

}