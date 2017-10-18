/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @brief
 */
#ifndef SCANNER_H
#define SCANNER_H

// Lexems
#define ID 0
#define NUM 1
#define PLUS 2 // +
#define MINUS 3 // -
#define MUL 4 // *
#define DIV 5 // /
#define INC 6 // ++
#define DEC 7 // --
#define ASSIGNMENT 8 // :=
#define LEQ 9 // <=
#define NEQ 10 // <>
#define LTN 11 // <

// Keywords
/*
 * The value of keywords increments depending on position in array.
 * The value of the keyword "As" is 20, then "Asc" is equal to 21 ...
 * The last value "True" is set to 54.
 * All the values can be found in keyWordsHelper.txt
 */
extern char *keyWords[];

// Special chars
#define END_OF_FILE 55

// Error codes
#define LEX_ERROR -1

// Fce
void setSourceFile(FILE *f);
int getNextToken();

#endif //UNTITLED_SCANER_H