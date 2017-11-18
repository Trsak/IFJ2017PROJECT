//
// Created by todehnal on 15.11.17.
//

#ifndef IFJ2017PROJECT_AST_H
#define IFJ2017PROJECT_AST_H

#include <stdbool.h>
#include "strings.h"
#include "symtable.h"

typedef struct Exp {
	enum { integer_exp, string_exp, variable_exp,
		binary_exp, unary_exp} tag;

	union {
		int	numberExp;
		double	decimalExp;
		string	stringExp;
		BinaryTreePtr	variableExp;
		struct {
			string operator;
			struct Exp* left;
			struct Exp* right;
		} binaryExp;

		struct {
			string operator;
			struct Exp* operand;
		} unaryExp;

	} op;
} ast_exp;

typedef struct Stmt {
	enum {while_stmt, var_decl_stmt, var_decl_assign_stmt,
		function_decl_stmt, function_definition_stmt, var_assign_function_stmt,
		var_assign_stmt} tag;

	union {
		struct {
			struct ast_exp* condition;
			struct Stmt* block;
		} while_stmt;

		struct {
			struct ast_exp* variable;
		} var_decl_stmt;

		struct {
			struct Stmt* left_decl;
			struct ast_exp* right;
		} var_decl_assign_stmt;

		struct {
			BinaryTreePtr variable;
			// TODO: arguments
		} function_decl_stmt;
	} op;
} ast_stmt;

#endif //IFJ2017PROJECT_AST_H
