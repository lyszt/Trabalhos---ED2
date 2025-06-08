#define degree 4
#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\Trabalho_TF\\dados.txt"
#else
#define INPUT "dados.txt"
#endif
#define EMPTY_VALUE -1
#include <stdio.h>
#include <stdlib.h>
// To começando a odiar o CLION

// Estrutura necessária pra fazer a árvore b+
typedef struct Node
{
    // Nessa struct o valor é uma lista
    int* value;
    struct Node *right;
    struct Node *left;
} Node;


struct Node* makeNode(int* input){
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = input;
    // estrutura de dados onde cada nodo tem uma lista de m - 1 items 
    // Já se espera que a quantidade seja certa, não há verificações
    node->right = NULL;
    node->left = NULL;
    return node;
}

void buildNodeFromList(Node* root, int* num_list, int num_count){
    // Primeiro, alocar uma lista com o tamanho dos degraus
    int * chunk_list;
    for(int i = 0; i < num_count; i += degree){
        // Depois que aloca M-1, cria novo chunk_list pra numeros novos
        int* chunk_list = malloc(degree * sizeof(int));
        for(int j = 0; j < degree; j++){
            // Lista de M-1
            if (i + j < num_count) {
                // i + j, atual + indice lista
                chunk_list[j] = num_list[i + j];
            }
        }
         // Terminando, coloca a arvore no root
        if(root->left == root->right == NULL && root->value == NULL) {
            Node* node = makeNode(chunk_list);
            if(node == NULL){
                printf("[ ERRO ] Erro na alocação de memória.");
                return;
            }
            *root = *node;
            // Na nossa situação atual, mesmo que foi criado um root na main,
            // Ela vai sobrescrever com os valores m-1 
        }
    } // n²
    free(chunk_list);
    // Existe a possibilidade de dar free em uma memória nunca alocada aqui?
    
}

// Function for reading data
int makeNumList(int** num_list_ptr, int *num_count_ptr) {
    FILE *f = fopen(INPUT, "r");

    // A função vai adicionando os caracteres dos números a uma string
    // até encontrar um caracter vazio, ponto e vírgula, ou o fim do arquivo
    // Ela ignora o primeiro número e guarda ele, e no fim ela retorna a quantidade de dados e os numeros

    if (f == NULL) {
        printf("[ERRO] Não foi possível abrir o arquivo, saindo.\n");
        return -1;
    }


    // Pega primeira linha e aloca lista do tamanho dos numeros

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
    // Foi realmente necessário tratar o último número
    if (i > 0 && j < count) {
        number[i] = '\0';
        num_list[j++] = strtol(number, NULL, 10);
    }


    

    // Retorna a lista de numeros pro input da função
    *num_count_ptr = count;
    *num_list_ptr = num_list;

    return 0;
}

int main(){
    int* num_list;
    int num_count;

    makeNumList(&num_list, &num_count);
    for(int i = 0; i < num_count; i++){
      printf("%d\n", num_list[i]);
    }
}

// Personal notes
// I wonder if we can't just put lists of degrees inside the nodes 