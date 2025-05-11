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
    if (!file) {
        printf("[ERRO] Erro abrindo o arquivo.");
        return -1;
    }

    int count = 0;
    int num;

    printf("Iniciando contagem em %s...\n", filename);

    while(1) {
        // Pula caracteres não numéricos
        int discarded = fscanf(file, "%*[^0-9]");
        if(discarded == EOF) break; // Fim do arquivo

        // Tenta ler um número
        int read = fscanf(file, "%d", &num);

        if(read == 1) {
            count++;
        }
        else {
            printf("Caractere inválido encontrado. Parando busca.\n");
            break;
        }
    }

    fclose(file);
    printf("Total de números encontrados: %d\n\n", count);
    return count;
}

void externalSort(int chunk_size) {
    printf("\n=== INICIANDO ORDENACAO EXTERNA ===\n");
    printf("Tamanho dos blocos: %d elementos\n", chunk_size);

    // 1. Verificar quantidade total de números
    printf("\n[PASSO 1] Contando numeros no arquivo...\n");
    int total = count_numbers_in_file(INPUT);
    if(total == -1) {
        printf("Falha contando quantidade de números no arquivo.\n");
        return;
    }
    if(total <= 0) {
        printf("Erro ao ler arquivo de entrada\n");
        return;
    }
    printf("Total de numeros encontrados: %d\n", total);

    // 2. Calcular quantos arquivos temporários serão criados
    int num_arquivos = (total + chunk_size - 1) / chunk_size;
    printf("\n[PASSO 2] Criando %d arquivos temporarios\n", num_arquivos);
    char nome_arquivo[256];

    // 3. Ler, ordenar e salvar em arquivos temporários
    printf("\n[PASSO 3] Processando blocos:\n");
    FILE *entrada = fopen(INPUT, "r");
    for(int i = 0; i < num_arquivos; i++) {
        // Calcular tamanho do bloco
        int tamanho;
        if (i == num_arquivos - 1) {
            tamanho = total % chunk_size;
        } else {
            tamanho = chunk_size;
        }

        printf("\n-> Bloco %d/%d (%d elementos)\n", i+1, num_arquivos, tamanho);

        // Ler números
        printf("   Lendo do arquivo original...\n");
        int *bloco = malloc(tamanho * sizeof(int));
        for(int j = 0; j < tamanho; j++) {
            fscanf(entrada, "%d", &bloco[j]);
            while(fgetc(entrada) == ';');
        }

        // Ordenar e salvar
        printf("   Ordenando...\n");
        qsort(bloco, tamanho, sizeof(int), compare);

        sprintf(nome_arquivo, PROCESS_N, i);
        printf("   Salvando no arquivo temporario: %s\n", nome_arquivo);
        FILE *temp = fopen(nome_arquivo, "w");
        for(int j = 0; j < tamanho; j++) fprintf(temp, "%d;", bloco[j]);
        fclose(temp);
        free(bloco);
    }
    fclose(entrada);

    // 4. Fazer merge dos arquivos temporários
    printf("\n[PASSO 4] Iniciando merge dos arquivos:\n");
    // Ponteiro duplo é um array de arquivos FILE
    // Isso facilita aqui pra baixo
    FILE **arquivos = malloc(num_arquivos * sizeof(FILE*));
    int *valores = malloc(num_arquivos * sizeof(int));

    // Abrir arquivos temporários
    printf("Abrindo arquivos para merge...\n");
    for(int i = 0; i < num_arquivos; i++) {
        sprintf(nome_arquivo, PROCESS_N, i);
        arquivos[i] = fopen(nome_arquivo, "r");
        fscanf(arquivos[i], "%d;", &valores[i]);
    }

    // Escrever resultado ordenado
    int elementos_processados = 0;
    FILE *saida = fopen(OUTPUT, "w");
    printf("\n[PASSO 5] Gerando arquivo final:\n");
    while(1) {
        // Encontrar menor valor
        int menor_valor = INT_MAX;
        int indice_menor = -1;

        for(int i = 0; i < num_arquivos; i++) {
            if(valores[i] < menor_valor) {
                menor_valor = valores[i];
                indice_menor = i;
            }
        }

        if(indice_menor == -1) break;

        // Escrever e atualizar
        fprintf(saida, "%d;", menor_valor);
        elementos_processados++;

        if(elementos_processados % 1000 == 0) {
            printf("-> Progresso: %d elementos ordenados\n", elementos_processados);
        }

        if(fscanf(arquivos[indice_menor], "%d;", &valores[indice_menor]) != 1) {
            printf("Arquivo %d esvaziado\n", indice_menor);
            valores[indice_menor] = INT_MAX;
        }
    }

    // 5. Limpeza final
    printf("\n[PASSO 6] Finalizando processo:\n");
    for(int i = 0; i < num_arquivos; i++) {
        fclose(arquivos[i]);
        sprintf(nome_arquivo, PROCESS_N, i);
        remove(nome_arquivo);
        printf("Removido arquivo temporario: %s\n", nome_arquivo);
    }
    free(arquivos);
    free(valores);
    fclose(saida);

    printf("\n=== PROCESSO CONCLUIDO ===\n");
    printf("Arquivo final gerado: %s\n", OUTPUT);
    printf("Total de elementos ordenados: %d\n\n", elementos_processados);
}
int main() {
    int quantidade_dados;
    printf("Quantidade de elementos por vez:\n");
    scanf("%d", &quantidade_dados);
    printf("Pegou os dados. Fazendo external sort...");
    externalSort(quantidade_dados);

    printf("Saída final disponível em [ SAIDA.TXT ]\n");
    return 0;
}