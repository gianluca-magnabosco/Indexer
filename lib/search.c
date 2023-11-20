#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "search.h"

void handleSearch(char *termo, char **arquivos, int qtd_arquivos) {

    char *termoOriginal = strdup(termo);

    int numeroPalavras = countPalavras(termo);

    PalavraNode *palavras = initializePalavras(termo, numeroPalavras, arquivos, qtd_arquivos);

    for (int i = 0; i < qtd_arquivos; i++) {
        calculateTF(palavras, numeroPalavras, arquivos[i], i);
    }

    for (int i = 0; i < numeroPalavras; i++) {
        palavras[i].inverseDocumentFrequency = calculateIDF(palavras[i], arquivos, qtd_arquivos);
    }

    for (int i = 0; i < qtd_arquivos; i++) {
        calculateTFIDF(palavras, numeroPalavras, i);
    }


    for (int i = 0; i < qtd_arquivos; i++) {
        double TD_IDF_TERMO = 0.0;

        for (int j = 0; j < numeroPalavras; j++) {
            TD_IDF_TERMO += palavras[j].arquivosFreq[i].TF_IDF;
        }

        TD_IDF_TERMO = TD_IDF_TERMO / numeroPalavras;

        printf("Arquivo: %s Termo: \"%s\" TF-IDF: %f\n", arquivos[i], termoOriginal, TD_IDF_TERMO);
    }



    for (int i = 0; i < numeroPalavras; i++) {
        for (int j = 0; j < qtd_arquivos; j++) {
            printf("Palavra %s: Arquivo %s - Count Palavra %d - Count Total %d - TF %f - IDF %f - TF-IDF %f\n", palavras[i].palavra, palavras[i].arquivosFreq[j].nomeArquivo, palavras[i].arquivosFreq[j].countPalavra, palavras[i].arquivosFreq[j].countTotal, palavras[i].arquivosFreq[j].termFrequency, palavras[i].inverseDocumentFrequency, palavras[i].arquivosFreq[j].TF_IDF);
        }
    }

    for (int i = 0; i < numeroPalavras; i++) {
        free(palavras[i].palavra);
        free(palavras[i].arquivosFreq);
    }
    
    free(palavras);
    free(termoOriginal);
}

int countPalavras(char *termo) {
    int j = 1;

    for (int i = 0; termo[i] != '\0'; i++) {
        if (termo[i] == ' ') {
            j++;
        }
    }

    return j;
}

PalavraNode* initializePalavras(char *termo, int numeroPalavras, char **arquivos, int qtd_arquivos) {
    PalavraNode *arrayPalavras = (PalavraNode*) malloc(sizeof(PalavraNode) * numeroPalavras);

    if (arrayPalavras == NULL) {
        printf("\nErro ao alocar memoria!\n");
        exit(1);
    }

    char *token = strtok(termo, " ");

    for (int i = 0; i < numeroPalavras; i++) {
        arrayPalavras[i].palavra = strdup(token);
        arrayPalavras[i].inverseDocumentFrequency = 0.0;

        arrayPalavras[i].arquivosFreq = (PalavraArquivoFreq*) malloc(sizeof(PalavraArquivoFreq) * qtd_arquivos);

        if (arrayPalavras[i].arquivosFreq == NULL) {
            printf("\nErro ao alocar memoria!\n");
            exit(1);
        }

        for (int j = 0; j < qtd_arquivos; j++) {
            arrayPalavras[i].arquivosFreq[j].nomeArquivo = arquivos[j];
            arrayPalavras[i].arquivosFreq[j].termFrequency = 0.0;
            arrayPalavras[i].arquivosFreq[j].TF_IDF = 0.0;
            arrayPalavras[i].arquivosFreq[j].countPalavra = 0;
            arrayPalavras[i].arquivosFreq[j].countTotal = 0;
        }

        token = strtok(NULL, " ");
    }

    return arrayPalavras;
}


void calculateTF(PalavraNode *palavras, int numeroPalavras, char *nomeArquivo, int indiceArquivo) {

    FILE *arquivo;
    int letra, i, count = 0;
    char buffer[TAM_MAX_PALAVRA];

    if ((arquivo = fopen(nomeArquivo, "r")) == NULL) {
        printf("\nErro ao abrir arquivo: %s!\n", nomeArquivo);
        fclose(arquivo);
        exit(1);
    }

    while (!feof(arquivo)) {

        letra = getc(arquivo);

        i = 0;
        while ((letra >= 65 && letra <= 90) || (letra >= 97 && letra <= 122)) {
            if (letra >= 65 && letra <= 90) {
                letra += 32;
            }

            buffer[i] = (char) letra;

            letra = getc(arquivo);
            i++;
        }

        if (i > 0) {
            buffer[i] = '\0';

            if (i < TAM_MIN_PALAVRA) {
                continue;
            }

            for (int j = 0; j < numeroPalavras; j++) {      
                if (strcmp(buffer, palavras[j].palavra) == 0) {
                    palavras[j].arquivosFreq[indiceArquivo].countPalavra++;
                }

                palavras[j].arquivosFreq[indiceArquivo].countTotal++;
            }          
        }
    }
    fclose(arquivo);

    for (int i = 0; i < numeroPalavras; i++) {
        double countPalavra = (double) palavras[i].arquivosFreq[indiceArquivo].countPalavra;
        double countTotal = (double) palavras[i].arquivosFreq[indiceArquivo].countTotal;
        if (countTotal > 0 && countPalavra > 0) {
            palavras[i].arquivosFreq[indiceArquivo].termFrequency = countPalavra / countTotal;
        }
    }
}

double calculateIDF(PalavraNode palavra, char **arquivos, int qtd_arquivos) {
    int presente = 0;

    for (int i = 0; i < qtd_arquivos; i++) {
        if (palavra.arquivosFreq[i].countPalavra > 0) {
            presente++;
        }
    }

    if (presente == 0) {
        return 0.0;
    }

    return log10((double) qtd_arquivos / presente);
}

void calculateTFIDF(PalavraNode *palavra, int numeroPalavras, int indiceArquivo) {
    for (int i = 0; i < numeroPalavras; i++) {
        double tf = palavra[i].arquivosFreq[indiceArquivo].termFrequency;
        double idf = palavra[i].inverseDocumentFrequency;
        palavra[i].arquivosFreq[indiceArquivo].TF_IDF = tf * idf;
    }
}

