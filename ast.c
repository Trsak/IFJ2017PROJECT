/**
 * @file ast.c
 * @author Tomas Odehnal (xodehn08)
 * @brief Implementation of Abstract Syntax Tree (AST)
 */

#include "ast.h"

void addArgumentToArray(functionArgs* args, ast_exp* argument) {
	while(args->next != NULL) {
		args = args->next;
	}
	args->next = (functionArgs*) gcmalloc(sizeof(functionArgs));
	args->argument = argument;
}


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


/**
 * @copydoc
 */
ast_stmt* make_whileStmt(ast_exp* condition, stmtArray code_block) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = while_stmt;
	e->op.while_stmt.condition = condition;
	e->op.while_stmt.block = code_block;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_varDeclStmt(ast_exp* variable) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = var_decl_stmt;
	e->op.var_decl_stmt.variable = variable;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_varDeclAssignStmt(ast_stmt* left_decl, ast_exp* right) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = var_decl_assign_stmt;
	e->op.var_decl_assign_stmt.left_decl = left_decl;
	e->op.var_decl_assign_stmt.right = right;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_functionDeclStmt(BinaryTreePtr function, functionArgs *args) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = function_decl_stmt;
	e->op.function_decl_stmt.function = function;
	e->op.function_decl_stmt.args = args;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_functionDefStmt(BinaryTreePtr function, functionArgs *args, stmtArray code_block) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = function_definition_stmt;
	e->op.function_definition_stmt.function = function;
	e->op.function_definition_stmt.args = args;
	e->op.function_definition_stmt.block = code_block;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_callFunctionStmt(BinaryTreePtr function, functionArgs *args) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = call_function_stmt;
	e->op.call_function_stmt.function = function;
	e->op.call_function_stmt.args = args;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_varAssignFunctionStmt(ast_exp* left, ast_stmt* callingFunction) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = var_assign_stmt;
	e->op.var_assign_function_stmt.left = left;
	e->op.var_assign_function_stmt.callFunction = callingFunction;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_returnStmt(ast_exp* ret) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = return_stmt;
	e->op.return_stmt.ret = ret;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_ifStmt(ast_exp* condition, stmtArray ifBlock, ast_stmt* elseStmt) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = if_stmt;
	e->op.if_stmt.condition = condition;
	e->op.if_stmt.ifBlock = ifBlock;
	e->op.if_stmt.elseStmt = elseStmt;
	return e;
}