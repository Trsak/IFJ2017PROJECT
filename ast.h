/**
 * @file ast.h
 * @author Tomas Odehnal (xodehn08)
 * @brief Implementation of Abstract Syntax Tree (AST)
 */

#ifndef IFJ2017PROJECT_AST_H
#define IFJ2017PROJECT_AST_H

#include <stdbool.h>
#include "strings.h"
#include "symtable.h"

typedef struct Exp {
	enum { integerExp, doubleExp, stringExp, variableExp,
		binaryExp, unaryExp, bracketExp} tag_exp;

	union {
		int	numberExp;
		double	decimalExp;
		string	stringExp;
		BinaryTreePtr	variableExp;
		struct {
			string oper;
			struct Exp* left;
			struct Exp* right;
		} binaryExp;

		struct {
			string oper;
			struct Exp* operand;
		} unaryExp;

		struct {
			string leftBracket;
			struct Exp* expression;
			string rightBracket;
		} bracketExp;
	} op;

	enum {exp_integer = 1, exp_decimal = 2, exp_string = 3} datatype;

} ast_exp;

typedef struct functionArgs {
	ast_exp *argument;
	struct functionArgs *next;
} functionArgs;

typedef struct {
	struct Stmt* array;
	int length;
} stmtArray;

enum builtin_function {
	Length = 0, SubStr, Asc, Chr
};

typedef struct Stmt {
	enum {while_stmt, var_decl_stmt, var_decl_assign_stmt,
		function_decl_stmt, function_definition_stmt, var_assign_function_stmt, var_assign_builtin_function_stmt,
		var_assign_stmt, if_stmt, return_stmt, input_stmt, print_stmt, scope_stmt, optimalization_stmt} tag_stmt;

	union {
		struct {
			ast_exp* condition;
			stmtArray block;
		} while_stmt;

		struct {
			BinaryTreePtr variable;
		} var_decl_stmt;

		struct {
			struct Stmt* left_decl;
			ast_exp* right;
		} var_decl_assign_stmt;

		struct {
			BinaryTreePtr left;
			ast_exp* expression;
		} var_assign_stmt;

		struct {
			BinaryTreePtr function;
			functionArgs *args;
		} function_decl_stmt;

		struct {
			BinaryTreePtr function;
			functionArgs *args;
			stmtArray block;
		} function_definition_stmt;

		struct {
			enum builtin_function function;
			functionArgs *args;
		} var_assign_builtin_function_stmt;

		struct {
			BinaryTreePtr left;
			BinaryTreePtr function;
			functionArgs *args;
		} var_assign_function_stmt;

		struct {
			ast_exp* ret;
		} return_stmt;

		struct {
			ast_exp* condition;
			stmtArray ifBlock;
			struct Stmt* elseStmt;
		} if_stmt;

		struct {
			BinaryTreePtr variable;
		} input_stmt;

		struct {
			ast_exp* expression;
		} print_stmt;

        struct {
            int nothing;
        } optimalization_stmt;
	} op;
} ast_stmt;

void stmtArrayInit(stmtArray* array);

void addStmtToArray(stmtArray* array, ast_stmt* stmt);

void addArgumentToArray(functionArgs** args, ast_exp* argument);


/** =====EXPRESSION FUNCTIONS===== */
/**
 * @brief Create node in AST for number (integer). Represents E -> INTEGER
 * @param number Number (integer)
 * @return Pointer to AST node (expression).
 */
ast_exp* make_numberExp(int number);

/**
 * @brief Create node in AST for number (double). Represents E -> DOUBLE
 * @param number Number (double)
 * @return Pointer to AST node (expression).
 */
ast_exp* make_decimalExp(double number);

/**
 * @brief Create node in AST for string (char *). Represents E -> STRING
 * @param str String
 * @return Pointer to AST node (expression).
 */
ast_exp* make_stringExp(string str);

/**
 * @brief Create node in AST for variable. Represents E -> id
 * @param variable Variable
 * @return Pointer to AST node (expression).
 */
ast_exp* make_variableExp(BinaryTreePtr variable);

/**
 * @brief Create node in AST of binary expression. Represents E -> E <operator> E
 * Example: E -> E + E
 *
 * @param oper Operator
 * @param left Left operand
 * @param right Right operand
 * @return Pointer to AST node (expression).
 */
ast_exp* make_binaryExp(string oper, ast_exp *left, ast_exp* right, datatype dtype);

/**
 * @brief Create node in AST of binary expression. Represents E -> <operator>E
 * Example: E -> -E
 *
 * @param oper Operator
 * @param operand Operand
 * @return Pointer to AST node (expression).
 */
ast_exp* make_unaryExp(string oper, ast_exp *operand, datatype dtype);

/**
 * @brief Create node in AST of expression in brackets. Represents E -> (E)
 * @param leftBracket Left bracket
 * @param expression Abstract syntax subtree for expression in brackets.
 * @param rightBracket Right bracket
 * @return Pointer to AST node (expression).
 */
ast_exp* make_bracketExp(string leftBracket, ast_exp *expression, string rightBracket, datatype dtype);


/** =====STATEMENT FUNCTIONS===== */
/**
 * @brief Create node in AST of while loop.
 * @param condition Condition of while loop (expression).
 * @param code_block Pointer to array of statements in while loop.
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_whileStmt(ast_exp* condition, stmtArray code_block);

/**
 * @brief Create node in AST of variable declaration statement.
 * @param variable
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_varDeclStmt(BinaryTreePtr variable);

/**
 * @brief Create node in AST of variable declaration with inicialization statement.
 * @param left_decl
 * @param right
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_varDeclAssignStmt(ast_stmt* left_decl, ast_exp* right);

/**
 * @brief Create node in AST of function declaration statement.
 * @param function
 * @param args
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_functionDeclStmt(BinaryTreePtr function, functionArgs *args);

/**
 * @brief Create node in AST of function definition statement.
 * @param function
 * @param args
 * @param code_block
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_functionDefStmt(BinaryTreePtr function, functionArgs *args, stmtArray code_block);

/**
 * @brief Create node in AST of call function statement.
 * @param args
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_varAssignBuiltinFunctionStmt(enum builtin_function function, functionArgs *args);

/**
 * @brief Create node in AST of assignment to varible of calling function statement.
 * @param left
 * @param callingFunction
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_varAssignFunctionStmt(BinaryTreePtr left, BinaryTreePtr function, functionArgs* args);

/**
 * @brief Create node in AST of assignment to variable statement.
 * @param left Variable (as ast_exp*) to store the result of expression.
 * @param expression Expression to be assigned to variable
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_varAssignStmt(BinaryTreePtr left, ast_exp* expression);

/**
 * @brief Create node in AST of return statement.
 * @param ret
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_returnStmt(ast_exp* ret);

/**
 * @brief Create node in AST of if statement.
 *
 * @param condition
 * @param ifBlock
 * @param elseStmt
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_ifStmt(ast_exp* condition, stmtArray ifBlock, ast_stmt* elseStmt);

/**
 * @brief Create node in AST of input statement.
 *
 * @param identifier Identifier to store input from user
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_inputStmt(BinaryTreePtr variable);

/**
 * @brief Create node in AST of print statement.
 *
 * @param expression Expression (it's result) to print to standard output
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_printStmt(ast_exp* expression);

/**
 * @brief Create node in AST to show where the main block begins.
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_scopeStmt();

/**
 * @brief Create node in AST for optimalization purpose.
 * @return Pointer to AST node (statement).
 */
ast_stmt* make_optimalizationStmt();

#endif //IFJ2017PROJECT_AST_H
