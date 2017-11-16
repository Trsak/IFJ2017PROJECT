//
// Created by todehnal on 15.11.17.
//

#ifndef IFJ2017PROJECT_AST_H
#define IFJ2017PROJECT_AST_H

typedef struct Exp {
	enum { integer_exp, string_exp, variable_exp,
		binary_exp, unary_exp, call_exp,
		projection_exp, record_exp } tag;

	union {
		int		numberExp;
		double	decimalExp;
		string	stringExp;
		string	variableExp;
		struct {
			string operator;
			struct Exp* left;
			struct Exp* right;
		} binaryExp;

		struct {
			string operator;
			struct Exp* operand;
		} unaryExp;

		struct {
			string name;
			struct Exp_list* arguments;
		} callExp;

	} op;
} ast;

#endif //IFJ2017PROJECT_AST_H
