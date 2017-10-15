/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @author Petr Sopf (xsopfp00)
 * @brief Main project file
 */
#include <stdio.h>
#include "scanner.h"
#include "error_codes.h"

int main(int argc, char **argv) {
    FILE *f;
    if (argc == 1) {
        printf("No input file\n");
        return ERROR_FILE;
    }
    if ((f = fopen(argv[1], "r")) == NULL) {
        printf("File could not be opened\n");
        return ERROR_FILE;
    }

    setSourceFile(f);

    fclose(f);
    return 0;
}
