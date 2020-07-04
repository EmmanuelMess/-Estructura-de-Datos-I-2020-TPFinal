#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
#include "trie/trie.h"
#include "parseador.h"

#define DEBUG true

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
        switch (metadatos.error) {
          case METADATOS_ERROR_NUM:
            printf("Error de parseo!\n");
            sigue = true;
            break;
          case METADATOS_ERROR_ARG_IMPRIMIR_NUM:
            printf("Uso: imprimir <alias>!\n");
            sigue = true;
            break;
        }

      } else if (metadatos.salir) {
        sigue = false;
      } else if(metadatos.imprimir) {
        //TODO trie get
      } else {
        char *alias = calloc(metadatos.largoAlias + 1, sizeof(char));
        int *enteros = malloc(metadatos.largo * sizeof(int));
        Rango rango = RANGO_INEXISTENTE;

        procesar_asignacion(metadatos, entrada, alias, enteros, &rango);

        ArbolAvl * arbol = itree_crear();
        itree_insertar(arbol, rango);

        trie_agregar(trie, alias, arbol);

#if DEBUG
        printf("DEBUG INFO\n");
        printf("Parser: %s ", alias);

        if (metadatos.esExtension) {
          for (int i = 0; i < metadatos.largo; ++i) {
            printf("%d ", enteros[i]);
          }
        } else {
          printf("%d:%d", rango.a, rango.b);
        }

        printf("\n");

        printf("Trie: %s: %s\n", alias,
               trie_chequear(trie, alias) ? "encotrado" : "error");
#endif
      }
    }

    free(entrada);
  }

  trie_destruir(trie);
}