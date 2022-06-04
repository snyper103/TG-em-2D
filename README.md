# Transformações geométricas em 2D
O OpenGL consiste em um conjunto de rotinas gráficas gratuitas. Pensando nisso foi feito um algoritimo em C/C++ de transformações geométricas em 2D utilizando openGL.

## Requsitos
É necessário ter o openGL instalado corretamente em seu computador, além disso é preciso ter o compilador de C/C++ intalado também.

### Execução
Basta compilar os arquivos contidos neste repositório e executá-los.

### Funcionalidades
1) Permitir ao usuário escolher as seguintes figuras geométricas planas regulares a serem desenhadas: Triângulo, Quadrado, Hexágono.
2) Permitir ao usuário digitar as coordenadas dos vértices de cada figura. Neste caso, os polígonos a serem desenhados poderão ser irregulares.
3) Permitir ao usuário utilizar cliques de mouse para determinar os vértices e/ou o centro de cada figura, no caso de polígonos irregulares.
4) Permitir ao usuário digitar o ponto central e o tamanho da aresta (borda) de cada polígono. Neste caso os polígonos desenhados serão regulares. 
5) Desenhar a figura na tela conforme parâmetros de entrada fornecidos pelo usuário (requisitos 1 a 4). Suportar o desenho e visualização de no máximo 5 figuras simultaneamente.
6) Permitir escolher qualquer uma das seguintes TGs, a serem realizadas sobre uma figura já desenhada: 

    - Translação, Escala, Rotação, Reflexão, Cisalhamento
7) Conforme a TG escolhida, solicita ao usuário os parâmetros correspondentes e necessários para realizar a transformação. O usuário pode escolher os parâmetros livremente por digitação. 
As seguintes funcionalidades estão implementadas para cada TG:
    - Translação: permite a translação positiva ou negativa em qualquer direção a  escolha do usuário (eixo x, o eixo y).
    - Escala: permite aumentar e permitir reduzir o objeto.
    - Reflexão: permite fazer a reflexão em relação ao eixo x e em relação ao eixo y.
    - Rotação: permite escolher o ângulo de rotação, que deve ser indicado em graus, podendo ser positivo ou negativo. A figura é rotacionada na mesma posição em que se encontra.
    - Cisalhamento: permite escolher uma proporção da figura para gerar o cisalhamento (ex: 10%, 20%, etc). 
8) Somente a transformação solicitada pelo usuário é aplicada a cada vez e mostrada ao usuário. 
9) Após confirmação do usuário o programa aplica a TG escolhida conforme os parâmetros informados (5, 6 e 7), e mostra o resultado na tela.
10) Possui um sistema de referência na tela de visualização, com escala indicativa das distâncias, com as seguintes características:
    - Ponto central de coordenadas no centro da tela de visualização.
    - Eixo de coordenadas x (linha contínua)
    - Eixo de coordenadas y (linha contínua)
    - Ambos eixos x e y tem 10 segmentos positivos e 10 segmentos negativos, possuem pequenas marcações para sinalizar cada divisão.
12) Permite ao usuário alternar livremente entre telas de desenho e de entrada de parâmetros.
13) Permite apagar os polígonos desenhados e redesenhar outros polígonos ou usar outras opções (aplicar mais de uma transformação por exemplo) sem sair do programa.
14) Valida as coordenadas de entrada conforme segue:
    - As coordenadas devem estar dentro dos limites do sistema de coordenadas do universo (da tela);
    - Não permite a entrada de pontos repetidos (iguais).
    - Permite o desenho de polígonos convexos ou côncavos para figuras irregulares.
