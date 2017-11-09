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
	ID,
	NUMBER,
	PLUS, // +
	MINUS, // -
	MULTIPLY, // *
	DIVISION, // /
	INCREMENT, // ++
	DECREMENT, // --
	ASSIGNMENT, // =
	LESS, // <
	LESS_EQUAL, // <=
	GREATER, // >
	GREATER_EQUAL, // >=
	NOT_EQUAL ,// <>
	DECIMAL_NUMBER, // 123.456 or 123e456 or 123.456E-789
	STRING_EXPRESSION, // Has to start: !"my string" .. returns only my string.
	BRACKET_LEFT, // (
	BRACKET_RIGHT, // )
	COMMA, // ,
	BACKSLASH, // backslash
	SEMICOLON, // ;
} lexems;

typedef enum
{
	AS,
	ASC,
	DECLARE,
	DIM,
	DO,
	DOUBLE,
	ELSE,
	END,
	CHR,
	FUNCTION,
	IF,
	INPUT,
	INTEGER,
	LENGTH,
	LOOP,
	PRINT,
	RETURN,
	SCOPE,
	STRING,
	SUBSTR,
	THEN,
	WHILE,
	AND,
	BOOLEAN,
	CONTINUE,
	ELSEIF,
	EXIT,
	FALSE,
	FOR,
	NEXT,
	NOT,
	OR,
	SHARED,
	STATIC,
	TRUE,
} keywords;

#define EOL '\n'
#define EOF '\0'

/*
// Lexems
#define ID 0
#define NUM 1
#define PLUS 2 // +
#define MINUS 3 // -
#define MUL 4 // *
#define DIV 5 // /
#define INC 6 // ++
#define DEC 7 // --
#define ASSIGNMENT 8 // =
#define LEQ 9 // <=
#define NEQ 10 // <>
#define LTN 11 // <
#define DECIMAL_NUM 12 // 123.456 or 123e456 or 123.456E-789
#define STRING_EXPRESSION 13 // Has to start: !"my string" .. returns only my string.
#define BRACKET_LEFT 14 // (
#define BRACKET_RIGHT 15 // )
#define COMMA 16 // ,
#define BACK_DIV 17 // backslash
#define SEMICOLON 18 // ;

// Keywords
#define AS 20
#define ASC 21
#define DECLARE 22
#define DIM 23
#define DO 24
#define DOUBLE 25
#define ELSE 26
#define END 27
#define CHR 28
#define FUNCTION 29
#define IF 30
#define INPUT 31
#define INTEGER 32
#define LENGTH 33
#define LOOP 34
#define PRINT 35
#define RETURN 36
#define SCOPE 37
#define STRING 38
#define SUBSTR 39
#define THEN 40
#define WHILE 41
#define AND 42
#define BOOLEAN 43
#define CONTINUE 44
#define ELSEIF 45
#define EXIT 46
#define FALSE 47
#define FOR 48
#define NEXT 49
#define NOT 50
#define OR 51
#define SHARED 52
#define STATIC 53
#define TRUE 54
#define EOL 55
*/

// Keywords
/*
 * The value of keywords increments depending on position in array.
 * The value of the keyword "As" is 20, then "Asc" is equal to 21 ...
 * The last value "True" is set to 54.
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