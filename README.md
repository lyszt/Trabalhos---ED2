
# TRABALHO I

Este relatório apresenta o desenvolvimento e a análise de uma solução voltada para a ordenação externa de números inteiros positivos, conforme proposto no Trabalho T1 da disciplina. O objetivo central é aplicar técnicas de manipulação de arquivos e otimização de uso de memória principal, simulando um cenário em que os dados excedem a capacidade de armazenamento interno e precisam ser processados em blocos.

Durante a implementação, foram consideradas boas práticas de engenharia de software, como modularização do código, clareza na nomeação de funções e variáveis, além da inserção de comentários explicativos detalhando cada etapa do processo. O  documento explora a lógica aplicada e discute as decisões de design algorítmico, identifica desafios enfrentados durante o desenvolvimento e realiza uma análise precisa da complexidade computacional de cada componente do código.

## Comanda
``` A ordenação a ser implementada deve ser realizada sobre números inteiros positivos vindos de um arquivo. Os números inteiros serão separados com o caractere `;` (ponto-e-vírgula) não havendo espaço entre os números e ponto-e-vírgula. Os blocos em cada arquivo deverão ser separados por um espaço em branco.
A única entrada de dados por parte do usuário é a quantidade de elementos a serem carregados por vez em memória (M). O arquivo contendo os números inteiros que serão ordenados deve se chamar obrigatoriamente dados.txt. e o arquivo resultante contendo os números já ordenados deve ter seu nome impresso em tela. ```