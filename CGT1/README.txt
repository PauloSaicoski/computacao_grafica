Requisitos implementados:
- Leitura das amostras do arquivo de entrada no formato especificado.
- Aplica��o da DCT sobre as amostras de entrada.
- Aplica��o da transformada inversa (IDCT) para reconstru��o dos dados originais.
- Calcular a diferen�a entre a entrada e os dados reconstru�dos. (grafico das diferen�as
ficou comprometido por conta dos valores muito baixos)
- Plotagem dos gr�ficos das quatro diferentes amostragens. O tamanho do gr�fico
deve se adaptar horizontalmente � quantidade de amostras lidas.
- Salvar o resultado reconstru�do em um arquivo �output.dct�, no mesmo formato do
arquivo original.

Extras implementados:
- Exibir as N fun��es bases usadas na DCT. Neste extra, o bot�o "base" deve ser marcado 
e utiliza-se as setas para selecionar a base a ser exibida
- Implementa��o de uma interface interativa. Bot�es na tela ditam os graficos a serem 
exibidos, os graficos s�o redimencionados de acordo com a quantia de bot�es marcados,
tentando sempre alcan�ar um aspecto de grade
- Todos os elementos da tela se ajustam ao tamanho da janela, que pode ser alterada em 
tempo real, com exce��o dos textos, por falta de acesso ao tamanho da fonte
- O programa funciona com mais de 1 input, apenas 2 inputs por vez foram testados, as 
declara��es de vari�veis e demais requisitos para o segundo input est�o prontas por�m
comentadas na main, apenas apagar o asterisco da linha 287 deve ser o suficiente para
que funcionem

Obs:
O zip enviado cont�m 2 inputs de exemplo,que se encontram dentro da pasta pssarkis, por�m
para que o projeto os leia corretamente � necess�rio que estes estejam na pasta anterior 
ao projeto em s�, o mesmo vale para as libs do canvas