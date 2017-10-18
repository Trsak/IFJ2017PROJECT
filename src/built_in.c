/**
 * @file built_in.c
 * @author Roman Bartl (xbartl06)
 * @author Petr Sopf (xsopfp00)
 * @brief Built in functions of IFJ17 language
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "built_in.h"

/**
 * @copydoc print
 */
void print(char **stringsToPrint) {
    (void) stringsToPrint;
}

/**
 * @copydoc input
 */
char *input() {
    return "Hello world";
}

/**
 * @copydoc stringLength
 */
int stringLength(char *string) {
    (void) string;
    return 0;
}

/**
 * @copydoc subStr
 */
char *subStr(char *string, int startChar, int stringLength) {
    if (string == NULL || string[0] == '\0' || startChar <= 0) {
        char *out = (char *) malloc(sizeof(char));
        if (out == NULL) {
            return NULL;
        }

        out[0] = '\0';
        return out;
    }

    int len = strlen((const char *) string);

    if (stringLength < 0 || stringLength > (len - startChar)) {
        stringLength = len - startChar + 1;
    }

    char *out = (char *) malloc((stringLength + 1) * sizeof(char));
    if (out == NULL) {
        return NULL;
    }

    for (int i = 0; i < stringLength; i++) {
        out[i] = string[startChar + i - 1];
    }

    out[stringLength] = '\0';
    return out;
}

/**
 * @copydoc asc
 */
int asc(char *string, int charPosition) {
    int len = strlen((const char *) string);

    if (charPosition < 0 || charPosition > (len - 1)) {
        return 0;
    }

    return (int) string[charPosition];
}

/**
 * @copydoc chr
 */
char *chr(int asciiVal) {
    char *out = (char *) malloc(2 * sizeof(char));
    if (out == NULL) {
        return NULL;
    }

    out[0] = asciiVal;
    out[1] = '\0';

    return out;
}
