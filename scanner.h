/**
 * @file scanner.h
 * @author Jan Bartosek (xbarto92)
 * @brief Defines the values for every posible lexem that can come from stdin.
 */

#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "strings.h"

typedef enum {
    ID, // _ or ABC_abc95
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
    ASC = 31,
    DECLARE = 32,
    DIM = 33,
    DO = 34,
    DOUBLE = 35,
    ELSE = 36,
    END = 37,
    CHR = 38,
    FUNCTION = 39,
    IF = 40,
    INPUT = 41,
    INTEGER = 42,
    LENGTH = 43,
    LOOP = 44,
    PRINT = 45,
    RETURN = 46,
    SCOPE = 47,
    STRING = 48,
    SUBSTR = 49,
    THEN = 50,
    WHILE = 51,
    AND = 52,
    BOOLEAN = 53,
    CONTINUE = 54,
    ELSEIF = 55,
    EXIT = 56,
    FALSE = 57,
    FOR = 58,
    NEXT = 59,
    NOT = 60,
    OR = 61,
    SHARED = 62,
    STATIC = 63,
    TRUE = 64,
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

// Error codes
#define LEX_ERROR -1

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