/**
 * @file scanner.c
 * @author Jan Bartosek (xbarto92)
 * @author Petr Šopf (xsopfp00)
 * @brief Returns "tokens" that represent each word or char from stdin to the parser
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
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

unsigned line = 1; // Line Counter (for err messages, exp.: The syntax error is on line 5)

/**
 * @copydoc tokenInit
 */
int tokenInit(token *T) {
    string attr;
    if (strInit(&attr) == ERROR_INTERNAL) {
        return ERROR_INTERNAL;
    }
    T->value = attr;
    T->line = line;
    return 0;
}

/**
 * @copydoc tokenFree
 */
void tokenFree(token *T) {
    strFree(&T->value);
}

//TODO Every calling of printErrAndExit  should have as the first argument ERROR_SCANNER but it doesn't work for any reason. Temporary replaced by 1.

/**
 * @copydoc getNextToken
 */
token getNextToken() {

    token T; //TODO - CALL function strFree(attr) !!
    if (tokenInit(&T) == ERROR_INTERNAL) {
        printErrAndExit(ERROR_INTERNAL, "There is a memory error while allocating token structure.");
        T.lexem = ERROR_INTERNAL;
        return T;
    }

    int state = 0;
    int c;

    bool decimal = false;
    bool decimal_e = false;

    while (1) {
        c = getchar();
        switch (state) {
            case 0: // The beginning

                if (c ==
                    '\n') { // If there are multiple EOLs, returns only one and increments line counter with every EOL
                    line++;
                    while (isspace(c)) {
                        c = getchar();
                        if (c == '\n') {
                            line++;
                        }
                    }
                    ungetc(c, stdin);
                    T.lexem = EOL;
                    return T;
                } else if (c == EOF) {
                    T.lexem = EOF;
                    return T;
                } else if (c == 39) { // It's an one-line comment
                    state = 1;
                } else if (c == '/') { // It's a multi-line comment or division
                    state = 2;
                } else if (isspace(c)) {  // It's a white space (ignore that!)
                    break;
                } else if (isalpha(c) || c == '_') { // It's an ID or a keyword
                    strAddChar(&T.value, tolower(c));
                    state = 4;
                } else if (isdigit(c)) { // It's a number
                    strAddChar(&T.value, c);
                    state = 5;
                } else if (c == 33) { // It's a string
                    c = getchar();
                    if (c == 34) { // ASCII 34 == "
                        state = 6;
                    } else if (c == EOL || c == EOF) {
                        printErrAndExit(ERROR_SCANNER,
                                        "on line: %d - The '!' was given and the following should be '\"' but %c was given",
                                        T.line, c);
                        return T;
                    }
                } else if (c == '+') { // It's a plus
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            T.lexem = PLUS;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = PLUS_ASSIGNMENT;
                        return T;
                    }
                    ungetc(c, stdin);
                    T.lexem = PLUS;
                    return T;
                } else if (c == '-') { // It's minus or decrement
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            T.lexem = MINUS;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = MINUS_ASSIGNMENT;
                        return T;
                    }
                    ungetc(c, stdin);
                    T.lexem = MINUS;
                    return T;
                } else if (c == '*') {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            T.lexem = MULTIPLY;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = MULTIPLY_ASSIGNMENT;
                        return T;
                    }
                    ungetc(c, stdin);
                    T.lexem = MULTIPLY;
                    return T;
                } else if (c == 92) {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            T.lexem = BACKSLASH;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = BACKSLASH_ASSIGNMENT;
                        return T;
                    }
                    ungetc(c, stdin);
                    T.lexem = BACKSLASH;
                    return T;
                } else if (c == '(') {
                    T.lexem = BRACKET_LEFT;
                    return T;
                } else if (c == ')') {
                    T.lexem = BRACKET_RIGHT;
                    return T;
                } else if (c == ',') {
                    T.lexem = COMMA;
                    return T;
                } else if (c == ';') {
                    T.lexem = SEMICOLON;
                    return T;
                } else if (c == '=') {
                    T.lexem = ASSIGNMENT;
                    return T;
                } else if (c == '<') {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            T.lexem = LESS;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = LESS_EQUAL;
                        return T;
                    } else if (c == '>') {
                        T.lexem = NOT_EQUAL;
                        return T;
                    } else {
                        ungetc(c, stdin);
                        T.lexem = LESS;
                        return T;
                    }
                } else if (c == '>') {
                    c = getchar();
                    while (isspace(c)) {
                        if (c == EOL || c == EOF) {
                            ungetc(c, stdin);
                            T.lexem = GREATER;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = GREATER_EQUAL;
                        return T;
                    } else {
                        ungetc(c, stdin);
                        T.lexem = GREATER;
                        return T;
                    }
                } else if (c == '&') {
                    c = getchar();
                    if (c == 'B') {
                        state = 7;
                    } else if (c == 'O') {
                        state = 8;
                    } else if (c == 'H') {
                        state = 9;
                    }
                } else {
                    printErrAndExit(ERROR_SCANNER, "on line: %d - Unknown character was given: %c", T.line, c);
                    return T;
                }
                break;

            case 1: //one-Line Comment
                if (c == EOL) {
                    state = 0;
                    ungetc(c, stdin);
                } else if (c == EOF) {
                    T.lexem = EOF;
                    return T;
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
                            T.lexem = DIVISION;
                            return T;
                        }
                        c = getchar();
                    }
                    if (c == '=') {
                        T.lexem = DIVISION_ASSIGNMENT;
                        return T;
                    }
                    ungetc(c, stdin);
                    T.lexem = DIVISION;
                    return T;
                }

            case 3: // Multi-Line Comment
                if (c == 39) {
                    c = getchar();
                    if (c == '/') {
                        state = 0;
                    }
                } else if (c == EOF) {
                    printErrAndExit(ERROR_SCANNER, "on line: %d - Never-Ending Multi-Line Comment", T.line);
                    return T;
                }
                break;

            case 4: // ID or Keyword

                if (isalnum(c) || c == '_') {
                    strAddChar(&T.value, tolower(c));
                } else {
                    ungetc(c, stdin);
                    for (unsigned int i = 0; i < sizeof(keyWords) / sizeof(char *); i++) {
                        if (strcmp((&T.value)->str, keyWords[i]) == 0) {
                            T.lexem = 30 + i;
                            return T;
                        }
                    }
                    if (T.value.length > MAX_ID_LENGTH) {
                        T.value.str[MAX_ID_LENGTH] = '\0';
                    }
                    T.lexem = ID;
                    return T;
                }
                break;


            case 5: // It's a Number
                if (isdigit(c)) {
                    strAddChar(&T.value, c);
                } else if (c == '.') {
                    if (decimal || decimal_e) {
                        printErrAndExit(ERROR_SCANNER,
                                        "on line: %d - Wrong format of a decimal number. Multiple dots were used.",
                                        T.line);
                        return T;
                    }
                    strAddChar(&T.value, c);
                    c = getchar();
                    if (isdigit(c)) {
                        strAddChar(&T.value, c);
                    } else {
                        printErrAndExit(ERROR_SCANNER,
                                        "on line: %d - Wrong format of a decimal number. A number is required after a dot, but %c was given",
                                        T.line, c);
                        return T;
                    }
                    decimal = true;
                } else if (c == 'e' || c == 'E') {
                    if (decimal_e) {
                        printErrAndExit(ERROR_SCANNER,
                                        "on line: %d - Wrong format of a decimal number. Multiple exponent expressions were used.",
                                        T.line);
                        return T;
                    }
                    decimal_e = true;
                    strAddChar(&T.value, c);
                    c = getchar();
                    if (isdigit(c) || c == '+' || c == '-') {
                        strAddChar(&T.value, c);
                    } else {
                        printErrAndExit(ERROR_SCANNER,
                                        "on line: %d - Wrong format of a decimal number. A number or '+' or '-' is required after an exponent expression, but %c was given",
                                        T.line, c);
                        return T;
                    }
                } else if (isalpha(c) || c == '_') {
                    printErrAndExit(ERROR_SCANNER,
                                    "on line: %d - Wrong number format. %c char was given but a number is required",
                                    T.line, c);
                    return T;
                } else {
                    ungetc(c, stdin);
                    if (decimal || decimal_e) {
                        T.lexem = DECIMAL_NUMBER;
                        return T;
                    }
                    T.lexem = NUMBER;
                    return T;
                }
                break;

            case 6: // String
                if (c == EOL || c == EOF) {
                    printErrAndExit(ERROR_SCANNER,
                                    "on line: %d - Wrong string format. '\"' is required to end the string but EOL or EOF was given",
                                    T.line);
                    return T;
                } else if (c == 34) {
                    T.lexem = STRING_EXPRESSION;
                    return T;
                }

                int ascii = (int) c;

                if (ascii == 92) {
                    char next = getchar();
                    if (next == '"') {
                        strAddChar(&T.value, '"');
                    } else if (next == 'n') {
                        strAddChar(&T.value, (char) 92);
                        strAddChar(&T.value, '0');
                        strAddChar(&T.value, '1');
                        strAddChar(&T.value, '0');
                    } else if (next == 't') {
                        strAddChar(&T.value, (char) 92);
                        strAddChar(&T.value, '0');
                        strAddChar(&T.value, '0');
                        strAddChar(&T.value, '9');
                    } else if (next == (char) 92) {
                        strAddChar(&T.value, (char) 92);
                        strAddChar(&T.value, '0');
                        strAddChar(&T.value, '9');
                        strAddChar(&T.value, '2');
                    } else if (next >= '0' && next <= '9') {
                        char second = getchar();
                        if (second >= '0' && second <= '9') {
                            char third = getchar();
                            if (third >= '0' && third <= '9') {
                                strAddChar(&T.value, (char) 92);
                                strAddChar(&T.value, next);
                                strAddChar(&T.value, second);
                                strAddChar(&T.value, third);
                            } else {
                                ungetc(next, stdin);
                                ungetc(second, stdin);
                                ungetc(third, stdin);
                            }
                        } else {
                            ungetc(next, stdin);
                            ungetc(second, stdin);
                        }

                    } else {
                        ungetc(next, stdin);
                    }
                } else if ((ascii >= 0 && ascii <= 32) || ascii == 35) {
                    strAddChar(&T.value, (char) 92);
                    strAddChar(&T.value, '0');

                    if (ascii < 10) {
                        strAddChar(&T.value, '0');
                    } else {
                        int asciiNext = ascii / 10;
                        strAddChar(&T.value, (char) (asciiNext + 48));
                    }

                    ascii = ascii % 10;
                    strAddChar(&T.value, (char) (ascii + 48));
                } else {
                    strAddChar(&T.value, c);
                }

                break;

            case 7: // Binary number
                if (isdigit(c)) {
                    if (c == '0' || c == '1') {
                        strAddChar(&T.value, c);
                        break;
                    }
                    printErrAndExit(ERROR_SCANNER,
                                    "on line: %d - Binary number input contains number bigger than 1",
                                    T.line);
                } else {
                    ungetc(c, stdin);
                    int val = strtol(T.value.str, 0, 2);
                    sprintf(T.value.str, "%d", val);
                    T.lexem = NUMBER;
                    return T;
                }

            case 8: // Octal number
                if (isdigit(c)) {
                    if (c == '8' || c == '9') {
                        printErrAndExit(ERROR_SCANNER,
                                        "on line: %d - Octal number input contains number bigger than 7",
                                        T.line);
                    }
                    strAddChar(&T.value, c);
                    break;
                } else {
                    ungetc(c, stdin);
                    int val = strtol(T.value.str, 0, 8);
                    sprintf(T.value.str, "%d", val);
                    T.lexem = NUMBER;
                    return T;
                }

            case 9: // Hex number
                if (isalnum(c)) {
                    if (isdigit(c) || c == 'a' || c == 'A' || c == 'b' || c == 'B' || c == 'c' || c == 'C' ||
                        c == 'd' || c == 'D' || c == 'e' || c == 'E' || c == 'f' || c == 'F') {
                        strAddChar(&T.value, c);
                        break;
                    }
                    printErrAndExit(ERROR_SCANNER,
                                    "on line: %d - Octal number input contains letter bigger than F",
                                    T.line);
                } else {
                    ungetc(c, stdin);
                    int val = strtol(T.value.str, 0, 16);
                    strSizeUp(&T.value);
                    sprintf(T.value.str, "%d", val);
                    T.lexem = NUMBER;
                    return T;
                }
        }
    }
}











