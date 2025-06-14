#define degree 3
// Não consegui ter o mesmo resultado do doc com o degrau 4, somente com o 3
// devo subtrair aqui?
#ifdef _WIN32
// Configurado para o meu computador, alterar se for usar no Windows
#define INPUT "C:\\Users\\neoka\\CLionProjects\\Trabalho---POD\\Trabalho_TF\\dados.txt"
#else
#define INPUT "dados.txt"
#endif
// Imports
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>


typedef struct Node
{   
    // Nessa struct o valor é uma lista de ponteiros
    int* value;
    // Número de chaves válidas neste nó.
    int num_keys;
    // is_leaf é necessário para saber se o espaço alocado é uma chave ou guarda um valor real
    bool is_leaf;
    struct Node* next;
    struct Node** children;
} Node;


// função pro qsort
// Se der negativo, 'a' vem primeiro. Se der positivo, 'b' vem primeiro.
int compare(const void* a,const void* b) {
  	return *(int*)a - *(int*)b;
}

// comparação pro qsort pra nós
int compare_nodes(const void* a, const void* b) {
    Node* node_a = *(Node**)a;
    Node* node_b = *(Node**)b;
    return node_a->value[0] - node_b->value[0];
}



// Função usada pra fazer os Nodes
struct Node* makeNode(bool is_leaf){
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL ){
        printf("[ERRO] Erro na alocação de memória.");
        return NULL;
    }
    node->num_keys = 0;
    node->next = NULL;
    node->is_leaf = is_leaf;

    // Aloca espaço para o número de valores (M - 1)
    node->value = (int*)malloc((degree - 1) * sizeof(int));
    // Aloca espaço para o número máximo de ponteiros (M)
    node->children = (Node**)malloc(degree * sizeof(Node*));

    // Inicializa ponteiros como nulos
    for(int i = 0; i < degree; i++) {
        node->children[i] = NULL;
    }
    
    return node;
}

int find_left(Node* node) {
    // Função pra encontrar o elemento mais a esquerda
    // pra achar o menor número de uma sub-árvore depois de ordenada
    if (node == NULL) {
        return -1; 
    }
    Node* current = node;
    while (!current->is_leaf) {
        current = current->children[0];
    }
    return current->value[0];
}

