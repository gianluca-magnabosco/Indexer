#ifndef SEARCH_H
#define SEARCH_H

#define TAM_MIN_PALAVRA 2
#define TAM_MAX_PALAVRA 128

typedef struct PalavraArquivoFreq {
    char *nomeArquivo;
    double termFrequency;
    double TF_IDF;
    int countPalavra;
    int countTotal;
} PalavraArquivoFreq;

typedef struct PalavraNode {
    char *palavra;
    double inverseDocumentFrequency;
    PalavraArquivoFreq *arquivosFreq;
} PalavraNode;


void handleSearch(char *termo, char **arquivos, int qtd_arquivos);
PalavraNode* initializePalavras(char *termo, int numeroPalavras, char **arquivos, int qtd_arquivos);
int countPalavras(char *termo);
void calculateTF(PalavraNode *palavras, int numeroPalavras, char *nomeArquivo, int indiceArquivo);
double calculateIDF(PalavraNode palavra, char **arquivos, int qtd_arquivos);
void calculateTFIDF(PalavraNode *palavra, int numeroPalavras, int indiceArquivo);

#endif
