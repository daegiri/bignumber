#ifndef __BIGNUMBER_H__
#define __BIGNUMBER_H__

#include <stdbool.h>

// Definição da estrutura geral
struct big_number {
    char *digits;
    int length;
    bool negative;
};

// Funções para ler, criar e excluir um número
struct big_number *read_big_number();
struct big_number *create_big_number(char *digits, int size, bool is_negative);
void destroy_big_number(struct big_number *number);

// Checa se o número existe e é negativo para imprimir seu sinal
#define PRINT_BIG_NUMBER(bn) do { \
    if ((bn) != NULL) { \
        if ((bn)->negative) { \
            printf("-"); \
        } \
        printf("%s\n", (bn)->digits); \
    } else { \
        printf("NULL big_number\n"); \
    } \
} while (0)

#endif