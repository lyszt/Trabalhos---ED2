// Trabalho TP1

//  Trabalho Prático: Sistema de Registos de Alunos em Arquivo Binário

//  Objetivo:

//  Implementar as funções CRUD (Create, Read, Update, Delete) para um  sistema de registos de alunos, manipulando diretamente
// um arquivo.  O foco é a eficiência, realizando operações "in-place" (no local)  sempre que possível, sem reescrever o arquivo
// inteiro.

//  Testes:

//  1- Criar um arquivo inicial com 5 alunos 

//    typedef struct {     int id;     char nome[TAMANHO_NOME];     float media;     int ativo; // 1 para ativo, 0 para "apagado" (deleção
// lógica) } Aluno;

// 2- Listar todos os alunos ativos 

// 3- Atualizar a média do aluno com ID 3 para 9.5

// 4- Apagar (logicamente) o aluno com ID 2 

// 5-  Atualizar um aluno que não existe (ID 99)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Originalmente foi feito em .txt mas vi que o trabalho pede um arquivo binário
#define ARQUIVO "./alunos.bin"
#define MAXIMO_LINHA 2048

typedef struct {
    int id; 
    char nome[50];
    float media; 
    int ativo;
} Aluno; 

void criarListaDeAlunos(Aluno* lista_alunos, int quantidade_alunos) {
    FILE *f = fopen(ARQUIVO, "wb");

    for(int i = 0; i < quantidade_alunos; i++){
        fwrite(&lista_alunos[i], sizeof(Aluno), 1, f);
    }
    fclose(f);    

}

Aluno* getAluno(FILE *f, int id){
    rewind(f);
    Aluno* aluno = malloc(sizeof(Aluno));
    if(fread(aluno, sizeof(Aluno), 1, f) != 1){
        free(aluno);
        return NULL;
    }
    return aluno;
}


void ListAlunos(FILE *f, int id, int opts){
    // id == 0 == listar todos
    // opts == 1, 2 == ativo, inativo
    Aluno aluno;
    rewind(f);
    while (fread(&aluno, sizeof(Aluno), 1, f) == 1) {
        if (id != 0 && aluno.id != id)
            continue;
        if ((opts == 1 && aluno.ativo != 1) || (opts == 2 && aluno.ativo != 0))
            continue;
        printf("ID: %d\n", aluno.id);
        printf("Nome: %s\n", aluno.nome);
        printf("Média: %.1f\n", aluno.media);
        printf("Status: %s\n", aluno.ativo == 1 ? "Ativo" : "Inativo");
        printf("\n");
    }
}

void atualizarMedia(FILE *f, int id, float media){
    rewind(f);
    Aluno* aluno = malloc(sizeof(Aluno));
    while (fread(aluno, sizeof(Aluno), 1, f) == 1) {
        if (aluno->id != id)
            continue;
        fseek(f, -sizeof(Aluno), SEEK_CUR);
        aluno->media = media;
        fwrite(aluno, sizeof(Aluno), 1, f);
        printf("Média de %s atualizada.\n", aluno->nome);
        break;
    }
    free(aluno);
}


void apagarAluno(FILE *f, int id){
    rewind(f);
    Aluno* aluno = malloc(sizeof(Aluno));
    while (fread(aluno, sizeof(Aluno), 1, f) == 1) {
        if (aluno->id != id)
            continue;
        fseek(f, -sizeof(Aluno), SEEK_CUR);
        aluno->ativo = 0;
        fwrite(aluno, sizeof(Aluno), 1, f);
        printf("Aluno %s foi marcado como inativo/apagado.\n", aluno->nome);
        break;
    }
    free(aluno);
}


void callSystem(FILE *f, int quantidade_alunos){
    int choice;
    while(1){
        printf("Sistema de registro de Alunos em Arquivo Binário\n");
        printf("1. Listar alunos ativos\n");
        printf("2. Listar alunos inativos\n");
        printf("3. Atualizar media\n");
        printf("4. Apagar aluno\n\n");
        printf("Inserir qualquer outro número para sair.");
        scanf("%d", &choice);
        while(getchar() != '\n');
        switch(choice){
            case 1:
            // Listar todos alunos sem se importar com id que estão ativos
                ListAlunos(f, 0, 1);
                break;
            case 2:
            // Listar todos alunos sem se importar com id que estão inativos
                ListAlunos(f, 0, 2);
                break;
            case 3:
            // Primeiro pegar media nova
                float new_grade;
                int id;
                printf("Inserir o id e a nova média (id novamedia):");
                scanf("%d %f", &id, &new_grade);
                while(getchar() != '\n');
                atualizarMedia(f, id, new_grade);
                break;
            case 4:
            // Igual a função de cima mas muda o parametro
                printf("Inserir o id para a remoção:");
                scanf("%d", &id);
                while(getchar() != '\n');
                apagarAluno(f, id);
                break;

            default:
                return;
        }
    }
}

int main(){
    // Usei essa função para criar os dados dinamicamente
    char* nomes[] = {"Pedro","Carlos","Ronaldo","Rodolfo", "Miau"};
    float medias[] = {5.5,4.5,3.0,1.0,2.0};
    int status[] = {1,1,1,1,0};
    int quantidade_alunos = 5;
    Aluno alunos[5];
    for(int i = 0; i < quantidade_alunos; i++){
        Aluno current = {i, nomes[i], medias[i], status[i]};
        alunos[i] = current;
    }

    criarListaDeAlunos(alunos, 5);

    // Inicio do sistema
    FILE *f = fopen(ARQUIVO, "rb+");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
    callSystem(f, quantidade_alunos);


    return 0;
}