/**
 * @file ast.c
 * @author Tomas Odehnal (xodehn08)
 * @brief Implementation of Abstract Syntax Tree (AST)
 */

#include "ast.h"

void addArgumentToArray(functionArgs** args, ast_exp* argument) {
	if(*args == NULL) {
		(*args) = (functionArgs*) gcmalloc(sizeof(functionArgs));
		(*args)->argument = argument;
		(*args)->next = NULL;
	}
	else {
		functionArgs* tmp = *args;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = (functionArgs*) gcmalloc(sizeof(functionArgs));
		tmp = tmp->next;
		tmp->argument = argument;
		tmp->next = NULL;
	}
	//printf("%s\n", (*args)->argument->op.variableExp->data.name);
}

void stmtArrayInit(stmtArray* array) {
	array->length = 0;
	array->array = NULL;
}

void addStmtToArray(stmtArray* array, ast_stmt* stmt) {
	if(array->length == 0) {
		array->length++;
		array->array = (ast_stmt*) gcmalloc(array->length * sizeof(ast_stmt));
		array->array[array->length-1] = *stmt;
	}
	else {
		array->array = (ast_stmt *) gcrealloc(array->array, (array->length + 1) * sizeof(ast_stmt));
		array->array[array->length] = *stmt;
		array->length++;
	}
}


/**
 * @copydoc
 */
ast_exp* make_numberExp(int number) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = integerExp;
	e->op.numberExp = number;
	e->datatype = exp_integer;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_decimalExp(double number) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = doubleExp;
	e->op.decimalExp = number;
	e->datatype = exp_decimal;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_stringExp(string str) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = stringExp;
	e->op.stringExp = str;
	e->datatype = exp_string;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_variableExp(BinaryTreePtr variable) {
	ast_exp *e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = variableExp;
	e->op.variableExp = variable;
	e->datatype = variable->data.type;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_binaryExp(string oper, ast_exp* left, ast_exp* right, datatype dtype) {
	ast_exp* e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = binaryExp;
	e->op.binaryExp.oper = oper;
	e->op.binaryExp.left = left;
	e->op.binaryExp.right = right;
	e->datatype = dtype;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_unaryExp(string oper, ast_exp* operand, datatype dtype) {
	ast_exp* e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = unaryExp;
	e->op.unaryExp.oper = oper;
	e->op.unaryExp.operand = operand;
	e->datatype = dtype;
	return e;
}

/**
 * @copydoc
 */
ast_exp* make_bracketExp(string leftBracket, ast_exp *expression, string rightBracket, datatype dtype) {
	ast_exp* e = (ast_exp*) gcmalloc(sizeof(ast_exp));

	e->tag_exp = bracketExp;
	e->op.bracketExp.leftBracket = leftBracket;
	e->op.bracketExp.expression = expression;
	e->op.bracketExp.rightBracket = rightBracket;
	e->datatype = dtype;
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
ast_stmt* make_varAssignFunctionStmt(BinaryTreePtr left, ast_stmt* callingFunction) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = var_assign_function_stmt;
	e->op.var_assign_function_stmt.left = left;
	e->op.var_assign_function_stmt.callFunction = callingFunction;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_varAssignStmt(BinaryTreePtr left, ast_exp* expression) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = var_assign_stmt;
	e->op.var_assign_stmt.left = left;
	e->op.var_assign_stmt.expression = expression;
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

/**
 * @copydoc
 */
ast_stmt* make_inputStmt(ast_exp* identifier) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = input_stmt;
	e->op.input_stmt.identifier= identifier;
	return e;
}

/**
 * @copydoc
 */
ast_stmt* make_printStmt(ast_exp* expression) {
	ast_stmt* e = (ast_stmt*) gcmalloc(sizeof(ast_stmt));

	e->tag_stmt = print_stmt;
	e->op.print_stmt.expression = expression;
	return e;
}