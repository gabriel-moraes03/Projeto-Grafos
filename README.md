# Projeto Final Grafos

### Link para relatório:

https://docs.google.com/document/d/1SpKjy77coXA9qU96z0LKDsuP6_mcFtZofpt3sWdrfNQ/edit?usp=sharing


## Teste 1 (teste1.cpp): Algortimo usando KD-Tree modifiado com distância pela fórmula de Haversine

Primeira tentativa de resolução do problema em questão. Nessa etapa, a estrutura de dados base para o projeto era uma KD-Tree, a qual foi julgada como interressante devido seu caráter de armazenamento bi-dimensional de dados. Para a construção do caminho, foi utilizado um algortimo de vizinho mais próximo baseado na KD-Tree, no qual, o vizinho mais próximo ainda não visitado, seria o próximo ponto no caminho. Nessa versão, para o cálculo da distância entre os dois pontos, era utilizado a Fórmula de Haversine, que calcula a distância entre dois pontos geográficos com latitude e longitude. A leitura nesssa versão era feita através de um arquivo, onde seu caminho era determinado na linha 171. A partir do arquivo, era identificado a quantidade de vértices do grafo para finalizar a leitura do arquivo. Como saída, temos a sequência dos pontos, a distância (em relação ao ponto atual e o último ponto), e a maior distância percorrida entre dois vértices. Os resultados eram muito sensíveis em relação ao ponto inicial, diferentes pontos iniciais culminavam em resultados muito diferentes, além de que, ao surgirem problemas, julgamos melhor utilizar alguma outra estrutura de dados, e não a KD-Tree. Assim, seguimos para uma segunda tentativa.

## Teste 2 (teste2.cpp): Algoritmo de inserção mais próxima

Segunda tentativa de resolução do problema. Nessa versão, para o armazenamento dos pontos utilizamos apenas um vetor dinâmico de elementos do registro ponto, onde cada elemento tem um inteiro representando o id do ponto, dois números ponto flutuantes com precisão dupla para representar a latitude e a longitude do ponto, além de um booleano representando se aquele ponto já foi visitado ou não. Para encontrarmos o melhor caminho, utilizamos o algortimo de inserção mais próxima e armazenamos os resultados em um vetor dinâmico de tuplas, onde o primeiro elemento é um registro do tipo ponto, o qual armazena o ponto visitado, e o segundo é um número ponto flutuante de precisão dupla, que armazena a distância do ponto em relação ao ponto anterior. Nesse caso, identificamos os diferentes tipos de coordenadas, desse modo, temos duas funções para a distância entre dois pontos, uma baseada na Fórmula de Haversine para casos de coordenadas geográficas, e uma baseada na distância euclidiana entre dois pontos.

## Teste 3 (teste2matrizDeDistancias.cpp): Algoritmo de inserção mais próxima com matriz de distâncias

Exatamente o mesmo código da segunda tentativa, porém, ao invés de calcular as distâncias a cada iteração da inserção mais próxima, que pode resultar em cálculos desnecessários, o algortimo calcula inicialmente a distância de todos os pontos para todos os pontos, armazenando em uma matriz. Apesar de uma maior necessidade de armazenamento, essa modificação reduziu em 1/3 o tempo computacional.

## Versão Final (otimizacao-do-transporte.cpp)

Mesma versão do Teste 3 com o ajuste da leitura e escrita, que nessa versão são realizadas através de comandos cin e cout.

## Como rodar o projeto?
Basta executar os comandos a seguir:

-   Primeiramente acessar o diretório
-   g++ otimizacao-do-transporte.cpp -o nomeQualquer
-   nomeQualquer < arquivo-de-entrada.ins > arquivo-de-saida.txt
