/**
 * @file built_in.c
 * @author Roman Bartl (xbartl06)
 * @author Petr Sopf (xsopfp00)
 * @brief Built in functions of IFJ17 language
 */


#include "<stdio.h>"
#include "<stdlib.h>"
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
    return "Hello world";
}

/**
 * @copydoc asc
 */
int asc(char *string, int charPosition) {
    return 0;
}

/**
 * @copydoc chr
 */
char *chr(int asciiVal) {
    if (asciiVal > 255 || asciiVal < 0) {
        return NULL;
    }

    char *out = (char *) malloc(2 * sizeof(char));
    if (out == NULL) {
        return NULL;
    }

    out[0] = asciiVal;
    out[1] = '\0';

    return out;
}
