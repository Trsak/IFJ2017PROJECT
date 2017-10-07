#include <stdio.h>
#include "scaner.h"


#define FILE_ERROR 5

int main(int argc, char** argv) {
    FILE *f;
    if (argc == 1)
    {
        printf("Neni zadan vstupni soubor\n");
        return FILE_ERROR;
    }
    if ((f = fopen(argv[1], "r")) == NULL)
    {
        printf("Soubor se nepodarilo otevrit\n");
        return FILE_ERROR;
    }

    setSourceFile(f);

    fclose(f);
    return 0;
}