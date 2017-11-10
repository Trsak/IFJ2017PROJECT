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
        printErrMsg(ERROR_FILE, "Bad arguments! Use /ifj2017 <file>!");
        return ERROR_FILE;
    }

    if ((f = fopen(argv[1], "r")) == NULL) {
        printErrMsg(ERROR_FILE, "File '%s' could not be opened.", argv[1]);
        return ERROR_FILE;
    }

    setSourceFile(f);

    if(!program()) {
        //return 1;
        //TODO - uncomment
    }

    //only for debug
    //TODO - delete after
    int i;

    for(i = 0; i < 200; i++) {
        printf("%d, ", tree[i]);
    }

    if(i == 200) printf("Uspesny preklad");

    printf("\n");



    fclose(f);
    return 0;
}
