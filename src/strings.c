/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @brief
 */
#include "strings.h"
#include <malloc.h>
#include "strings.h"

int strInit(string *s) {
    if ((s->str = (char *) malloc(STR_LEN_INC)) == NULL) {
        return STR_ERROR;
    }
    s->str[0] = '\0';
    s->length = 0;
    s->allocSize = STR_LEN_INC;
    return STR_SUCCESS;
}

void strFree(string *s) {
    free(s->str);
}

void strClear(string *s) {
    strFree(s);
    strInit(s);
}

int strAddChar(string *s1, char c) {
    if (s1->length + 1 >= s1->allocSize) {
        if ((s1->str = (char *) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL) {
            return STR_ERROR;
        }
        s1->allocSize = s1->length + STR_LEN_INC;
    }
    s1->str[s1->length] = c;
    s1->length++;
    s1->str[s1->length] = '\0';
    return STR_SUCCESS;
}