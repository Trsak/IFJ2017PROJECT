/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @brief
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "strings.h"

char *keyWords[] = {"as", "asc", "declare", "dim", "do", "double", "else", "end", "chr",
                    "function", "if", "input", "integer", "length", "loop", "print",
                    "return", "scope", "string", "substr", "then", "while", "and",
                    "boolean", "continue", "elseif", "exit", "false", "for", "next",
                    "not", "or", "shared", "static", "true"};

string attr; // Global variable used for attribute sending

FILE *source;

void setSourceFile(FILE *f) {
	source = f;
	strInit(&attr);
}

int getNextToken(string *attr) {
	int state = 0;
	int c;
	strClear(attr);

	while (1) {
		c = getc(source);

		switch (state) {
			case 0: // The beggining
				if (c == EOL) {
					return EOL;
				}
				else if (isspace(c)) {  // It's a white space (ignore that!)
					break;
				} else if (c == 39){ // It's an one line comment
					while (c == EOL){
						c = getc(source);
						if (c == EOF){
							return END_OF_FILE;
						}
					}
				} else if (c == '/') { // It's a multi line comment or division
					state = 1;
				} else if (isalpha(c) || c == '_') { // It's an ID or keyword
					strAddChar(attr, toLower(c));
					state = 2;
				} else if (isdigit(c)) { // It's a number
					strAddChar(attr, c);
					state = 3;
				} else if (c == ':') { // It's an assignment
					state = 4;
				} else if (c == '+') { // It's plus or increment
					state = 5;
				} else if (c == '-') { // It's minus or decrement
					state = 6;
				} else if (c == '<') { // It's one of compare operators
					state = 7;
				} else if (c == '!') { // It's a string
					state = 8;
				} else if (c == '*') {
					return MUL;
				} else if (c == '(') {
					return BRACKET_LEFT;
				} else if (c == ')') {
					return BRACKET_RIGHT;
				} else if (c == ',') {
					return COMMA;
				} else if (c == EOF) {
					return END_OF_FILE;
				} else {
					return LEX_ERROR;
				}
				break;

			case 1: // Comment
				if (c == 39) {  // ASCII 39 == '
					c = getc(source);
					while (c == 39){
						c = getc(source);
						if (c == EOF) {
							return LEX_ERROR; // Never-ending comment
						}
					}
					c = getc(source);
					if (c == '/'){
						state = 0;
					}
					else {
						ungetc(c, source);
						ungetc(c, source);
					}
				} else{
					ungetc(c, source);
					return DIV;
				}
				break;

			case 2: // ID or Keyword
				if (isalnum(c) || c == '_') {
					strAddChar(attr, toLower(c));
				} else {
					ungetc(c, source);
				}
				for (unsigned int i = 0; i < sizeof(keyWords); i++) {
					if (strcmp(attr->str, keyWords[i])) {
						return 20 + i;
					}
				}
				return ID;

			case 3: // Number
				static bool decimal = false;
				static bool decimal_e = false;
				if (isdigit(c)) {
					strAddChar(attr, c);
				} else if (c == '.'){
					if (decimal || decimal_e){
						return LEX_ERROR;
					}
					decimal = true;
					strAddChar(attr, c);
				} else if (c == 'e' || c == 'E'){
					if (decimal_e){
						return LEX_ERROR;
					}
					decimal_e = true;
					strAddChar(attr, c);
					c = getc(source);
					if (isdigit(c) || c == '+' || c == '-'){
						strAddChar(attr, c);
					} else {
						return LEX_ERROR;
					}
				} else if (isalpha(c)) {
					return LEX_ERROR;
				} else {
					ungetc(c, source);
					if (decimal || decimal_e){
						return DECIMAL_NUM;
					}
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

			case 7: // <= or <> or <
				if (c == '=') {
					return LEQ;
				} else if (c == '>') {
					return NEQ;
				} else {
					ungetc(c, source);
					return LTN;
				}
			case 8: // String
				if (c == 34){ // ASCII 34 == "
					while (1){
						c = getc(source);
						if (c == 34){
							return STRING;
						} else if (c == EOF){
							return  LEX_ERROR; // Never-ending string
						} else {
							strAddChar(attr, c);
						}
					}
				}

		}
	}
}