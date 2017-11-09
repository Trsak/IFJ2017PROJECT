/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @author Roman Bartl (xbartl06)
 * @brief Defines the values for every lexem that can show in an input file.
 */


#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

typedef enum
{
	ID, // _ or ABC_abc95
	NUMBER, // int value 123456
	DECIMAL_NUMBER, // float value 123.456 or 123e456 or 123.456E-789
	STRING_EXPRESSION, // Has to start: !"my string" .. returns only my string.

	PLUS, // +
	MINUS, // -
	MULTIPLY, // *
	DIVISION, // /
	BACKSLASH, // \.

	PLUS_ASSIGNMENT, // +=
	MINUS_ASSIGNMENT, // -=
	MULTIPLY_ASSIGNMENT, // *=
	DIVISION_ASSIGNMENT, // /=
	BACKSLASH_ASSIGNMENT, // \=

	ASSIGNMENT, // =
	LESS, // <
	LESS_EQUAL, // <=
	GREATER, // >
	GREATER_EQUAL, // >=
	NOT_EQUAL ,// <>

	BRACKET_LEFT, // (
	BRACKET_RIGHT, // )
	COMMA, // ,
	SEMICOLON // ;
} lexems;

typedef enum
{
	AS = 0,
	ASC = 1,
	DECLARE = 2,
	DIM = 3,
	DO = 4,
	DOUBLE = 5,
	ELSE = 6,
	END = 7,
	CHR = 8,
	FUNCTION = 9,
	IF = 10,
	INPUT = 11,
	INTEGER = 12,
	LENGTH = 13,
	LOOP = 14,
	PRINT = 15,
	RETURN = 16,
	SCOPE = 17,
	STRING = 18,
	SUBSTR = 19,
	THEN = 20,
	WHILE = 21,
	AND = 22,
	BOOLEAN = 23,
	CONTINUE = 24,
	ELSEIF = 25,
	EXIT = 26,
	FALSE = 27,
	FOR = 28,
	NEXT = 29,
	NOT = 30,
	OR = 31,
	SHARED = 32,
	STATIC = 33,
	TRUE = 34
} keywords;

#define EOL '\n'
#define EOF '\0'

// Keywords
/*
 * The value of keywords increments depending on position in array.
 * The value of the keyword "As" is 0, then "Asc" is equal to 1 ...
 * The last value "True" is set to 34.
 * All the values can be found in keyWordsHelper.txt
 */
extern char *keyWords[];

// Error codes
#define LEX_ERROR -1

/**
 * @param FILE - this is an input file in IFJ2017 language.
 */
void setSourceFile(FILE *f);

/**
 * @return - integer value of the last unread lexem
 *
 * The value can be found above in file scanner.h
 *
 * @brief Function is called in parser for every single word from input file.
 */
int getNextToken();

#endif