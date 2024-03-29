/**
 * @project IFJ17
 * @file parser.h
 * @author Roman Bartl (xbartl06)
 * @author Tomas Odehnal (xodehn08)
 * @brief  Parsing all blocks, statements and expressions (.h file)
 */

#ifndef IFJ2017PROJECT_PARSER_H
#define IFJ2017PROJECT_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include "scanner.h"
#include "error_codes.h"
#include "garbage_collector.h"
#include "symtable.h"
#include "precedence.h"
#include "ast.h"
#include "stack.h"

typedef struct {
    char* name;
    int argsNum;
    datatype types[3];
    datatype returnType;
} builtinFunction;

builtinFunction builtinFunctions[4];
bool inFunction;            // flag for checking if parser is in body of function (for return statement)
bool isExpression;
char *functionName;
stmtArray globalStmtArray;
Stack stmtStack;
token PreviousToken;        // global variable where is stored last token for check
BinaryTreePtr symtable;     // global variable - symtable (all identifiers with theirs specifications)
bool isRelativeOper;
BinaryTreePtr helpAssignVar;




/**
 * Main construction that decides if will be chosen 'function' non-terminal block or 'main program' non-terminal block.
 * Always jumps into 'main program' non-terminal.
 * But if terminal is DECLARE or FUNCTION, parser jumps into 'function' non-terminal.
 *
 * If any syntax error occurred in parsing the printErrAndExit() function is called and whole translation is stopped.
 */
void program();


/**
 * Here parser decides whether 'declare function' non-terminal will continue or 'function' non-terminal.
 * For 'declare function' non-terminal checks only function header.
 * For 'function' non-terminal checks function header then can follow block of statements and after that 'end function'.
 * Than calls 'function next' non-terminal.
 */
void functions();


/**
 * Checks main construction 'Scope End Scope'.
 * This block can contain countless statements.
 * After ending 'Scope' block 'EOF' must follow.
 */
void mainBody();


/**
 * This non-terminal is called by 'mainBody'.
 * It checks if current token is 'Scope' and following token is 'EOL'.
 */
void mainBodyIt();


/**
 * @brief Save name of currently parsed function.
 *
 * @param name Name of function from Token.
 */
void saveFunctionName(string name);


/**
 * Checks if given lexem is equal to 'ID'.
 * If not, calls printErrAndExit() function.
 *
 * @param Token
 */
void IdToken(token Token);


/**
 * Creates new node whenever is variable of function declared.
 *
 * @param name
 * @param type
 * @param declared
 * @param defined
 * @param isFunction
 * @param params
 */
void createNode(BinaryTreePtr *table, char *name, datatype type, bool declared,	bool defined,
				bool isFunction, BinaryTreePtr *params, datatype *typeOfParams, int paramNumber);


/**
 * Creates new node in tree of arguments of function
 *
 * @param params
 * @param name
 * @param type
 */
void createParamsNode(BinaryTreePtr *params, char *name, datatype type);


/**
 * Statements non-terminal.
 * Superfluous block can contain zero or countless statements.
 * In switch is decided what statement will be checked.
 * For 'ID' will be checked assignment. Flag 'unaryOp' is set on true because in this case can be used unary operations.
 * For declaration statement will be 'unaryOp' set on false because no one can simply use undeclared variable.
 * For 'INPUT' is checked if only 'ID' follows.
 * For 'PRINT' statement must expression be finished by ';'. 'PRINT' can handle with more expressions or variables.
 * Block 'DO WHILE' can also contain another statements. Ends with terminal 'LOOP'.
 * Block 'IF-THEN' can be completed with countless 'ELSEIF' statements or 'ELSE' statement. Calls ifNext() function
 * to check if any of these two terminals is next in line.
 * 'RETURN' statement can be placed only in 'function' block.
 *
 * In the end of every single one statement must follow 'EOL'.
 */
void statement();


