#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#define PROCESS_N "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\processo_%d.txt"
#define OUTPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\saida.txt"
#else
#define INPUT     "dados.txt"
#define PROCESS_N "processo_%d.txt"
#define OUTPUT    "saida.txt"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Other functions
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// External
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

int count_numbers_in_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    int count = 0;
    int num;

    // O e 1 são para verificar se o fscanf funcionou
    while (fscanf(file, "%*[^0-9-]") == 0 || fscanf(file, "%d", &num) == 1) {
        count++;
    }

    fclose(file);
    return count;
}

void externalSort(int chunk_size) {
    // 1. Verificar quantidade total de números
    int total = count_numbers_in_file(INPUT);
    if(total <= 0) {
        printf("Erro ao ler arquivo de entrada\n");
        return;
    }

    // 2. Calcular quantos arquivos temporários serão criados
    int num_arquivos = (total + chunk_size - 1) / chunk_size;
    char nome_arquivo[256];

    // 3. Ler, ordenar e salvar em arquivos temporários
    FILE *entrada = fopen(INPUT, "r");
    for(int i = 0; i < num_arquivos; i++) {
        // 3.1 Calcular tamanho deste bloco
        int tamanho;
        if (i == num_arquivos - 1) {
            tamanho = total % chunk_size;
        } else {
            tamanho = chunk_size;
        }

        // 3.2 Ler números do arquivo original
        int *bloco = malloc(tamanho * sizeof(int));
        for(int j = 0; j < tamanho; j++) {
            fscanf(entrada, "%d", &bloco[j]);
            while(fgetc(entrada) == ';');
        }

        // 3.3 Ordenar e salvar em arquivo temporário
        qsort(bloco, tamanho, sizeof(int), compare);
        sprintf(nome_arquivo, PROCESS_N, i);
        FILE *temp = fopen(nome_arquivo, "w");
        for(int j = 0; j < tamanho; j++) fprintf(temp, "%d;", bloco[j]);
        fclose(temp);
        free(bloco);
    }
    fclose(entrada);

    // 4. Fazer merge dos arquivos temporários
    FILE **arquivos = malloc(num_arquivos * sizeof(FILE*));
    int *valores = malloc(num_arquivos * sizeof(int));

    // 4.1 Abrir todos os arquivos e ler primeiro valor
    for(int i = 0; i < num_arquivos; i++) {
        sprintf(nome_arquivo, PROCESS_N, i);
        arquivos[i] = fopen(nome_arquivo, "r");
        fscanf(arquivos[i], "%d;", &valores[i]);
    }

    // 4.2 Escrever resultado ordenado
    FILE *saida = fopen(OUTPUT, "w");
    while(1) {
        // Encontrar menor valor atual
        int menor_valor = INT_MAX;
        int indice_menor = -1;

        for(int i = 0; i < num_arquivos; i++) {
            if(valores[i] < menor_valor) {
                menor_valor = valores[i];
                indice_menor = i;
            }
        }

        // Se todos valores forem processados
        if(indice_menor == -1) break;

        // Escrever e atualizar
        fprintf(saida, "%d;", menor_valor);
        if(fscanf(arquivos[indice_menor], "%d;", &valores[indice_menor]) != 1) {
            valores[indice_menor] = INT_MAX; // Marcar como finalizado
        }
    }

    // 5. Limpeza final
    for(int i = 0; i < num_arquivos; i++) {
        fclose(arquivos[i]);
        sprintf(nome_arquivo, PROCESS_N, i);
        remove(nome_arquivo);
    }
    free(arquivos);
    free(valores);
    fclose(saida);
}

int main() {
    int quantidade_dados;
    printf("Quantidade de elementos por vez:\n");
    if (scanf("%d", &quantidade_dados) != 1 || quantidade_dados <= 0) {
        printf("[ERRO] Entrada inválida\n");
        return 0;
    }

    externalSort(quantidade_dados);

    printf("Saída final disponível em [ SAIDA.TXT ]\n");
    return 0;
}