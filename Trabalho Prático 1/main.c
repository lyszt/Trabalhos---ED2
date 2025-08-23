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
#define ARQUIVO "./alunos.txt"
#define MAXIMO_LINHA 2048;

typedef struct {
    int id; 
    char* nome;
    float media; 
    int ativo;
} Aluno; 

void criarListaDeAlunos(Aluno* lista_alunos, int quantidade_alunos) {
    FILE *f = fopen(ARQUIVO, "w+");

    for(int i = 0; i < quantidade_alunos; i++){
        fprintf(f, " %d", lista_alunos[i].id);
        fprintf(f, " %s", lista_alunos[i].nome);
        fprintf(f, " %.1f", lista_alunos[i].media);
        fprintf(f, " %d", lista_alunos[i].ativo);
        fputc('\n', f);
    }
    fclose(f);    

}

Aluno getAluno(FILE *f, int id){
    fseek(f, 0, SEEK_SET);
    // id, nome, media e status
    // Pula pro id
    char max_line[2048];
    for(int i = 0; i < id; i++){
        fgets(max_line, sizeof(max_line), f);
    }

    char id_ignore[100];
    char nome[100];
    char media[100];
    char status[100];
    fscanf(f, "%99s %99s %99s %99s", id_ignore, nome, media, status);


    char* endptr;
    Aluno aluno_lido = {(int) id, nome, strtof(media, &endptr), atoi(status)};
    return aluno_lido;
}

void listActive(FILE *f, int total_alunos){
    for(int i = 0; i < total_alunos; i++){
        Aluno aluno = getAluno(f, i);
        char* status;
        status = (aluno.ativo == 1)? "Ativo" : "Apagado";
        if(aluno.ativo == 1){
            printf("Aluno %d - %s - Média: %f - STATUS: %s\n", aluno.id, aluno.nome, aluno.media, status);
        }
    }
}

void listInactive(FILE *f, int total_alunos){
    for(int i = 0; i < total_alunos; i++){
        Aluno aluno = getAluno(f, i);
        char* status;
        status = (aluno.ativo == 1)? "Ativo" : "Apagado";
        if(aluno.ativo == 0){
            printf("Aluno %d - %s - Média: %f - STATUS: %s\n", aluno.id, aluno.nome, aluno.media, status);
        }
    }
}

void callSystem(FILE *f, int quantidade_alunos){
    printf("Sistema de registro de Alunos em Arquivo Binário\n");
    printf("1. Listar alunos ativos\n");
    printf("2. Listar alunos inativos\n");
    printf("3. Atualizar media\n");
    printf("4. Apagar aluno\n\n");
    int choice;
    scanf("%d", &choice);
    switch(choice){
        case 1:
            listActive(f, quantidade_alunos);
            break;
        case 2:
            listInactive(f, quantidade_alunos);
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
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
    FILE *f = fopen(ARQUIVO, "r+");
    callSystem(f, quantidade_alunos);


    return 0;
}