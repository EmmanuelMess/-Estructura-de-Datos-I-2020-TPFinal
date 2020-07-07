#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
#include "trie/trie.h"
#include "parseador.h"

#define DEBUG true

char* max_puntero(char* a, char* b) {
  return a > b? a: b;
}

int main(int argc, char *argv[]) {
#if DEBUG
  if (argc == 2) {
    main_tests();

    return 0;
  }
#endif

  Trie *trie = trie_crear();
  bool sigue = true;

  while (sigue) {
    printf("> ");
    char *entrada = malloc(1024 * sizeof(char));
    entrada = fgets(entrada, 1024, stdin);
    if (entrada != NULL) {
      remover_espacios(entrada);

      Metadatos metadatos = chequeador(entrada);

      if (metadatos.error) {
        printf("Error de parseo: %.5s\n",
               max_puntero(entrada, metadatos.posError - 2));

        if (metadatos.error == METADATOS_ERROR_ARG_IMPRIMIR_NUM) {
          printf("Uso: imprimir <alias>!\n");
        }

        sigue = true;
      } else if (metadatos.salir) {
        sigue = false;
      } else if (metadatos.imprimir) {
        char *alias = entrada + strlen("imprimir");
        ArbolAvl *arbol = trie_obtener(trie, alias);
        itree_imprimir_arbol(arbol);
      } else if (metadatos.union_ || metadatos.interseccion || metadatos.resta) {
        char aliasA[metadatos.largoOperando1];
        char aliasB[metadatos.largoOperando1];
        procesar_operacion(metadatos, entrada, aliasA, aliasB);

        ArbolAvl *arbolA = trie_obtener(trie, aliasA);
        ArbolAvl *arbolB = trie_obtener(trie, aliasB);
      } else if (metadatos.complemento) {
        char aliasA[metadatos.largoOperando1];
        procesar_operacion(metadatos, entrada, aliasA, NULL);

        ArbolAvl *arbolA = trie_obtener(trie, aliasA);
      } else {
        char *alias = calloc(metadatos.largoAlias + 1, sizeof(char));
        ArbolAvl *arbol = itree_crear();

        if (metadatos.esExtension) {
          int *enteros = malloc(metadatos.largo * sizeof(int));

          procesar_asignacion(metadatos, entrada, alias, enteros, NULL);

          for (int i = 0; i < metadatos.largo; ++i) {
            itree_insertar(arbol, (Rango) {.a = enteros[i], .b = enteros[i]});
          }

#if DEBUG
          printf("DEBUG INFO\n");
          printf("Parser: %s ", alias);

          for (int i = 0; i < metadatos.largo; ++i) {
            printf("%d ", enteros[i]);
          }
#endif
        } else {
          Rango rango;

          procesar_asignacion(metadatos, entrada, alias, NULL, &rango);

          itree_insertar(arbol, rango);

#if DEBUG
          printf("DEBUG INFO\n");
          printf("Parser: %s ", alias);

          printf("%d:%d", rango.a, rango.b);
#endif
        }

        trie_agregar(trie, alias, arbol);

#if DEBUG
        printf("\n");

        printf("Trie: %s: %s\n", alias,
               trie_obtener(trie, alias) != NULL ? "encotrado" : "error");
#endif
      }
    }

    free(entrada);
  }

  trie_destruir(trie);
}