void buildNodeFromList(Node** root, int* num_list, int num_count){
    // Precisamos receber um ponteiro duplo aqui
    // o endereço do node
    if (num_count <= 0) {
        printf("[ERRO] Não é possível alocar uma lista com 0 números.");
        return;
    }
    
    int full_node_size = degree -1;
    // Calcula o número mínimo de folhas necessárias
    // A quantidade de folhas deve garantir que cada folha tenha ao menos o minimo de elementos,
    // O cálculo é arredondado para cima
    // número total de itens dividido pelo número mínimo de chaves por folha.

    // int min_keys = (full_node_size + 1) / 2; 
    int leaves = (num_count + full_node_size - 1) / full_node_size;
    // Cria uma lista para armazenar
    Node** leaves_list = (Node**)malloc(leaves * sizeof(Node*));

    if(leaves_list == NULL ){
        printf("[ERRO] Falha na alocação de memória.");
        return;
    }
    int original_i = 0;
    for(int i = 0; i < leaves; i++){
        // criando um nó folha, que armazenará valores reais
        leaves_list[i] = makeNode(true);

        // Copia as chaves da lista original para o novo nó folha
        // O loop copia no máximo M - 1 valores ou até que todos os valores sejam processados
        int keys = 0;
        while (original_i < num_count && keys < full_node_size) {
            // Copia o valor atual da lista original para a posição correspondente no nó folha
            leaves_list[i]->value[keys] = num_list[original_i];
            keys++;
            original_i++;
        }
        // Atualiza o número de chaves armazenadas neste nó folha
        leaves_list[i]->num_keys = keys;
        qsort(leaves_list[i]->value, keys, sizeof(int), compare);
        // necessario, se não o nó fica fora de ordem

    }
    
    // ligando os nós entre si
    for (int i = 0; i < leaves - 1; i++) {
        // um == proximo na lista e em diante
        leaves_list[i]->next = leaves_list[i + 1];
    }

    int current_level_nodes = leaves;
    Node** nodes_for_level = leaves_list;
    
    // Existe a possibilidade de dar free em uma memória nunca alocada aqui?

    while (current_level_nodes > 1) {
    // Calcula quantos nós pais serão necessários no próximo nível
    int num_parent_nodes = (current_level_nodes + degree - 1) / degree;

    // Aloca memória para a lista de nós pais
    Node** parent_nodes_list = (Node**)malloc(num_parent_nodes * sizeof(Node*));
    if(parent_nodes_list == NULL ){
        printf("[ERRO] Falha na alocação de memória.");
        return;
    }

    int child_index = 0;
    for (int i = 0; i < num_parent_nodes; i++) {
        // nó de valores que contem valores e não chaves
        parent_nodes_list[i] = makeNode(false);

        int keys_added_to_parent = 0;
         // conecta os nós filhos ao nó pai atual
        // Cada pai agrupará até degree filhos do nível inferior.

        for (int j = 0; j < degree && child_index < current_level_nodes; j++) {
            Node* child = nodes_for_level[child_index];
            parent_nodes_list[i]->children[j] = child;

            // Promove a primeira chave do filho para o nó pai,
            // exc para o primeiro filho (j > 0)
            if (j > 0) {
                // tive que fazer uma função pra encontrar o primeiro elemento do primeiro filho
                parent_nodes_list[i]->value[keys_added_to_parent] = find_left(child);
                keys_added_to_parent++;
            }
            child_index++;
        }
        // Atualiza o número de chaves armazenadas no nó pai
        parent_nodes_list[i]->num_keys = keys_added_to_parent;
    }
    free(nodes_for_level); 
     // free na lista do nível que acabamos de processar

    // O nível de pais recém-criado se torna o nível atual para a próxima iteração.
    nodes_for_level = parent_nodes_list;
    current_level_nodes = num_parent_nodes;
    

}   
    // Ao final do while nodes_for_level vai ter só a raiz da árvore
    *root = nodes_for_level[0];
    
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
    // importante, se nao deixar a lista ordenada nao vai
    qsort(num_list, count, sizeof(int), compare);
    *num_list_ptr = num_list;

    return 0;
}

// Função recursiva para imprimir a árvore
// filho da esquerda, chave do pai, filho da direita
void print_tree_recursive(Node* node, const char* key_prefix) {
    if (node == NULL) {
        printf("[ERRO] Um nó vazio no nó foi encontrado, saindo.");
        return;
    }
    // Se o nó é uma folha, imprime com o prefixo de folha "--"
     if (node->is_leaf) {
        printf("|--");
        // Este if/else evita um espaço extra no final da linha.
        for (int i = 0; i < node->num_keys; i++) {
            if(i != node->num_keys - 1) {
                printf("%d ", node->value[i]);
            } else {
                printf("%d", node->value[i]);
            }
        }
        printf("\n");
    } else { 
        //Vai indo pelas chaves printando os valores
        // Se for um nó interno, percorre suas chaves e filhos.
        for (int i = 0; i < node->num_keys; i++) {
            print_tree_recursive(node->children[i], "|-");
            printf("%s%d\n", key_prefix, node->value[i]);
        }
        // chama a função para o último filho, o maior / mais a direita
        print_tree_recursive(node->children[node->num_keys], "|-");
    }
}

// Função principal de impressão. Ela apenas inicia o processo recursivo
void printTree(Node* root) {
    if (root == NULL) {
        printf("[ERRO] A arvore esta vazia.\n");
        return;
    }
    print_tree_recursive(root, "|");
}

// Libera toda a memória usada pela árvore.
// Navega até os nós mais baixos e vai liberando a memória na volta,
void freeTree(Node* node) {
    // entra nas chaves e vai em todos os filhos pra liberar
    if (node == NULL) {
        return;
    }
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            freeTree(node->children[i]);
        }
    }
    free(node->value);
    free(node->children);
    free(node);
}


int main() {
    int* num_list = NULL;
    int num_count = 0;

    // Lê os dados do arquivo para a memória.
    makeNumList(&num_list, &num_count);
    //Monta a árvore B+ com os dados lidos
    Node* root = NULL;
    buildNodeFromList(&root, num_list, num_count);
    printTree(root);
    free(num_list);
    freeTree(root);
    return 0;
}


