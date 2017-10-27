/**
 * @file main.c
 * @author Jan Bartosek (xbarto92)
 * @author Roman Bartl (xbartl06)
 * @brief
 */


#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

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
#define STRING_EXPRESSION 13 // Has to start: !"my string" .. returns only my String.
#define BRACKET_LEFT 14 // (
#define BRACKET_RIGHT 15 // )
#define COMMA 16 // ,
#define BACK_DIV 17 // backslash
#define SEMICOLON // ;

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


// Keywords
/*
 * The value of keywords increments depending on position in array.
 * The value of the keyword "As" is 20, then "Asc" is equal to 21 ...
 * The last value "True" is set to 54.
 * All the values can be found in keyWordsHelper.txt
 */
extern char *keyWords[];

// Special chars
#define END_OF_LINE 55
#define END_OF_FILE 56

// Error codes
#define LEX_ERROR -1

// Fce
void setSourceFile(FILE *f);
int getNextToken();

#endif