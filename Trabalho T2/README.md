

Trabalho T2
Este trabalho tem como objetivo a prática de operações em tabelas hash e a leitura em arquivos texto.
I Instruções
A montagem da tabela hash deve ser realizada sobre números inteiros positivos vindos de um arquivo. Os números inteiros serão separados com o caractere `;` (ponto-e-vírgula) não havendo espaço entre os números e ponto-e-vírgula.
Não haverá entrada de dados por parte do usuário e todas as informações serão carregadas a partir de um arquivo. O arquivo contendo os números inteiros e demais informações para a montagem da tabela hash deve se chamar obrigatoriamente dados.txt e a tabela hash resultante deverá ser impressa em tela.
O arquivo dados.txt terá a seguinte estrutura:
1a linha: valor de M (tamanho da tabela hash)
2a linha: tipo de endereçamento (tratamento de colisões). O 0 indica endereçamento fechado com inserção da nova chave ao final da lista encadeada; e 1 indica endereçamento aberto com sondagem linear.
3a linha: inteiros a serem armazenados na tabela hash.

A saída deverá ser produzida em tela de acordo com as instruções contidas no arquivo. Exemplos:


11
0
7;17;36;100;106;205
0:
1: 100
2:
3: 36
4:
5:
6: 17
7: 7 -> 106 -> 205
8:
9:
10:
11
1
7;17;36;100;106;205
0:
1: 100
2:
3: 36
4:
5:
6: 17
7: 7
8: 106
9: 205
10:


II Entrega
O trabalho deve conter um arquivo com o código fonte, comentários explicando a lógica de implementação e uma demonstração de funcionamento do código (função main). Além do código, cada estudante deverá entregar um relatório escrito, em formato PDF, contendo a descrição da lógica utilizada na implementação, comentando sobre as decisões tomadas e as facilidades/dificuldades durante este tempo, bem como uma análise da complexidade de cada trecho de código escrito pelo estudante.
A entrega da atividade será on-line através do SIGAA. O estudante deve enviar um único arquivo ZIP contendo as suas implementações e demais itens solicitados. O arquivo ZIP deve ser nomeado com a matrícula do estudante.
Data limite para entrega: 25/05/2025, 23h59m.



III Avaliação
A avaliação será realizada de acordo com a entrega do estudante observados os seguintes critérios:
código compilando e funcionando sem alterações na lógica;
nível de comentários explicando a lógica utilizada;
presença da demonstração do código na função  main;
utilização dos laços de repetição da forma correta (for, while, do-while, etc).
Nomes das variáveis e funções;
Nível de detalhamento do relatório de implementação;
Correção da análise da complexidade;
Coerência do texto do relatório.
Em caso de plágio (incluindo utilização de ferramentas de geração de código e de texto), a entrega será desconsiderada e os estudantes envolvidos receberão nota 0. Procedimentos administrativos poderão ser iniciados após consulta à coordenação de curso. Os professores do CCR se reservam ao direito de solicitar que estudantes apresentem o código produzido para o trabalho após a entrega. 


