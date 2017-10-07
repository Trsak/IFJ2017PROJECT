//
// Created by Bartosek on 06.10.2017.
//

#ifndef UNTITLED_STRING_H
#define UNTITLED_STRING_H

#endif //UNTITLED_STRING_H

typedef struct
{
    char* str;		// Řetězec ukončení znakem /0
    int length;		// Délka řetězce
    int allocSize;	// Velikost alokované paměti
} string;

#define STR_LEN_INC 8 // Počáteční délka inicializace v bytech

#define STR_ERROR   1
#define STR_SUCCESS 0

int strInit(string *s);
void strFree(string *s);
void strClear(string *s);
int strAddChar(string *s1, char c);