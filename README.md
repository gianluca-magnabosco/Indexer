# Indexer

## Para gerar o executável

* Instale o GCC;
* Utilize o seguinte comando:

```
    gcc ./lib/freq.c ./lib/freq_word.c ./lib/search.c ./indexer.c -o indexer
```

* O comando acima irá gerar um executável chamado indexer.

## Para utilizar a aplicação

* Selecione uma das opções abaixo e rode o indexer passando os parâmetros adequados.
* Exemplo:
```
    ./indexer --freq 300 1gb.txt
    ./indexer --freq-word the 1gb.txt
    ./indexer --search "the end" 1gb.txt gigateste.txt testezinho.txt
```

## Opções disponíveis na aplicação

### --freq N ARQUIVO
* Exibe o número de ocorrência das N palavras que mais aparecem em ARQUIVO, em ordem decrescente de ocorrência.

### --freq-word PALAVRA ARQUIVO
* Exibe o número de ocorrências de PALAVRA em ARQUIVO. 

### --search TERMO ARQUIVO [ARQUIVO ...]
* Exibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por TERMO. A listagem é apresentada em ordem descrescente de relevância. TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre aspas.

