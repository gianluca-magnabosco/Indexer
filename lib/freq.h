#ifndef FREQ_H
#define FREQ_H

#define QTD_CHARS_VALIDOS 26
#define TAM_MIN_PALAVRA 2
#define TAM_MAX_PALAVRA 128

typedef struct TrieNode {
    int isEnd;
    unsigned frequencia;
    int minHeapIndex;
    struct TrieNode* child[QTD_CHARS_VALIDOS];
} TrieNode;

typedef struct MinHeapNode {
    struct TrieNode* root;
    unsigned frequencia;
    char* palavra;
} MinHeapNode;

typedef struct MinHeap {
    unsigned tamanho;
    int count;
    struct MinHeapNode* array;
} MinHeap;

void handleFreq(char *nomeArquivo, int n);
void displayNMostFrequent(MinHeap* minHeap);

TrieNode* createTrieNode();
MinHeap* createMinHeap(int tamanho);
void insertTrieAndHeap(TrieNode** root, MinHeap* minHeap, char* palavra);
void insertUtil(TrieNode** root, MinHeap* minHeap, char* palavra, char* palavraCopy);
void insertMinHeap(MinHeap* minHeap, TrieNode** root, char* palavra);
void buildMinHeap(MinHeap* minHeap);
void minHeapify(MinHeap* minHeap, int indice);
void swapMinHeapNodes(MinHeapNode* a, MinHeapNode* b);
void freeTrieNodes(TrieNode* root);
void freeMinHeap(MinHeap* minHeap);
void quicksortFrequencies(MinHeapNode arr[], int low, int high);
int partitionFrequencies(MinHeapNode arr[], int low, int high);

#endif
