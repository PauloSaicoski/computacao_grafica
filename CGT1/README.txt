Requisitos implementados:
- Leitura das amostras do arquivo de entrada no formato especificado.
- Aplicação da DCT sobre as amostras de entrada.
- Aplicação da transformada inversa (IDCT) para reconstrução dos dados originais.
- Calcular a diferença entre a entrada e os dados reconstruídos. (grafico das diferenças
ficou comprometido por conta dos valores muito baixos)
- Plotagem dos gráficos das quatro diferentes amostragens. O tamanho do gráfico
deve se adaptar horizontalmente à quantidade de amostras lidas.
- Salvar o resultado reconstruído em um arquivo “output.dct”, no mesmo formato do
arquivo original.

Extras implementados:
- Exibir as N funções bases usadas na DCT. Neste extra, o botão "base" deve ser marcado 
e utiliza-se as setas para selecionar a base a ser exibida
- Implementação de uma interface interativa. Botões na tela ditam os graficos a serem 
exibidos, os graficos são redimencionados de acordo com a quantia de botões marcados,
tentando sempre alcançar um aspecto de grade
- Todos os elementos da tela se ajustam ao tamanho da janela, que pode ser alterada em 
tempo real, com exceção dos textos, por falta de acesso ao tamanho da fonte
- O programa funciona com mais de 1 input, apenas 2 inputs por vez foram testados, as 
declarações de variáveis e demais requisitos para o segundo input estão prontas porém
comentadas na main, apenas apagar o asterisco da linha 287 deve ser o suficiente para
que funcionem

Obs:
O zip enviado contém 2 inputs de exemplo,que se encontram dentro da pasta pssarkis, porém
para que o projeto os leia corretamente é necessário que estes estejam na pasta anterior 
ao projeto em sí, o mesmo vale para as libs do canvas