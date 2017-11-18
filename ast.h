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
		binaryExp, unaryExp} tag_exp;

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

	} op;
} ast_exp;

typedef struct functionArgs {
	ast_exp *argument;
	struct functionArgs *next;
} functionArgs;

typedef struct Stmt {
	enum {while_stmt, var_decl_stmt, var_decl_assign_stmt,
		function_decl_stmt, function_definition_stmt, var_assign_function_stmt, call_function_stmt,
		var_assign_stmt, if_stmt, return_stmt} tag_stmt;

	union {
		struct {
			ast_exp* condition;
			struct Stmt* block;
		} while_stmt;

		struct {
			ast_exp* variable;
		} var_decl_stmt;

		struct {
			struct Stmt* left_decl;
			ast_exp* right;
		} var_decl_assign_stmt;

		struct {
			BinaryTreePtr function;
			functionArgs *args;
		} function_decl_stmt;

		struct {
			BinaryTreePtr function;
			functionArgs *args;
			struct Stmt* block;
		} function_definition_stmt;

		struct {
			BinaryTreePtr function;
			functionArgs *args;
		} call_function_stmt;

		struct {
			ast_exp* left;
			struct Stmt* callFunction;
		} var_assign_function_stmt;

		struct {
			ast_exp* ret;
		} return_stmt;

		struct {
			ast_exp* condition;
			struct Stmt* ifBlock;
			struct Stmt* elseStmt;
		} if_stmt;
	} op;
} ast_stmt;

/**
 * @brief Create node in AST for number (integer). Represents E -> INTEGER
 * @param number Number (integer)
 * @return Pointer to AST node.
 */
ast_exp* make_numberExp(int number);

/**
 * @brief Create node in AST for number (double). Represents E -> DOUBLE
 * @param number Number (double)
 * @return Pointer to AST node.
 */
ast_exp* make_decimalExp(double number);

/**
 * @brief Create node in AST for string (char *). Represents E -> STRING
 * @param str String
 * @return Pointer to AST node.
 */
ast_exp* make_stringExp(string str);

/**
 * @brief Create node in AST for variable. Represents E -> id
 * @param variable Variable
 * @return Pointer to AST node.
 */
ast_exp* make_variableExp(BinaryTreePtr variable);

/**
 * @brief Create node in AST of binary expression. Represents E -> E <operator> E
 * Example: E -> E + E
 *
 * @param oper Operator
 * @param left Left operand
 * @param right Right operand
 * @return Pointer to AST node.
 */
ast_exp* make_binaryExp(string oper, ast_exp *left, ast_exp* right);

/**
 * @brief Create node in AST of binary expression. Represents E -> <operator>E
 * Example: E -> -E
 *
 * @param oper Operator
 * @param operand Operand
 * @return Pointer to AST node.
 */
ast_exp* make_unaryExp(string oper, ast_exp *operand);

#endif //IFJ2017PROJECT_AST_H
