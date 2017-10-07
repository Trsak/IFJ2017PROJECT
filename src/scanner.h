/**
 * @file main.c
 * @author Jan Bartosek
 * @brief
 */
#ifndef UNTITLED_SCANER_H
#define UNTITLED_SCANER_H
#endif //UNTITLED_SCANER_H

// Lexémy
#define ID 0
#define NUM 1
#define PLUS 2 // +
#define MINUS 3 // -
#define MUL 4 // *
#define DIV 5 // /
#define INC 6 // ++
#define DEC 7 // --
#define ASSIGNMENT 8 // :=
#define LEQ 9 // <=
#define NEQ 10 // <>
#define LTN 11 // <

// Klíčová slova
/*
 * Hodnota klíčových slov se inkrementuje v závislosti na jejich pořadí v poli.
 * Hodnota výrazu "As" je 20, výrazu "Asc" je 21 atd..
 * Poslední hodnota "True" je rovna 54.
 * Všechny hodnoty jsou rozepsány v souboru keyWordsHelper.txt
 */
char* keyWords[] = { "As", "Asc", "Declare", "Dim", "Do", "Double", "Else", "End", "Chr",
                     "Function", "If", "Input", "Integer", "Length", "Loop", "Print",
                     "Return", "Scope", "String", "SubStr", "Then", "While", "And",
                     "Boolean", "Continue", "Elseif", "Exit", "False", "For", "Next",
                     "Not", "Or", "Shared", "Static", "True" };

// Speciální znaky
#define END_OF_FILE 55

// Chybové hlášky
#define LEX_ERROR -1

// Fce
void setSourceFile(FILE *f);
int getNextToken();