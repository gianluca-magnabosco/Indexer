#include <stdio.h>
#include <string.h>
#include "freq_word.h"


void handleFreqWord(char* palavra, char* nomeArquivo) {
    
    FILE *arquivo;
    int letra, i, count = 0;
    char buffer[TAM_MAX_PALAVRA];

    if ((arquivo = fopen(nomeArquivo, "r")) == NULL) {
        printf("\nErro ao abrir arquivo: %s!\n", nomeArquivo);
        fclose(arquivo);
        return;
    }

    int newWord = 0, skipped = 0;

    while (!feof(arquivo)) {

        letra = getc(arquivo);

        if ((letra >= 65 && letra <= 90) || (letra >= 97 && letra <= 122)) {
            if (skipped) {
                newWord = 0;
            } else {
                newWord = 1;
            }
        } else {
            skipped = 0;
            newWord = 1;
        }

        if (!newWord) {
            continue;
        }

        i = 0;
        while ((newWord && !skipped) && ((letra >= 65 && letra <= 90) || (letra >= 97 && letra <= 122))) {
            if (letra >= 65 && letra <= 90) {
                letra += 32;
            }

            buffer[i] = (char) letra;

            if (buffer[i] != palavra[i]) {
                newWord = 0;
                skipped = 1;
                break;
            }

            letra = getc(arquivo);
            i++;
        }

        if (i > 0 && !skipped) {
            buffer[i] = '\0';
            if (strcmp(buffer, palavra) == 0) {
                count++;
            }
        }
    }
    fclose(arquivo);

    printf("\nPalavra: %s - Count: %d\n", palavra, count);
}
