#ifdef _WIN32
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#endif
#ifdef linux
#define INPUT "dados.txt"
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT "saida.txt"



// Function for reading data
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
// Sorting
void insertionSort(int input[], int n) {
    for (int i = 0; i < n; i++)
    {
        int key = input[i];
        int j = i - 1;

        while ( j >= 0 && input[j] > key )
        {
            input[j + 1 ] = input[j];
            j = j - 1;
        }
        input[j + 1] = key;
    }
}

void printIntList(int list[], int size){
    for(int i = 0; i < size; i++) {
        printf("%d", list[i]);
        printf(" ");
    }
}
int main () {
    // I'm inputting 15 numbers
    int data_amount;
    printf("Insira a quantidade de elementos:\n");
    scanf("%d", &data_amount);
    int* list = malloc(data_amount * sizeof(int));
    makeNumList(data_amount, list);

    printf("LISTA ORIGINAL:\n");
    printIntList(list, data_amount);
    free(list);



}