/**
 * Checks if following terminal is 'ASSIGNMENT' or unary operation.
 * If not and previous statement is declaration, then function can end with no error. If previous terminal was only 'ID'
 * so no declaration and any of assignment terminals is used, then ends with error.
 * But if previous statement was declaration and unary operation is used, also ends with error.
 * If assignment is possible then will store value from expression to symtable in node where is stored identifier of
 * given name
 *
 * @param isDeclaration
 * @param name
 */
void assignment(bool isDeclaration, char *name);


/**
 * Checks if given Token contains unary operator.
 *
 * @param Token
 * @return - TRUE if Token contains this operator, otherwise returns FALSE.
 */
bool unaryOperation(token Token);


/**
 * Checks if given terminal is equal to 'FUNCTION' or 'DECLARE' terminal.
 *
 * @param lexem
 * @return - TRUE if terminal is equal to these terminals, otherwise returns FALSE.
 */
bool functionFirst(int lexem);


/**
 * Is called by 'function' non-terminal and checks right format of function header.
 * First terminal is 'FUNCTION' then 'ID' follows. After must be brackets with optional parameters and then
 * data type of return value
 *
 * Optional parameters are check by declareParams() function and data type is checked by asDataType() function.
 */
void functionHeader(bool isDeclared, bool isDefined);


/**
 * Checks if another terminal is 'AS' and then must follow right data type which is check by function dataType().
 */
void asDataType(datatype *type);


/**
 * Checks if another terminal is right data type.
 */
void dataType(datatype *type);


/**
 * Checks if following terminal is 'FUNCTION' or 'DECLARE'.
 * If yes then calls again function() non-terminal.
 * If not the function ends because presumes that this terminal is 'Scope' -
 * - so the block of functions will not be check anymore.
 */
void functionNext();


/**
 * Checks if function block ends with 'END', 'FUNCTION' and 'EOL' terminals.
 */
void functionEnd();


/**
 * Checks following token.
 * If terminal in token is not 'ELSE' of 'ELSEIF' then ends.
 * If the terminal is equal to one of these two terminals then chooses a branch that will be checked.
 * For 'ELSEIF' calls elseif() function and for 'ELSE' checks only if the following terminal is 'EOL'
 * then calls statement() function.
 */
void ifNext(int* pushCounter);


/**
 * Following non-terminal must be condition (some expression) then following 'THEN' and 'EOL' terminals.
 * After that follows block of statements.
 * And if next token is 'ELSEIF' then the function calls itself to check another 'ELSEIF' statement.
 * If terminal is not 'ELSEIF' then returns back to ifNext() function.
 */
void elseIf(int* pushCounter);


/**
 * This non-terminal is called only at function declaration or definition.
 * Checks format of given parameter or parameters ('ID', 'AS' and data type).
 * Then calls declareParamsNext() function to check if there are more parameters.
 */
void declareParams(BinaryTreePtr node, BinaryTreePtr *params, datatype **typeOfParams, int *paramNumber);


/**
 * This function is called by declareParams() and checks if following terminal is comma, if not then ends because assumes
 * that no other parameter will be given.
 * If following terminal is comma, then calls again declareParams() to check another parameter.
 */
void declareParamsNext(BinaryTreePtr node, BinaryTreePtr *params, datatype **typeOfParams, int *paramNumber);


/**
 * This function is called in statement() function if 'PRINT' terminal was given.
 * Checks if another expression to print out is given.
 * If not then ends.
 * If yes then checks if semicolon character is following and then calls function itself to check if another expression
 * will be printed out.
 */
void printNext();


/**
 * Checks if given lexem is equal to 'END'.
 * If not, calls printErrAndExit() function.
 *
 * @param Token
 */
void end(token Token);


/**
 * Checks if given lexem is equal to 'EOL'.
 * If not, calls printErrAndExit() function.
 *
 * @param lexem
 */
void eol(token Token);


/**
 *
 */
void params(bool builtIn, int *paramNumber);


/**
 *
 */
void paramsNext(bool builtIn, int *paramNumber);

#endif