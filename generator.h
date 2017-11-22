/**
 * @file generator.h
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#ifndef IFJ2017PROJECT_GENERATOR_H
#define IFJ2017PROJECT_GENERATOR_H

#include <stdbool.h>
#include "parser.h"

bool inScope;

int currentRegister;

int currentHelpRegister;

int currentWhile;

char *frame;

typedef enum {
    NOWHERE = 0,
    IN_WHILE = 1,
    IN_IF = 2
} inL;

inL inside;
/**
 * @return void
 *
 * Starts code generating
 */
void startGenerating();

void generateCode(stmtArray block);

/**
 * @return void
 *
 * Generates code to print given expression
 */
void printStatement(ast_exp *expression);

/**
 * @return void
 *
 * Generates code to assign value for variable
 */
void varAssign(BinaryTreePtr var, ast_exp *expression);

void getInput(BinaryTreePtr var);

char *generateSymbol(datatype type, char *value);

char *getRegister();

void generateBinaryExp(ast_exp *expression);

char *generateIntegerSymbol(int value);

char *getNextRegister(char *nextReg);

char *getHelpRegister();

char *getNewWhile();

char *generateFloatSymbol(double value);

void varDeclare(BinaryTreePtr var);

void whileStatement(ast_exp *condition, stmtArray block);

char *getVarFrame();

#endif //IFJ2017PROJECT_GENERATOR_H
