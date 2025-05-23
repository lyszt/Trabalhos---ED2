#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\dados.txt"
#define EMPTY_VALUE -1
#else
// Mudei o sistema de arquivos, caso não consiga ler o arquivo, por favor mude o caminho aqui
#define INPUT "Trabalho T2/dados.txt"
#define EMPTY_VALUE -1
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
int makeNumList(int* hash_table_type, int *hash_size_ptr, int** num_list_ptr, int *num_count_ptr) {
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

    char hash_type_string[1000];
    if (fgets(hash_type_string, sizeof(hash_type_string), f) == NULL) {
        printf("[ERRO] Erro lendo o arquivo dados.txt\n");
        fclose(f);
        return 0;
    }
    int hash_type = strtol(hash_type_string, NULL, 10);
    fgetc(f);

    char number_line[1000];
    if (fgets(number_line, sizeof(number_line), f) == NULL) {
        printf("[ERRO] Erro lendo o arquivo dados.txt\n");
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
        printf("[ERRO] Erro lendo o arquivo dados.txt\n");
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
    *hash_table_type = hash_type;

    return 0;
}



void printLinearHashTable(int* hash_table, int size) {
    for (int i = 0; i < size; i++) {
        if (hash_table[i] == EMPTY_VALUE) {
            printf("Index %d: NULL\n", i);
        } else {
            printf("Index %d: %d\n", i, hash_table[i]);
        }
    }
}

void printNodeHashTable(Node** table, int size) {
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

void makeNodeHashTable(int hash_table_size, int data_count, int* number_list)
{
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
    printNodeHashTable(hash_table, hash_table_size);
    // Função usada pra achar números
    // Eu não dou escolha aqui ao usuário de escolher o que está procurando, já que o trabalho não especificava
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
}

void makeLinearHashTable(int hash_table_size, int data_count, int* number_list){
        int* hash_table = (int*)malloc(hash_table_size * sizeof(int));
    // Valores vazios são declarados como -1, supondo que nenhum dos valores venha a ser negativo
    // Caso isso não seja o caso, define-se no topo da função a global EMPTY_VALUE
        for (int i = 0; i < hash_table_size; i++) {
            hash_table[i] = EMPTY_VALUE;
        }

        for (int i = 0; i < data_count; i++) {
            int value = number_list[i];
            int index = value % hash_table_size;

            int attempts = 0;

            while (hash_table[index] != EMPTY_VALUE) {
                index = (index + 1) % hash_table_size;
                attempts++;

                if (attempts == hash_table_size) {
                    printf("[ERRO] Tabela hash cheia. Não é possível inserir o valor %d\n", value);
                    break; // or return; depending on what behavior you want
                }

            hash_table[index] = value;
        }
    free(hash_table);
    printLinearHashTable(hash_table, hash_table_size);

}

int main()
{

    int* number_list;
    int data_count = 0;
    int hash_table_size = 0;
    int hash_table_type;
    // A função manda os valores direto pro endereço das variáveis declaradas acima
    makeNumList(&hash_table_type, &hash_table_size, &number_list, &data_count);
    switch (hash_table_type)
    {
        case 0:
            makeNodeHashTable(hash_table_size, data_count, number_list);
        break;
        case 1:
            makeLinearHashTable(hash_table_size, data_count, number_list);
        break;
        default:
            printf("Por favor insira um tipo de tabela hash válido (entre 1 e 2).\n");

    }


    return 0;
}