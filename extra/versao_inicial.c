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
void makeNumList(const int size, int* num_list) {
    FILE *f = fopen(INPUT, "r");

    // A função vai adicionando os caracteres dos números a uma string
    // até encontrar um caracter vazio, ponto e vírgula, ou o fim do arquivo

    if (f == NULL) {
        printf("[ERRO] Não foi possível abrir o arquivo, saindo.\n");
        return;
    }

    int i = 0;
    int j = 0;
    char number[16];
    char c;

    while ((c = fgetc(f)) != EOF && j < size) {
        if (c == ' ' || c == '\n') {
            if (i > 0) {
                number[i] = '\0';
                num_list[j++] = strtol(number, NULL, 10);
                i = 0;
            }
            continue;
        }

        if (c == ';') {
            if (i > 0) {
                number[i] = '\0';
                num_list[j++] = strtol(number, NULL, 10);
                i = 0;
            }
        } else if (i < 15) {
            number[i++] = c;
        }

        if (j >= size) {
            break;
        }
    }

    // Trata o último número, se necessário
    if (i > 0 && j < size) {
        number[i] = '\0';
        num_list[j++] = strtol(number, NULL, 10);
    }

    fclose(f);
}

// Sorting
void insertionSort(int input[], const int n) {
    if (n == 0){return;}
    if (input == NULL)
    {
        printf("[ERRO] Lista inválida.\n");
        return;
    }
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


void printIntList(int list[], const int size){
    if (list == NULL)
    {
        printf("[ERRO] Lista vazia ou inválida.\n");
        return;
    }
    for(int i = 0; i < size; i++) {
        printf("%d", list[i]);
        printf(" ");
    }
}

void intListOrderToFile(int list[], const int size)
{
    FILE *f = fopen(OUTPUT, "w");
    if (f == NULL)
    {
        printf("[ERRO] Não foi possível abrir o arquivo, saindo.\n");
        return;
    }
    if (list == NULL)
    {
        printf("Lista vazia, saindo. \n");
        return;
    }
    for (int i = 0; i < size; i++)
    {
        fprintf(f, "%d;", list[i]);
    }
    printf("LISTA ORGANIZADA:\n");
    printIntList(list, size);
    printf("- Disponível em %s\n", OUTPUT);
    // A lista no arquivo sai da mesma forma que foi lida, com ponto e vírgula

}

int main () {
    // Estou colocando 15 números
    int data_amount;
    printf("Insira a quantidade de elementos:\n");
    if (scanf("%d", &data_amount) != 1 || data_amount <= 0){
        printf("[ERRO] Entrada inválida\n");
        return 0;
    };
    int* list = malloc(data_amount * sizeof(int));
    if (list == NULL)
    {
        printf("[ERRO] Falha ao alocar memória.\n");
        return 1;
    }
    makeNumList(data_amount, list);

    printf("LISTA ORIGINAL:\n");
    printIntList(list, data_amount);
    insertionSort(list, data_amount);
    // Insere a lista em um arquivo, numeros separados via ";"
    intListOrderToFile(list, data_amount);
    free(list);

    return 0;
}