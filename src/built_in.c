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
    return 0;
}

/**
 * @copydoc subStr
 */
char *subStr(char *string, int startChar, int stringLength) {
    if (string == NULL || string == "" || startChar <= 0) {
        return "";
    }

    int len = strlen(string);

    if (stringLength < 0 || stringLength > (len - startChar)) {
        char c = string[startChar - 1];
        stringLength = 0;
        for (int i = startChar; c != '\0'; i++) {
            ++stringLength;
        }
    }

    char *out = (char *) malloc((stringLength + 1) * sizeof(char));

    for (unsigned int i = 0; i < stringLength; i++) {
        out[i] = string[startChar + i];
    }

    out[stringLength] = '\0';
    return out;
}

/**
 * @copydoc asc
 */
int asc(char *string, int charPosition) {
    int len = strlen(string);

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
