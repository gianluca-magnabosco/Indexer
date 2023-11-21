#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/freq.h"
#include "lib/freq_word.h"
#include "lib/search.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        printf("\nInsira um parametro valido!\nUso: indexer [--freq N ARQUIVO | --freq-palavra PALAVRA ARQUIVO | --search TERMO ARQUIVO [ARQUIVO ...]]\n");
        return 1;
    }

    if (strcmp(argv[1], "--freq") == 0) {
        if (argc != 4) {
            printf("\nUsagem invalida da opcao --freq.\nUsagem: --freq N ARQUIVO\n");
            return 1;
        }

        int n = atoi(argv[2]);
        char *nomeArquivo = argv[3];
        handleFreq(nomeArquivo, n);

        return 0;
    }

      
    if (strcmp(argv[1], "--freq-word") == 0) {
        if (argc != 4) {
            printf("\nUsagem invalida da opcao --freq-word.\nUsagem: --freq-word PALAVRA ARQUIVO\n");
            return 1;
        }

        char *palavra = argv[2];
        char *nomeArquivo = argv[3];

        if (strlen(palavra) < 2) {
            printf("\nA palavra deve ter 2 ou mais caracteres!\n");
            return 1;
        }

        handleFreqWord(palavra, nomeArquivo);

        return 0;
    }


    if (strcmp(argv[1], "--search") == 0) {
        if (argc < 4) {
            printf("\nUsagem invalida da opcao --search.\nUsagem: --search TERMO ARQUIVO [ARQUIVO ...]\n");
            return 1;
        }

        char *termo = argv[2];
        int qtd_arquivos = argc - 3;
        char **arquivos = (char **) malloc(sizeof(char *) * qtd_arquivos);

        if (arquivos == NULL) {
            printf("\nErro ao alocar memoria!\n");
            exit(1);
        }

        for (int i = 3; i < argc; i++) {
            arquivos[i - 3] = strdup(argv[i]);
        }

        handleSearch(termo, arquivos, qtd_arquivos);

        for (int i = 0; i < qtd_arquivos; i++) {
            free(arquivos[i]);
        }

        free(arquivos);

        return 0;
    }


    printf("\nErro: Opcao nao encontrada!\n");
    return 1;
}
