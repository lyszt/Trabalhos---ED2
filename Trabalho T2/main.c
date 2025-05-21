#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#define OUTPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\saida.txt"
#else
// Mudei o sistema de arquivos, caso não consiga ler o arquivo, por favor mude o caminho aqui
#define INPUT "Trabalho T2/dados.txt"
#endif
#include <stdio.h>
#include <stdlib.h>

// Para utilizar-se na hash-table
typedef struct Node
{
    int value;
    struct Node *next;
} Node;


// Function for reading data
int makeNumList(int *hash_size_ptr, int** num_list_ptr, int *num_count_ptr) {
    FILE *f = fopen(INPUT, "r");

    // A função vai adicionando os caracteres dos números a uma string
    // até encontrar um caracter vazio, ponto e vírgula, ou o fim do arquivo
    // Ela ignora o primeiro número e guarda ele, e no fim ela retorna a quantidade de dados e os numeros

    if (f == NULL) {
        printf("[ERRO] Não foi possível abrir o arquivo, saindo.\n");
        return -1;
    }


    // Pega primeira linha e aloca lista do tamanho dos numeros
    char data_count_string[1000];
    fscanf(f, "%[^\n]", data_count_string);
    int hash_list_size = strtol(data_count_string, NULL, 10);

    fgetc(f);
    // Limpar o buffer

    char number_line[1000];
    if (fgets(number_line, sizeof(number_line), f) == NULL) {
        printf("[ERRO] Erro lendo o arquivo dados.txt");
        fclose(f);
        return 0;
    }

    // Count = 1 para caso o último número não tenha ;
    int count = 1;
    for (int i = 0; number_line[i]; ++i) {
        if (number_line[i] == ';') count++;
    }

    // Aloca lista baseada na quantidade de numeros
    int* num_list = malloc(count * sizeof(int));
    if (num_list == NULL)
    {
        printf("[ERRO] Erro lendo o arquivo dados.txt");
        fclose(f);
        return -1;
    }

    int i = 0;
    int j = 0;
    char number[16];

    // Adiciona os números em uma string até achar ";", depois converte pra int
    for (int k = 0; number_line[k] && j < count; ++k) {
        char c = number_line[k];
        if (c == ';' || c == '\n' || c == '\0') {
            if (i > 0) {
                number[i] = '\0';
                num_list[j++] = strtol(number, NULL, 10);
                i = 0;
            }
        } else if (i < 15) {
            number[i++] = c;
        }

    }

    // Retorna o endereço pro tamanho da hash_table e a lista de numeros pro input da função
   *hash_size_ptr = hash_list_size;
    *num_count_ptr = count;
    *num_list_ptr = num_list;

    return 0;
}


void printHashTable(Node** table, int size) {
    for (int i = 0; i < size; i++) {
        printf("Index %d:", i);
        Node* current = table[i];
        while (current != NULL) {
            printf(" %d ->", current->value);
            current = current->next;
        }
        printf(" NULL\n");
    }
}

int findInHashTable(int num, Node** hash_table, int hash_size)
{
    int target_index = num % hash_size;
    if (hash_table[target_index] == NULL)
    {
        return -1;
    }
    return target_index;
}


int main()
{
    int* number_list;
    int data_count = 0;
    int hash_table_size = 0;
    // A função manda os valores direto pro endereço das variáveis declaradas acima
    makeNumList(&hash_table_size, &number_list, &data_count);

    // Forma supostamente mais segura de alocar um array de pointers pra uma struct, inicializa todos os pointers pra null
    Node** hash_table = (Node**)calloc(hash_table_size, sizeof(Node*));
    for (int i = 1; i < data_count; i++)
    {
        int target_index = number_list[i] % hash_table_size;
        if (hash_table[target_index] == NULL)
        {
            hash_table[target_index] = (Node*)malloc(sizeof(Node));
            hash_table[target_index]->value = number_list[i];
            hash_table[target_index]->next = NULL;
        }
        else
        {
            Node *current = hash_table[target_index];
            while (current->next != NULL)
            {
                current = current->next;
            }
            Node* next_hash = (Node*)malloc(sizeof(Node));
            next_hash->value = number_list[i];
            next_hash->next = NULL;
            current->next = next_hash;
        }

    }
    printHashTable(hash_table, hash_table_size);
    // Função usada pra achar números
    int number_index_if_exists = findInHashTable(97, hash_table, hash_table_size);
    if (number_index_if_exists == -1)
    {
        printf("O número não foi encontrado na hash_table.");
    }
    else
    {
        printf("O número procurado e indicado na função está no índice %d\n",number_index_if_exists);
    }
    free(hash_table);
    return 0;
}