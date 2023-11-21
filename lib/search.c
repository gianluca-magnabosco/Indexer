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

    TermoNode* termoNodes = initializeTermoNodes(palavras, numeroPalavras, arquivos, qtd_arquivos);
    quicksort(termoNodes, 0, qtd_arquivos - 1);

    printResults(palavras, numeroPalavras, arquivos, qtd_arquivos, termoOriginal, termoNodes);
    
    freeEverything(palavras, numeroPalavras, termoNodes, qtd_arquivos, termoOriginal);
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


TermoNode* initializeTermoNodes(PalavraNode* palavras, int numeroPalavras, char** arquivos, int qtd_arquivos) {
    TermoNode* termoNodes = (TermoNode*) malloc(qtd_arquivos * sizeof(TermoNode));
    if (termoNodes == NULL) {
        printf("\nErro ao alocar memoria!\n");
        exit(1);
    }

    for (int i = 0; i < qtd_arquivos; i++) {
        TermoNode* termoNode = &termoNodes[i];
        termoNode->nomeArquivo = strdup(arquivos[i]);
        termoNode->tf_idf = 0.0;

        for (int j = 0; j < numeroPalavras; j++) {
            termoNode->tf_idf += palavras[j].arquivosFreq[i].TF_IDF;
        }

        termoNode->tf_idf = termoNode->tf_idf / numeroPalavras;
    }

    return termoNodes;
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

void swap(TermoNode *a, TermoNode *b) {
    TermoNode temp = *a;
    *a = *b;
    *b = temp;
}

int partition(TermoNode *arr, int low, int high) {
    double pivo = arr[high].tf_idf;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].tf_idf >= pivo) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(TermoNode *arr, int low, int high) {
    if (low < high) {
        int part = partition(arr, low, high);

        quicksort(arr, low, part - 1);
        quicksort(arr, part + 1, high);
    }
}

void printResults(PalavraNode* palavras, int numeroPalavras, char** arquivos, int qtd_arquivos, char* termoOriginal, TermoNode* termoNodes) {
    for (int i = 0; i < numeroPalavras; i++) {
        printf("\nPalavra: \"%s\"\n", palavras[i].palavra);
        
        for (int j = 0; j < qtd_arquivos; j++) {
            printf("Arquivo %s - Count Palavra %d - Count Total %d - TF %f - IDF %f - TF-IDF %f\n", palavras[i].arquivosFreq[j].nomeArquivo, palavras[i].arquivosFreq[j].countPalavra, palavras[i].arquivosFreq[j].countTotal, palavras[i].arquivosFreq[j].termFrequency, palavras[i].inverseDocumentFrequency, palavras[i].arquivosFreq[j].TF_IDF);
        }
    }

    printf("\n-------------------------------------------------------------------------------\n");

    printf("Termo: \"%s\"\n", termoOriginal);
    for (int i = 0; i < qtd_arquivos; i++) {
        printf("Arquivo: %s - TF-IDF: %f\n", termoNodes[i].nomeArquivo, termoNodes[i].tf_idf);
    }

    printf("-------------------------------------------------------------------------------\n");
}

void freeEverything(PalavraNode* palavras, int numeroPalavras, TermoNode* termoNodes, int qtd_arquivos, char* termoOriginal) {
    for (int i = 0; i < numeroPalavras; i++) {
        free(palavras[i].palavra);
        free(palavras[i].arquivosFreq);
    }

    for (int i = 0; i < qtd_arquivos; i++) {
        free(termoNodes[i].nomeArquivo);
    }

    free(termoNodes);
    free(palavras);
    free(termoOriginal);
}
