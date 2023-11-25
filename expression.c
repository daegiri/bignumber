#include <stdlib.h>
#include <stdio.h>

#include "expression.h"
#include "helpers.h"

// Processo de adição de números positivos
static struct big_number *add_positive_big_numbers(struct big_number *bg1, struct big_number *bg2) {
    int max_length = bg1->length > bg2->length ? bg1->length : bg2->length;
    char *result = calloc(max_length + 1, sizeof(char));

    int rest = 0;
    int i = 0;

    for (; i < max_length; i++) {
        int digit1 = i < bg1->length ? bg1->digits[bg1->length - i - 1] - '0' : 0;
        int digit2 = i < bg2->length ? bg2->digits[bg2->length - i - 1] - '0' : 0;

        int sum = digit1 + digit2 + rest;
        result[i] = (sum % 10) + '0';
        rest = sum / 10;
    }

    if (rest > 0) {
        result[i++] = rest + '0';
    }

    reverse_string(result);

    return create_big_number(result, i, false);
}

// Processo de subtração de números positivos
static struct big_number *subtract_positive_big_numbers(struct big_number *bg1, struct big_number *bg2) {
    int max_length = bg1->length;
    char *result = calloc(max_length + 1, sizeof(char));

    int rest = 0;
    int i = 0;

    for (; i < max_length; i++) {
        int digit1 = i < bg1->length ? bg1->digits[bg1->length - i - 1] - '0' : 0;
        int digit2 = i < bg2->length ? bg2->digits[bg2->length - i - 1] - '0' : 0;

        if (digit1 < digit2 + rest) {
            digit1 = digit1 + 10;
            result[i] = (digit1 - digit2 - rest) + '0';
            rest = 1;
        } else {
            result[i] = (digit1 - digit2 - rest) + '0';
            rest = 0;
        }
    }

    if (rest > 0) {
        result[i++] = rest + '0';
    }

    reverse_string(result);

    return create_big_number(result, i, false);
}


// Subtração - sinais iguais subtrai, sinais diferentes soma
static struct big_number *subtract(struct big_number *bg1, struct big_number *bg2) {
    struct big_number *result = NULL;
    if (bg1->negative == bg2->negative) {
        if (bg1->length > bg2->length || (bg1->length == bg2->length && strcmp(bg1->digits, bg2->digits) >= 0)) {
            result = subtract_positive_big_numbers(bg1, bg2);
            result->negative = bg1->negative;
        } else {
            result = subtract_positive_big_numbers(bg2, bg1);
            result->negative = !bg1->negative;
        }
    } else {
        result = add_positive_big_numbers(bg1, bg2);
        result->negative = bg1->negative;
    }
    return result;
}


// Função para multiplicar dois big numbers
struct big_number* multiply(struct big_number* num1, struct big_number* num2) {
    
    int len1 = num1->length;
    int len2 = num2->length;
    int len = len1 + len2;

    // Alocando memória para result
    char *result = calloc( len + 1, sizeof(char));

    // Multiplicando
    for (int i = 0; i < len1; i++) {
        int carry = 0;
        for (int j = 0; j < len2; j++) {
            int product = (num1->digits[len1 - i - 1] - '0') * (num2->digits[len2 - j - 1] - '0') + carry;
            result[i + j] = product % 10;
            carry = product / 10;
        }
        result[i + len2] = carry;
    }

 // Invertendo o array de resultado
    for (int i = 0; i < len / 2; i++) {
        int temp = result[i];
        result[i] = result[len - i - 1];
        result[len - i - 1] = temp;
    }

    // Convertendo os dígitos de volta para caracteres
    for (int i = 0; i < len; i++) {
        result[i] += '0';
    }

    // Encontrando o primeiro dígito que não seja zero
    int start = 0;
    while (result[start] == '0' && start < len - 1) {
        start++;
    }

    // Criando uma nova string sem os zeros à esquerda
    int new_len = len - start;
    char *final_result = malloc((new_len + 1) * sizeof(char));
    memcpy(final_result, &result[start], new_len);
    final_result[new_len] = '\0'; // Adicionando o caractere nulo no final

    // Liberando a memória do array de resultado original
    free(result);

    // Definindo o sinal do resultado
    bool result_negative = num1->negative != num2->negative;

    return create_big_number(final_result, new_len, result_negative);
}

// Subtração - sinais iguais soma, sinais diferentes subtrai
static struct big_number *add(struct big_number *bg1, struct big_number *bg2) {
    struct big_number *result = NULL;
    if (bg1->negative == bg2->negative) {
        result = add_positive_big_numbers(bg1, bg2);
        result->negative = bg1->negative;
    } else {
        if (bg1->length > bg2->length || (bg1->length == bg2->length && strcmp(bg1->digits, bg2->digits) >= 0)) {
            result = subtract_positive_big_numbers(bg1, bg2);
            result->negative = bg1->negative;
        } else {
            result = subtract_positive_big_numbers(bg2, bg1);
            result->negative = !bg1->negative;
        }
    }
    return result;
}

// Cria nova expressão
struct expression *create_expression() {
    struct big_number *bg1 = read_big_number();
    struct big_number *bg2 = read_big_number();
    char op = getchar();
    getchar();

    struct expression *expression = malloc(sizeof(struct expression));

    expression->numbers[0] = bg1;
    expression->numbers[1] = bg2;
    expression->operation = convert_operation(op);
    expression->result = NULL;

    return expression;
}

// Cálculo do resultado final - direciona para as funções corretas
void calculate_expression(struct expression *expression) {
    struct big_number *result = NULL;

    switch (expression->operation) {
        case ADDITION:
            result = add(expression->numbers[0], expression->numbers[1]);
            break;
        case SUBTRACTION:
            result = subtract(expression->numbers[0], expression->numbers[1]);
            break;
        case MULTIPLICATION:
            result = multiply(expression->numbers[0], expression->numbers[1]);
            break;
        default:
            break;
    }

    expression->result = result;
}

// Libera a memória das expressões realizadas e números utilizados
void destroy_expression(struct expression *expression) {
    destroy_big_number(expression->numbers[0]);
    destroy_big_number(expression->numbers[1]);
    free(expression->result);
    free(expression);
}