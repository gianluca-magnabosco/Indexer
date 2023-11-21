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

typedef struct TermoNode {
    char *nomeArquivo;
    double tf_idf;
} TermoNode;


void handleSearch(char *termo, char **arquivos, int qtd_arquivos);
PalavraNode* initializePalavras(char *termo, int numeroPalavras, char **arquivos, int qtd_arquivos);
TermoNode* initializeTermoNodes(PalavraNode* palavras, int numeroPalavras, char** arquivos, int qtd_arquivos);
int countPalavras(char *termo);
void calculateTF(PalavraNode *palavras, int numeroPalavras, char *nomeArquivo, int indiceArquivo);
double calculateIDF(PalavraNode palavra, char **arquivos, int qtd_arquivos);
void calculateTFIDF(PalavraNode *palavra, int numeroPalavras, int indiceArquivo);
void swap(TermoNode *a, TermoNode *b);
int partition(TermoNode *arr, int low, int high);
void quicksort(TermoNode *arr, int low, int high);
void printResults(PalavraNode* palavras, int numeroPalavras, char** arquivos, int qtd_arquivos, char* termoOriginal, TermoNode* termoNodes);
void freeEverything(PalavraNode* palavras, int numeroPalavras, TermoNode* termoNodes, int qtd_arquivos, char* termoOriginal);

#endif
