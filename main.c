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

  Trie* trie = trie_crear();
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

        trie_agregar(trie, alias);

#if DEBUG
        printf("Parser: %s ", alias);

        if (metadatos.esExtension) {
          for (int i = 0; i < metadatos.largo; ++i) {
            printf("%d ", enteros[i]);
          }
        } else {
          printf("%d:%d", rango.a, rango.b);
        }

        printf("\n");

        printf("Trie: %s: %s\n", alias, trie_chequear(trie, alias)? "encotrado":"error");
#endif
      }

      free(entrada);
    }
  }

  trie_destruir(trie);
}