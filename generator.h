/**
 * @project IFJ17
 * @file generator.h
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#ifndef IFJ2017PROJECT_GENERATOR_H
#define IFJ2017PROJECT_GENERATOR_H

#include <stdbool.h>
#include "parser.h"

/** Variable for getting right registers */
bool inScope;

/** ID of current register */
int currentRegister;

/** ID of current help register */
int currentHelpRegister;

/** ID of current label */
int currentLabel;

/** For whiles inside whiles */
int whileCount;

/** Name of current frame */
char *frame;

/** Current function */
BinaryTreePtr currentFunction;

/**
 * @return void
 *
 * Inits all variables and starts generating
 */
void startGenerating();

/**
 * @return void
 * @param block Block of code
 *
 * Generates block of code
 */
void generateCode(stmtArray block);

/**
 * @return void
 *
 * Generates code for builtin functions
 */
void generateBuiltInFunctions();

/**
 * @return void
 * @param condition Condition expression
 * @param ifBlock Block of code inside if
 * @param elseStmt Pointer to next if-else or else
 *
 * Generates if, else-if and else statements and their block of codes
 */
void generateIf(ast_exp *condition, stmtArray ifBlock, struct Stmt *elseStmt);

/**
 * @return void
 * @param condition Condition expression
 * @param block Block of code inside whle
 *
 * Generates while and its block of codes
 */
void generateWhile(ast_exp *condition, stmtArray block);

/**
 * @return void
 * @param expression Return expression
 *
 * Generates return
 */
void generateReturn(ast_exp *expression);

/**
 * @return void
 * @param expression Expression to generate
 *
 * Generates expressions
 */
void generateExp(ast_exp *expression);

/**
 * @return void
 * @param operand1 First operand
 * @param operand2 Second operand
 * @param operatorStr Operator string
 * @param operand1Type First datatype
 * @param operand2Type Second datatype
 *
 * Generates expressions
 */
void generateOperation(char *destination, char *operand1, char *operand2, char *operatorStr, datatype operand1Type, datatype operand2Type);

/**
 * @return void
 * @param operatorStr Operator string
 * @param operand1 First operand
 * @param operand2 Second operand
 *
 * Generates data conversions
 */
void generateDataConversion(char *operand1, char *operand2, char *operatorStr);

/**
 * @return true if conversion is needed
 * @param reg Register
 * @param value Value
 * @param type Current datatype
 * @param destType Needed datatype
 *
 * Checks if conversion is needed, returns true and generates it when its needed
 */
bool generateImplicitConversion(char *reg, char *value, datatype type, datatype destType);

/**
 * @return void
 * @param arg Argument
 * @param argType Argument datatype
 * @param destType Needed datatype
 *
 * Generates argument implicit conversions
 */
void generateArgumentsConversion(char *arg, datatype argType, datatype destType);

/**
 * @return void
 * @param function Function to generate
 * @param args Function arguments
 * @param block Block of code inside functions
 *
 * Generates functions
 */
void generateFunction(BinaryTreePtr function, functionArgs *args, stmtArray block);

/**
 * @return Generated symbol
 * @param value Value of symbol
 *
 * Generates symbol for integer
 */
char *generateIntegerSymbol(int value);

/**
 * @return Generated symbol
 * @param value Value of symbol
 *
 * Generates symbol for float
 */
char *generateFloatSymbol(double value);

/**
 * @return Generated symbol
 * @param type Datatype of symbol
 * @param value Value of symbol
 *
 * Generates symbol for all datatypes
 */
char *generateSymbol(datatype type, char *value);


/**
 * @return void
 * @param expression Print expression
 *
 * Generates code to print given expression
 */
void printStatement(ast_exp *expression);

/**
 * @return void
 * @param var Variable where the result of expression will be stored
 * @param expression Expression
 *
 * Generates code to assign value to variable
 */
void varAssign(BinaryTreePtr var, ast_exp *expression);

/**
 * @return void
 * @param var Variable to declare
 *
 * Generates code to declare variable and then define its default value
 */
void varDeclare(BinaryTreePtr var);

/**
 * @return void
 * @param var Variable where the input value will be stored
 *
 * Gets users input and stores it in var
 */
void getInput(BinaryTreePtr var);

/**
 * @return void
 * @param args Function arguments
 * @param function Function
 * @param left Variable where the result value will be stored
 *
 * Assigns function result to variable
 */
void assignFunction(functionArgs *args, BinaryTreePtr function, BinaryTreePtr left);

/**
 * @return void
 * @param args Function arguments
 * @param function Bultin function
 * @param left Variable where the result value will be stored
 *
 * Assigns builtin function result to variable
 */
void getBuiltinFunction(BinaryTreePtr left, functionArgs *args, enum builtin_function function);

/**
 * @return New register
 *
 * Creates new register
 */
char *getRegister();

/**
 * @return void
 * @param id Register ID
 *
 * Returns register with given ID
 */
char *getRegisterByID(int id);

/**
 * @return void
 * @param reg Register
 *
 * Gets next register
 */
char *getNextRegister(char *reg);

/**
 * @return void
 *
 * Creates new help register
 */
char *getHelpRegister();

/**
 * @return void
 *
 * Creates new label
 */
char *getNewLabel();

/**
 * @return void
 *
 * Gets frame for varibale
 */
char *getVarFrame();

/**
 * @return symbol
 * @param var variable
 *
 * Returns symbol for variable
 */
char *getVarSymbol(BinaryTreePtr var);

/**
 * @return Whole register name
 * @param reg Register name
 * @param location Register location
 *
 * Gets whole register name
 */
char *getWholeRegisterName(const char *reg, char *location);

#endif //IFJ2017PROJECT_GENERATOR_H
