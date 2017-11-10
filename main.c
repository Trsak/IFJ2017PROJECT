/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @author Petr Sopf (xsopfp00)
 * @author Roman Bartl (xbartl06)
 * @brief Main project file
 */

#include <stdio.h>
#include "scanner.h"
#include "error_codes.h"
#include "parser.h"

int main(int argc, char **argv) {
    FILE *f;

    if (argc != 2) {
        printErrMsg(ERROR_INTERNAL, "Bad arguments! Use /ifj2017 <file>!");
        return ERROR_INTERNAL;
    }

    if ((f = fopen(argv[1], "r")) == NULL) {
        printErrMsg(ERROR_INTERNAL, "File '%s' could not be opened.", argv[1]);
        return ERROR_INTERNAL;
    }

    setSourceFile(f);


    int returnValue = parse();


    //only for debug
    //TODO - delete after
    for(int i = 0; i < 200; i++) {
        printf("%d, ", tree[i]);
    }
    printf("\n");


    fclose(f);

    return returnValue;
}
