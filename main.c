/**
 * @project IFJ17
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
#include "garbage_collector.h"
#include "generator.h"

int main(int argc, char **argv) {
    if (argc != 1) {
        printErrAndExit(ERROR_INTERNAL, "Bad usage! Just call ./ifj2017 without arguments!");
        return ERROR_INTERNAL;
    }

    gcInit();

    (void) argv;

    //initialization of symtable
    btInit(&symtable);

    program();

    startGenerating();

    return 0;
}
