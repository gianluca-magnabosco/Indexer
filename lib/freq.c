#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freq.h"


void handleFreq(char *nomeArquivo, int n) {
    MinHeap* minHeap = createMinHeap(n);

    TrieNode* root = NULL;
  
    FILE *arquivo;
    int letra, i;
    char palavra[TAM_MAX_PALAVRA];

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
            
            palavra[i] = (char) letra;

            letra = getc(arquivo);
            i++;
        }

        if (i > 0) {
            palavra[i] = '\0';

            if (i < TAM_MIN_PALAVRA) {
                continue;
            }

            insertTrieAndHeap(&root, minHeap, palavra);
        }
    }
    fclose(arquivo);

    displayNMostFrequent(minHeap);

    freeMinHeap(minHeap);
    freeTrieNodes(root);
}

void displayNMostFrequent(MinHeap* minHeap) {
    quicksortFrequencies(minHeap->array, 0, minHeap->count - 1);

    for (int i = 0; i < minHeap->count; i++) {
        printf("%d - Palavra: %s - Count: %d\n", (i + 1), minHeap->array[i].palavra, minHeap->array[i].frequencia);
    }
}

TrieNode* createTrieNode() {
    TrieNode* trieNode = (TrieNode*) malloc(sizeof(TrieNode));
    if (trieNode == NULL) {
        printf("\nErro ao alocar memoria!\n");
        exit(1);
    }

    trieNode->isEnd = 0;
    trieNode->frequencia = 0;
    trieNode->minHeapIndex = -1;
    for (int i = 0; i < QTD_CHARS_VALIDOS; i++) {
        trieNode->child[i] = NULL;
    }

    return trieNode;
}

MinHeap* createMinHeap(int tamanho) {
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    if (minHeap == NULL) {
        printf("\nErro ao alocar memoria!\n");
        exit(1);
    }
    minHeap->tamanho = tamanho;
    minHeap->count = 0;

    minHeap->array = (MinHeapNode*) malloc(sizeof(MinHeapNode) * tamanho);
    if (minHeap->array == NULL) {
        printf("\nErro ao alocar memoria!\n");
        exit(1);
    }

    return minHeap;
}

void insertTrieAndHeap(TrieNode** root, MinHeap* minHeap, char* palavra) {
    insertUtil(root, minHeap, palavra, palavra);
}

void insertUtil(TrieNode** root, MinHeap* minHeap, char* palavra, char* palavraCopy) {
    if (*root == NULL) {
        *root = createTrieNode();
    }

    if (*palavra != '\0') {
        insertUtil(&((*root)->child[*palavra - 97]), minHeap, palavra + 1, palavraCopy);
    } else {
        if ((*root)->isEnd) {
            ((*root)->frequencia)++;
        } else {
            (*root)->isEnd = 1;
            (*root)->frequencia = 1;
        }

        insertMinHeap(minHeap, root, palavraCopy);
    }
}

void insertMinHeap(MinHeap* minHeap, TrieNode** root, char* palavra) {
    if ((*root)->minHeapIndex != -1) {
        (minHeap->array[(*root)->minHeapIndex].frequencia)++;

        minHeapify(minHeap, (*root)->minHeapIndex);
    }

    else if (minHeap->count < minHeap->tamanho) {
        int count = minHeap->count;
        minHeap->array[count].frequencia = (*root)->frequencia;
        minHeap->array[count].palavra = (char*) malloc(sizeof(char) * (strlen(palavra) + 1));
        if (minHeap->array[count].palavra == NULL) {
            printf("\nErro ao alocar memoria!\n");
            exit(1);
        }
        strcpy(minHeap->array[count].palavra, palavra);

        minHeap->array[count].root = *root;
        (*root)->minHeapIndex = minHeap->count;

        (minHeap->count)++;
        buildMinHeap(minHeap);
    }

    else if ((*root)->frequencia > minHeap->array[0].frequencia) {
        minHeap->array[0].root->minHeapIndex = -1;
        minHeap->array[0].root = *root;
        minHeap->array[0].root->minHeapIndex = 0;
        minHeap->array[0].frequencia = (*root)->frequencia;

        free(minHeap->array[0].palavra);
        minHeap->array[0].palavra = (char*) malloc(sizeof(char) * (strlen(palavra) + 1));
        if (minHeap->array[0].palavra == NULL) {
            printf("\nErro ao alocar memoria!\n");
            exit(1);
        }
        strcpy(minHeap->array[0].palavra, palavra);

        minHeapify(minHeap, 0);
    }
}

void buildMinHeap(MinHeap* minHeap) {
    int n, i;
    n = minHeap->count - 1;

    for (i = ((n - 1) / 2); i >= 0; --i) {
        minHeapify(minHeap, i);
    }
}

void minHeapify(MinHeap* minHeap, int indice) {
    int esq, dir, menor;

    esq = 2 * indice + 1;
    dir = 2 * indice + 2;
    menor = indice;

    if (esq < minHeap->count && minHeap->array[esq].frequencia < minHeap->array[menor].frequencia) {
        menor = esq;
    }

    if (dir < minHeap->count && minHeap->array[dir].frequencia < minHeap->array[menor].frequencia) {
        menor = dir;
    }

    if (menor != indice) {
        minHeap->array[menor].root->minHeapIndex = indice;
        minHeap->array[indice].root->minHeapIndex = menor;

        swapMinHeapNodes(&minHeap->array[menor], &minHeap->array[indice]);

        minHeapify(minHeap, menor);
    }
}

void swapMinHeapNodes(MinHeapNode* a, MinHeapNode* b) {
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void freeTrieNodes(TrieNode* root) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < QTD_CHARS_VALIDOS; i++) {
        freeTrieNodes(root->child[i]);
    }

    free(root);
}

void freeMinHeap(MinHeap* minHeap) {
    if (minHeap == NULL) {
        return;
    }

    for (int i = 0; i < minHeap->count; i++) {
        free(minHeap->array[i].palavra);
    }

    free(minHeap->array);
    free(minHeap);
}

void quicksortFrequencies(MinHeapNode arr[], int low, int high) {
    if (low < high) {
        int part = partitionFrequencies(arr, low, high);

        quicksortFrequencies(arr, low, part - 1);
        quicksortFrequencies(arr, part + 1, high);
    }
}

int partitionFrequencies(MinHeapNode arr[], int low, int high) {
    int pivo = arr[high].frequencia;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].frequencia > pivo) {
            i++;
            swapMinHeapNodes(&arr[i], &arr[j]);
        }
    }

    swapMinHeapNodes(&arr[i + 1], &arr[high]);
    return (i + 1);
}
