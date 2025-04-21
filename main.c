#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Tive que deixar o caminho absoluto por conta do Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#define OUTPUT "saida.txt"

void makeNumList(int size, int* num_list) {
    FILE *f = fopen(INPUT, "r");

    // A função vai adicionando os caracteres dos números a uma string
    // até encontrar um caracter vazio, ponto e vírgula, ou o fim do arquivo

    // O trabalho cita blocos(mais de uma lista em um arquivo?)
    // , mas não foi considerado isso nessa função
    if (f == NULL) {
        printf("[ERRO] arquivo vazio, saindo.\n");
        return;
    }

    int i = 0;
    int j = 0;
    char number[16];
    char c;

    while ((c = fgetc(f)) != EOF) {
        if (c == ' ') {
            if (i > 0) {
                number[i] = '\0';
                num_list[j++] = strtol(number, NULL, 10);
                i = 0;
            }
            break;
        }
        if (c == ';' || i == 16) {
            number[i] = '\0';
            num_list[j++] = strtol(number, NULL, 10);
            i = 0;
        } else {
            number[i++] = c;
        }

        if (j >= size) {
            break;
        }
    }

    if (i > 0 && j < size) {
        number[i] = '\0';
        num_list[j++] = strtol(number, NULL, 10);
    }

    fclose(f);
}


int main () {
    int data_amount;
    printf("Insira a quantidade de elementos:\n");
    scanf("%d", &data_amount);
    int* list = malloc(data_amount * sizeof(int));
    makeNumList(data_amount, list);

    printf("LISTA ORIGINAL:\n");
    for(int i = 0; i < data_amount; i++) {
        printf("%d", list[i]);
        printf(" ");
    }
    free(list);



}