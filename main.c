#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tests.h"
#include "trie/trie.h"
#include "parseador.h"
#include "avl/deque.h"

#define DEBUG true

char* max_puntero(char* a, char* b) {
  return a > b? a: b;
}

void imprimir_intervalos(ArbolIntervalos *arbol) {
  if (arbol->arbolAvlNode == NULL) {
    printf("{}");
    return;
  }

  Deque *deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNode);

  printf("{");

  while (!deque_vacio(deque)) {
    ArbolIntervalosNode *nodo = deque_pop_front(deque);

    if (nodo->izquierda) {
      deque_push_front(deque, nodo->izquierda);
    }
    if (nodo->derecha) {
      deque_push_front(deque, nodo->derecha);
    }

    if(nodo->rango.a == nodo->rango.b) printf("%d", nodo->rango.a);
    else printf("%d:%d", nodo->rango.a, nodo->rango.b);

    if (!deque_vacio(deque)) printf(",");
  }

  printf("}\n");

  deque_destruir(deque);
}

void debug1(char * alias, Metadatos metadatos, int* enteros) {
#if DEBUG
  printf("DEBUG INFO\n");
  printf("Parser: %s ", alias);

  for (int i = 0; i < metadatos.largo; ++i) {
    printf("%d ", enteros[i]);
  }
#endif
}

void debug2(char * alias, Rango rango) {
#if DEBUG
  printf("DEBUG INFO\n");
  printf("Parser: %s ", alias);

  printf("%d:%d", rango.a, rango.b);
#endif
}

void debug3(char * alias, Trie * trie) {
#if DEBUG
  printf("\n");

  printf("Trie: %s: %s\n", alias,
         trie_obtener(trie, alias) != NULL ? "encotrado" : "error");
#endif
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

  printf("Tamaño maximo de una linea: 1023 caracteres\n");

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
        ArbolIntervalos *arbol = trie_obtener(trie, alias);

        imprimir_intervalos(arbol);

#if DEBUG
        arbolintervalos_imprimir_arbol(arbol);
#endif
      } else if (metadatos.union_ || metadatos.interseccion || metadatos.resta) {
        char alias[metadatos.largoAlias + 1];
        char aliasA[metadatos.largoOperando1 + 1];
        char aliasB[metadatos.largoOperando2 + 1];
        procesar_operacion(metadatos, entrada, alias, aliasA, aliasB);

        ArbolIntervalos *arbolA = trie_obtener(trie, aliasA);
        ArbolIntervalos *arbolB = trie_obtener(trie, aliasB);

        if(arbolA == NULL || arbolB == NULL) {
          if(arbolA == NULL) printf("Alias no existe: %s\n", aliasA);
          if(arbolB == NULL) printf("Alias no existe: %s\n", aliasB);
        } else {
          ArbolIntervalos *resultado;
          if (metadatos.union_)
            resultado = arbolintervalos_union(arbolA, arbolB);
          else if (metadatos.interseccion)
            resultado = arbolintervalos_interseccion(arbolA, arbolB);
          else if (metadatos.resta)
            resultado = arbolintervalos_resta(arbolA, arbolB);


          if (trie_obtener(trie, alias) != NULL)
            arbolintervalos_destruir(trie_obtener(trie, alias));

          trie_agregar(trie, alias, resultado);
        }
      } else if (metadatos.complemento) {
        char alias[metadatos.largoAlias + 1];
        char aliasA[metadatos.largoOperando1];
        procesar_operacion(metadatos, entrada, alias, aliasA, NULL);

        ArbolIntervalos *arbol = trie_obtener(trie, aliasA);

        if(arbol == NULL) {
          printf("Alias no existe: %s\n", aliasA);
        } else {
          ArbolIntervalos *resultado = arbolintervalos_complemento(arbol);

          if(trie_obtener(trie, alias) != NULL)
            arbolintervalos_destruir(trie_obtener(trie, alias));

          trie_agregar(trie, alias, resultado);
        }
      } else {
        char alias[metadatos.largoAlias + 1];
        ArbolIntervalos *arbol = arbolintervalos_crear();

        if (metadatos.esExtension) {
          int enteros[metadatos.largo];

          procesar_asignacion(metadatos, entrada, alias, enteros, NULL);

          for (int i = 0; i < metadatos.largo; ++i) {
            arbolintervalos_insertar(arbol,
                                     (Rango) {.a = enteros[i], .b = enteros[i]});
          }

          debug1(alias, metadatos, enteros);
        } else {
          Rango rango;

          procesar_asignacion(metadatos, entrada, alias, NULL, &rango);

          arbolintervalos_insertar(arbol, rango);

          debug2(alias, rango);
        }

        trie_agregar(trie, alias, arbol);

        debug3(alias, trie);
      }
    }

    free(entrada);
  }

  trie_destruir(trie);
}
