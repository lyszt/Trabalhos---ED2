#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#define PROCESS "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\processo.txt"
#define OUTPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\saida.txt"
#else
#define INPUT "dados.txt"
#define PROCESS "processo.txt"
#define OUTPUT "saida.txt"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void cloneFile(const char* origin, const char* destination) {
    FILE *f = fopen(origin, "r");
    FILE *clone = fopen(destination, "w");
    if (f == NULL || clone == NULL) {
        printf("[ERRO] Não foi possível abrir os arquivos para clonagem.\n");
        if (f) fclose(f);
        if (clone) fclose(clone);
        return;
    }
    char content[1024];
    while (fgets(content, sizeof(content), f)) {
        fputs(content, clone);
    }
    fclose(f);
    fclose(clone);
}

void mergeSortedChunks(FILE *output) {
    // Feito no processo final
    FILE *temp = fopen("temp.txt", "r");
    if (!temp) {
        printf("[ERRO] Arquivo temporário não encontrado.\n");
        return;
    }

    int *values = NULL;
    int size = 0, capacity = 0;
    char num_str[16];

    while (fscanf(temp, "%15[0-9];", num_str) == 1) {
        if (size >= capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            values = realloc(values, capacity * sizeof(int));
            if(values == NULL) {
                printf("Erro na alocação");
                return;
            }
        }
        values[size++] = atoi(num_str);
    }
    fclose(temp);

    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (values[i] > values[j]) {
                int temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

    // Escreve os arquivos de volta pra OUTPUT
    for (int i = 0; i < size; i++) {
        fprintf(output, "%d;", values[i]);
    }

    free(values);
}

void externalSort(int chunk_size) {
    FILE *f = fopen(PROCESS, "r");
    if (!f) {
        printf("[ERRO] Arquivo não encontrado.\n");
        return;
    }

    int *numbers = malloc(chunk_size * sizeof(int));
    if (!numbers) {
        printf("[ERRO] Falha na alocação de memória.\n");
        fclose(f);
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("[ERRO] Não foi possível criar temp.txt.\n");
        fclose(f);
        free(numbers);
        return;
    }
    fclose(temp);

    int count = 0;
    char num_str[16];

    while (fscanf(f, "%15[0-9];", num_str) == 1) {
        numbers[count++] = atoi(num_str);
        if (count == chunk_size) {
            for (int i = 0; i < count - 1; i++) {
                for (int j = i + 1; j < count; j++) {
                    if (numbers[i] > numbers[j]) {
                        int tmp = numbers[i];
                        numbers[i] = numbers[j];
                        numbers[j] = tmp;
                    }
                }
            }
            // Manda os dados ordenados para arquivo temp
            temp = fopen("temp.txt", "a");
            for (int i = 0; i < count; i++) {
                fprintf(temp, "%d;", numbers[i]);
            }
            fclose(temp);
            count = 0;
        }
    }

    // Números restantes
    if (count > 0) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (numbers[i] > numbers[j]) {
                    int tmp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = tmp;
                }
            }
        }
        temp = fopen("temp.txt", "a");
        for (int i = 0; i < count; i++) {
            fprintf(temp, "%d;", numbers[i]);
        }
        fclose(temp);
    }

    fclose(f);
    free(numbers);

    // Fase de merge
    FILE *output = fopen(PROCESS, "w");
    if (!output) {
        printf("[ERRO] Falha ao abrir arquivo de saída.\n");
        return;
    }
    mergeSortedChunks(output);
    fclose(output);
    remove("temp.txt");
}

int main() {
    int quantidade_dados;
    printf("Quantidade de elementos por vez:\n");
    if (scanf("%d", &quantidade_dados) != 1 || quantidade_dados <= 0) {
        printf("[ERRO] Entrada inválida\n");
        return 0;
    }

    cloneFile(INPUT, PROCESS);
    externalSort(quantidade_dados);
    cloneFile(PROCESS, OUTPUT);
    remove(PROCESS);

    printf("Saída final disponível em [ SAIDA.TXT ]\n");
    return 0;
}