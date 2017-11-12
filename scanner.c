/**
 * @file scanner.c
 * @author Jan Bartosek (xbarto92)
 * @brief Returns "tokens" that represent each word or char from stdin to the parser or the LEX_ERROR if a lexical error occurred
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "error_codes.h"
#include "symtable.h"

//array of all the keywords for an easy return implementation
char *keyWords[] = {"as", "asc", "declare", "dim", "do", "double", "else", "end", "chr",
                    "function", "if", "input", "integer", "length", "loop", "print",
                    "return", "scope", "string", "substr", "then", "while", "and",
                    "boolean", "continue", "elseif", "exit", "false", "for", "next",
                    "not", "or", "shared", "static", "true"};

//TODO - CALL function strFree(attr) !!

//TODO Every calling of printErrMsg should have as the first argument 1, but it doesn't work for any reason. Temporary replaced by 1.

/**
 * @copydoc getNextToken
 */
lexems getNextToken() {
    int state = 0;
    int c;
    strClear(&attr);

    bool decimal = false;
    bool decimal_e = false;

    while (1) {
        c = getchar();
        switch (state) {
            case 0: // The beginning

                if (c == EOL) {
                    return EOL;
                } else if (c == EOF) {
                    return EOF;
                } else if (c == 39) { // It's an one-line comment
                    state = 1;
                } else if (c == '/') { // It's a multi-line comment or division
                    state = 2;
                } else if (isspace(c)) {  // It's a white space (ignore that!)
                    break;
                } else if (isalpha(c) || c == '_') { // It's an ID or a keyword
                    strAddChar(&attr, tolower(c));
                    state = 4;
                } else if (isdigit(c)) { // It's a number
                    strAddChar(&attr, c);
                    state = 5;
                } else if (c == 33) { // It's a string
                    c = getchar();
                    if (c == 34) { // ASCII 34 == "
                        state = 6;
                    } else if (c == EOL || c == EOF) {
                        printErrMsg(1, "The '!' was given and the following should be '\"' but %c was given", c);
                        return LEX_ERROR;
                    }
                } else if (c == '+') { // It's a plus
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return PLUS;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return PLUS_ASSIGNMENT;
                    }
                    ungetc(c, stdin);
                    return PLUS;
                } else if (c == '-') { // It's minus or decrement
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return MINUS;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return MINUS_ASSIGNMENT;
                    }
                    ungetc(c, stdin);
                    return MINUS;
                } else if (c == '*') {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return MULTIPLY;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return MULTIPLY_ASSIGNMENT;
                    }
                    ungetc(c, stdin);
                    return MULTIPLY;
                } else if (c == 92) {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return BACKSLASH;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return BACKSLASH_ASSIGNMENT;
                    }
                    ungetc(c, stdin);
                    return BACKSLASH;
                } else if (c == '(') {
                    return BRACKET_LEFT;
                } else if (c == ')') {
                    return BRACKET_RIGHT;
                } else if (c == ',') {
                    return COMMA;
                } else if (c == ';') {
                    return SEMICOLON;
                } else if (c == '=') {
                    return ASSIGNMENT;
                } else if (c == '<') {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return LESS;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return LESS_EQUAL;
                    } else if (c == '>') {
                        return NOT_EQUAL;
                    } else {
                        ungetc(c, stdin);
                        return LESS;
                    }
                } else if (c == '>') {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return GREATER;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return GREATER_EQUAL;
                    } else {
                        ungetc(c, stdin);
                        return GREATER;
                    }
                } else {
                    printErrMsg(1, "Unknown character was given: %c", c);
                    return LEX_ERROR;
                }
                break;

            case 1: // One-Line Comment
                if (c == EOL) {
                    state = 0;
                } else if (c == EOF) {
                    return EOF;
                }
                break;

            case 2: // Multi-Line Comment or Division
                if (c == 39) {  // ASCII 39 == '
                    state = 3;
                    break;
                } else {
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            return DIVISION;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        return DIVISION_ASSIGNMENT;
                    }
                    ungetc(c, stdin);
                    return DIVISION;
                }

            case 3: // Multi-Line Comment
                if (c == 39) {
                    c = getchar();
                    if (c == '/') {
                        state = 0;
                    }
                } else if (c == EOF) {
                    printErrMsg(1, "Never-Ending Multi-Line Comment");
                    return LEX_ERROR;
                }
                break;

            case 4: // ID or Keyword TODO the length of ID and save it to the binary tree!!

                if (isalnum(c) || c == '_') {
                    strAddChar(&attr, tolower(c));
                } else {
                    ungetc(c, stdin);
                    for (unsigned int i = 0; i < sizeof(keyWords) / sizeof(char *); i++) {
                        if (strcmp((&attr)->str, keyWords[i]) == 0) {
                            return 30 + i;
                        }
                    }
                    return ID;
                }
                break;


            case 5: // It's a Number
                if (isdigit(c)) {
                    strAddChar(&attr, c);
                } else if (c == '.') {
                    if (decimal || decimal_e) {
                        printErrMsg(1, "Wrong format of a decimal number. Multiple dots were used.");
                        return LEX_ERROR;
                    }
                    strAddChar(&attr, c);
                    c = getchar();
                    if (isdigit(c)) {
                        strAddChar(&attr, c);
                    } else {
                        printErrMsg(1,
                                    "Wrong format of a decimal number. A number is required after a dot, but %c was given",
                                    c);
                        return LEX_ERROR;
                    }
                    decimal = true;
                } else if (c == 'e' || c == 'E') {
                    if (decimal_e) {
                        printErrMsg(1, "Wrong format of a decimal number. Multiple exponent expressions were used.");
                        return LEX_ERROR;
                    }
                    decimal_e = true;
                    strAddChar(&attr, c);
                    c = getchar();
                    if (isdigit(c) || c == '+' || c == '-') {
                        strAddChar(&attr, c);
                    } else {
                        printErrMsg(1,
                                    "Wrong format of a decimal number. A number or '+' or '-' is required after an exponent expression, but %c was given",
                                    c);
                        return LEX_ERROR;
                    }
                } else if (isalpha(c) || c == '_') {
                    printErrMsg(1, "Wrong number format. %c char was given but a number is required", c);
                    return LEX_ERROR;
                } else {
                    ungetc(c, stdin);
                    if (decimal || decimal_e) {
                        return DECIMAL_NUMBER;
                    }
                    return NUMBER;
                }
                break;

            case 6: // String
                if (c == EOL || c == EOF) {
                    printErrMsg(1, "Wrong string format. '\"' is required to end the string but EOL or EOF was given");
                    return LEX_ERROR;
                } else if (c == 34) {
                    return STRING_EXPRESSION;
                }
                strAddChar(&attr, c);
                break;
        }
    }
}