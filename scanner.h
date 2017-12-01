/**
 * @project IFJ17
 * @file scanner.h
 * @author Jan Bartosek (xbarto92)
 * @brief Defines the values for every posible lexem that can come from stdin.
 */

#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "strings.h"
#include "error_codes.h"
#include "garbage_collector.h"

typedef enum {
	ID = 0, // _ or ABC_abc95
	NUMBER, // int value 123456
	DECIMAL_NUMBER, // float value 123.456 or 123e456 or 123.456E-789
	STRING_EXPRESSION, // Has to start: !"my string" .. returns only my string.

	PLUS, // +
	MINUS, // -
	MULTIPLY, // *
	DIVISION, // /
	BACKSLASH, // \.
	ASSIGNMENT, // =

	PLUS_ASSIGNMENT, // +=
	MINUS_ASSIGNMENT, // -=
	MULTIPLY_ASSIGNMENT, // *=
	DIVISION_ASSIGNMENT, // /=
	BACKSLASH_ASSIGNMENT, // \=

	LESS, // <
	LESS_EQUAL, // <=
	GREATER, // >
	GREATER_EQUAL, // >=
	NOT_EQUAL,// <>

	BRACKET_LEFT, // (
	BRACKET_RIGHT, // )
	COMMA, // ,
	SEMICOLON, // ;

	//Keywords
	AS = 30,
	DECLARE = 31,
	DIM = 32,
	DO = 33,
	DOUBLE = 34,
	ELSE = 35,
	END = 36,
	FUNCTION = 37,
	IF = 38,
	INPUT = 39,
	INTEGER = 40,
	LOOP = 41,
	PRINT = 42,
	RETURN = 43,
	SCOPE = 44,
	STRING = 45,
	THEN = 46,
	WHILE = 47,
	AND = 48,
	BOOLEAN = 49,
	CONTINUE = 50,
	ELSEIF = 51,
	EXIT = 52,
	FALSE = 53,
	FOR = 54,
	NEXT = 55,
	NOT = 56,
	OR = 57,
	SHARED = 58,
	STATIC = 59,
	TRUE = 60,
    EOL_ENUM = 61,
	help = -1
} lexems;

typedef struct {
	lexems lexem;   // type of the read word
	unsigned line;  // line counter
	string value;   // if an ID was found, contains it's name, if it was a number it has it's value. The same with a string.
} token;

#define EOL '\n'

// Keywords
/*
 * The value of keywords increments depending on position in array.
 * The value of the keyword "As" is 30, then "Asc" is equal to 31 ...
 * The last value "True" is set to 64.
 * All the values can be found above
 */
extern char *keyWords[];

#define MAX_ID_LENGTH 128

/**
 * @param T - this is a token structure to operate with.
 * @return - value 99 if a memory error occurred or 0
 *
 * Function sets up a structure token.
 */
int tokenInit(token *T);

/**
 * @param T - this is a token structure to operate with.
 *
 * Destroys the whole structure
 */
void tokenFree(token *T);

/**
 * @return - value of the last lexem, line and a parameter if needed
 *
 * Function is called in parser.h to get value of every single word or char from stdin.
 * It reads char by char from the stdin until it can decide what kind of word it is.
 * Then it sends back a "token" of lexems datatype that represents what was read in the stdin.
 *
 * If an ID or a Number came in, the function saves the name in a symbol table represented by a binary tree.
 */
token getNextToken();

#endif