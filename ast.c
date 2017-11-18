/**
 * @file ast.c
 * @author Tomas Odehnal (xodehn08)
 * @brief Implementation of Abstract Syntax Tree (AST)
 */

#include "ast.h"

/**
 * @copydoc
 */
ast_exp* make_numberExp(int number) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = integerExp;
	e->op.numberExp = number;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_decimalExp(double number) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = doubleExp;
	e->op.decimalExp = number;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_stringExp(string str) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = stringExp;
	e->op.stringExp = str;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_variableExp(BinaryTreePtr variable) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = variableExp;
	e->op.variableExp = variable;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_binaryExp(string oper, ast_exp* left, ast_exp* right ) {
	ast_exp* e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = binaryExp;
	e->op.binaryExp.oper = oper;
	e->op.binaryExp.left = left;
	e->op.binaryExp.right = right;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_unaryExp(string oper, ast_exp* operand) {
	ast_exp* e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = unaryExp;
	e->op.unaryExp.oper = oper;
	e->op.unaryExp.operand = operand;
	return e;
}