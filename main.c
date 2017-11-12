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
    if (argc != 1) {
        printErrMsg(ERROR_INTERNAL, "Bad usage! Just call ./ifj2017 without arguments!");
        return ERROR_INTERNAL;
    }

    strInit(&attr);

    int returnValue = parse();

    //only for debug
    //TODO - delete after
    for (int i = 0; i < 200; i++) {
        printf("%d, ", tree[i]);
    }
    printf("\n");

    return returnValue;
}
