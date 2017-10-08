/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @brief
 */
#ifndef UNTITLED_STRING_H
#define UNTITLED_STRING_H

typedef struct {
    char *str;        // string that ends with /0 char
    int length;        // string lenght
    int allocSize;    // size of allocated memmory
} string;

#define STR_LEN_INC 8 // primary size of allocation in bytes

#define STR_ERROR   1
#define STR_SUCCESS 0

int strInit(string *s);

void strFree(string *s);

void strClear(string *s);

int strAddChar(string *s1, char c);

#endif //UNTITLED_STRING_H