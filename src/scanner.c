/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @brief
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "string.h"

char *keyWords[] = {"As", "Asc", "Declare", "Dim", "Do", "Double", "Else", "End", "Chr",
                    "Function", "If", "Input", "Integer", "Length", "Loop", "Print",
                    "Return", "Scope", "String", "SubStr", "Then", "While", "And",
                    "Boolean", "Continue", "Elseif", "Exit", "False", "For", "Next",
                    "Not", "Or", "Shared", "Static", "True"};

string attr; // Global variable used for attribute sending

FILE *source;

void setSourceFile(FILE *f) {
    source = f;
    strInit(&attr);
}

int getNextToken() {
    int state = 0;
    int c;
    strClear(&attr);

    while (1) {
        c = getc(source);

        switch (state) {
            case 0: // The beggining
                if (isspace(c)); // It's a white space (ignore that!)
                else if (c == '{') { // It's a comment
                    state = 1;
                } else if (isalpha(c)) { // It's an ID or keyword
                    strAddChar(&attr, c);
                    state = 2;
                } else if (isdigit(c)) { // It's a number
                    strAddChar(&attr, c);
                    state = 3;
                } else if (c == ':') { // It's an assignment
                    state = 4;
                } else if (c == '+') { // It's plus or increment
                    state = 5;
                } else if (c == '-') { // It's minus or decrement
                    state = 6;
                } else if (c == '<') {
                    state = 7;
                } else if (c == '*') {
                    return MUL;
                } else if (c == '/') {
                    return DIV;
                } else if (c == EOF) {
                    return END_OF_FILE;
                } else {
                    return LEX_ERROR;
                }
                break;

            case 1: // Comment
                if (c == '}') {
                    state = 0;
                } else if (c == EOF) { // Neverending comment
                    return LEX_ERROR;
                }
                break;

            case 2: // ID or Keyword
                if (isalnum(c)) {
                    strAddChar(&attr, c);
                } else {
                    ungetc(c, source);
                }
                for (int i = 0; i < sizeof(keyWords); i++) {
                    if (strcmp(&attr.str, keyWords[i])) {
                        return 20 + i;
                    }
                }
                return ID;

            case 3: // Number
                if (isdigit(c)) {
                    strAddChar(&attr, c);
                } else if (isalpha(c)) {
                    return LEX_ERROR;
                } else {
                    ungetc(c, source);
                    return NUM;
                }
                break;

            case 4: // Assignment
                if (c == '=') {
                    return ASSIGNMENT;
                }
                return LEX_ERROR;

            case 5: // Plus or Increment
                if (c == '+') {
                    return INC;
                } else {
                    ungetc(c, source);
                    return PLUS;
                }

            case 6: // Minus or Decrement
                if (c == '-') {
                    return DEC;
                } else {
                    ungetc(c, source);
                    return MINUS;
                }

                //TODO co je to za operátory?Je to správně?
            case 7: // <= or <> or <
                if (c == '=') {
                    return LEQ;
                } else if (c == '>') {
                    return NEQ;
                } else {
                    ungetc(c, source);
                    return LTN;
                }
        }
    }
}