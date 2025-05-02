#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#define PROCESS "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\processo.txt"
#define OUTPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\saida.txt"
#endif
#ifdef linux
#define INPUT "dados.txt"
#define PROCESS "processo.txt"
#define OUTPUT "saida.txt"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_LENGTH 16

void cloneFile(const char* origin, const char* destination) {
    FILE *f = fopen(origin, "r");
    FILE *clone = fopen(destination, "w");
    if (f == NULL) {
        printf("[ERRO] Não foi possível ler o arquivo.");
        return;
    }
    char content[1024];
    while (fgets(content, sizeof(content), f) != NULL) {
        fputs(content, clone);
    }

    fclose(f);
    fclose(clone);
}

void externalSort(int chunk_size) {
    FILE *f = fopen(PROCESS, "r+");
    if (f == NULL) {
        printf("[ERRO] Não foi possível abrir o arquivo.\n");
        return;
    }

    char num_str[MAX_NUM_LENGTH];
    int *numbers = (int *)malloc(chunk_size * sizeof(int));
    if (numbers == NULL) {
        printf("[ERRO] Não foi possível alocar memória.\n");
        fclose(f);
        return;
    }

    int count = 0;
    FILE *temp_file = fopen("temp.txt", "w");

    // Le o arquivo a partir da quantidade dada pelo usuário
    while (fscanf(f, "%15[0-9];", num_str) == 1) {
        numbers[count++] = atoi(num_str);

        if (count == chunk_size) {

            for (int i = 0; i < count - 1; i++) {
                for (int j = i + 1; j < count; j++) {
                    if (numbers[i] > numbers[j]) {
                        int aux = numbers[i];
                        numbers[i] = numbers[j];
                        numbers[j] = aux;
                    }
                }
            }
            if (temp_file == NULL) {
                printf("[ERRO] Não foi possível criar arquivo temporário.\n");
                fclose(f);
                free(numbers);
                return;
            }
            for (int i = 0; i < count; i++) {
                fprintf(temp_file, "%d;", numbers[i]);
            }
            fclose(temp_file);
            count = 0; // Reset for the next chunk
        }
    }

    if (count > 0) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (numbers[i] > numbers[j]) {
                    int temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }
            }
        }
        temp_file = fopen("temp.txt", "w");
        if (temp_file == NULL) {
            printf("[ERRO] Não foi possível criar arquivo temporário.\n");
            fclose(f);
            free(numbers);
            return;
        }
        for (int i = 0; i < count; i++) {
            fprintf(temp_file, "%d;", numbers[i]);
        }
        fclose(temp_file);
    }

    fseek(f, 0, SEEK_SET);
    temp_file = fopen("temp.txt", "r");
    if (temp_file == NULL) {
        printf("[ERRO] Não foi possível abrir arquivo temporário para mesclar.\n");
        fclose(f);
        free(numbers);
        return;
    }
    // Colocando de volta no arquivo
    char c;
    while ((c = fgetc(temp_file)) != EOF) {
        fputc(c, f);
    }

    fclose(f);
    fclose(temp_file);
    free(numbers);
}

int main() {
    int quantidade_dados;
    printf("Quantidade de elementos por vez:\n");
    if (scanf("%d", &quantidade_dados) != 1 || quantidade_dados <= 0) {
        printf("[ERRO] Entrada inválida\n");
        return 0;
    }
    // Clona o arquivo e coloca em processo.txt
    cloneFile(INPUT, PROCESS);
    // Usa ordenação externa para ordenar o arquivo
    // Usar 100
    externalSort(quantidade_dados);
    cloneFile(PROCESS, OUTPUT);
    // Apaga o arquivo que usamos para ordenar
    remove(PROCESS);
    printf("Saída final disponível em [ SAIDA.TXT ]");

    return 0;
}
