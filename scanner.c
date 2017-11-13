/**
 * @file scanner.c
 * @author Jan Bartosek (xbarto92)
 * @brief Returns "tokens" that represent each word or char from stdin to the parser or the LEX_ERROR if a lexical error occurred
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

unsigned line = 0; // Line Counter (for err messages, exp.: The syntax error is on line 5)

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

//TODO Every calling of printErrMsg should have as the first argument ERROR_SCANNER but it doesn't work for any reason. Temporary replaced by 1.

/**
 * @copydoc getNextToken
 */
token getNextToken() {

	token T; //TODO - CALL function strFree(attr) !!
	if (tokenInit(&T) == ERROR_INTERNAL) {
		printErrMsg(ERROR_INTERNAL, "There is a memory error while allocating token structure.");
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
				    EOL) { // If there are multiple EOLs, returns only one and increments line counter with every EOL
					line++;
					c = getchar();
					while (isspace(c) && c != EOF) {
						if (c == EOL) {
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
						printErrMsg(ERROR_SCANNER,
						            "Error on line: %d - The '!' was given and the following should be '\"' but %c was given",
						            T.line, c);
						T.lexem = LEX_ERROR;
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
				} else {
					printErrMsg(1, "Error on line: %d - Unknown character was given: %c", T.line, c);
					T.lexem = LEX_ERROR;
					return T;
				}
				break;

			case 1: // One-Line Comment
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
					printErrMsg(1, "Error on line: %d - Never-Ending Multi-Line Comment", T.line);
					T.lexem = LEX_ERROR;
					return T;
				}
				break;

			case 4: // ID or Keyword TODO the length of ID and save it to the binary tree!!

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
					T.lexem = ID;
					return T;
				}
				break;


			case 5: // It's a Number
				if (isdigit(c)) {
					strAddChar(&T.value, c);
				} else if (c == '.') {
					if (decimal || decimal_e) {
						printErrMsg(1, "Error on line: %d - Wrong format of a decimal number. Multiple dots were used.",
						            T.line);
						T.lexem = LEX_ERROR;
						return T;
					}
					strAddChar(&T.value, c);
					c = getchar();
					if (isdigit(c)) {
						strAddChar(&T.value, c);
					} else {
						printErrMsg(1,
						            "Error on line: %d - Wrong format of a decimal number. A number is required after a dot, but %c was given",
						            T.line, c);
						T.lexem = LEX_ERROR;
						return T;
					}
					decimal = true;
				} else if (c == 'e' || c == 'E') {
					if (decimal_e) {
						printErrMsg(1,
						            "Error on line: %d - Wrong format of a decimal number. Multiple exponent expressions were used.",
						            T.line);
						T.lexem = LEX_ERROR;
						return T;
					}
					decimal_e = true;
					strAddChar(&T.value, c);
					c = getchar();
					if (isdigit(c) || c == '+' || c == '-') {
						strAddChar(&T.value, c);
					} else {
						printErrMsg(1,
						            "Error on line: %d - Wrong format of a decimal number. A number or '+' or '-' is required after an exponent expression, but %c was given",
						            T.line, c);
						T.lexem = LEX_ERROR;
						return T;
					}
				} else if (isalpha(c) || c == '_') {
					printErrMsg(1,
					            "Error on line: %d - Wrong number format. %c char was given but a number is required",
					            T.line, c);
					T.lexem = LEX_ERROR;
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
					printErrMsg(1,
					            "Error on line: %d - Wrong string format. '\"' is required to end the string but EOL or EOF was given",
					            T.line);
					T.lexem = LEX_ERROR;
					return T;
				} else if (c == 34) {
					T.lexem = STRING_EXPRESSION;
					return T;
				}
				strAddChar(&T.value, c);
				break;
		}
	}
}