#include <stdio.h>
#include "search.h"


void handleSearch(char *termo, char **arquivos, int qtd_arquivos) {
    // talvez ver quantidade de palavras no termo..

    for (int i = 0; i < qtd_arquivos; i++) {
        printf("%s", arquivos[i]);
    }
}
