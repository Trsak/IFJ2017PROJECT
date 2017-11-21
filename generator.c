/**
 * @file generator.c
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#include "generator.h"
#include "symtable.h"
#include "parser.h"

void startGenerating() {
    printf(".IFJcode17\n");

    for (int i = 0; i < globalStmtArray.length; i++) {
        switch (globalStmtArray.array[i].tag_stmt) {
            case var_assign_stmt:
                varAssign(globalStmtArray.array[i].op.var_assign_stmt.left,
                          globalStmtArray.array[i].op.var_assign_stmt.expression);
                break;
            case print_stmt:
                printExpression(globalStmtArray.array[i].op.print_stmt.expression);
                break;
            default: //TODO remove
                break;
        }
    }
}

void printExpression(ast_exp *expression) {
    switch (expression->tag_exp) {
        case integerExp:
            printf("WRITE int@%d\n", expression->op.numberExp);
            break;
        case doubleExp:
            printf("WRITE float@%g\n", expression->op.decimalExp);
            break;
        case stringExp:
            printf("WRITE string@%s\n", expression->op.stringExp.str);
            break;
        case variableExp:
            printf("WRITE GF@%s\n", expression->op.variableExp->data.name);
            break;
        case binaryExp:
            printBinaryExpression(expression); //TODO :O
            break;
        default: //TODO remove
            break;
    }
}

void varAssign(BinaryTreePtr var, ast_exp *expression) {
    printf("DEFVAR GF@%s\n", var->data.name);

    switch (var->data.type) {
        case TYPE_NUMBER:
            printf("MOVE GF@%s int@%d\n", var->data.name, getIntegerExpressionValue(expression));
            break;
        case TYPE_DECIMAL:
            printf("MOVE GF@%s float@%g\n", var->data.name, getFloatExpressionValue(expression));
            break;
        case TYPE_STRING:
            printf("MOVE GF@%s string@%s\n", var->data.name, getStringExpressionValue(expression));
            break;
    }
}

void printBinaryExpression(ast_exp *expression) {
    (void) expression;
}

int getIntegerExpressionValue(ast_exp *expression) {
    switch (expression->tag_exp) {
        case integerExp:
            return expression->op.numberExp;
        case variableExp:
            return atoi(expression->op.variableExp->data.value.str);
        case binaryExp:
            if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                return getIntegerExpressionValue(expression->op.binaryExp.left) +
                       getIntegerExpressionValue(expression->op.binaryExp.right);
            } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                return getIntegerExpressionValue(expression->op.binaryExp.left) -
                       getIntegerExpressionValue(expression->op.binaryExp.right);
            } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                return getIntegerExpressionValue(expression->op.binaryExp.left) *
                       getIntegerExpressionValue(expression->op.binaryExp.right);
            }
            break;
        case bracketExp:
            return getIntegerExpressionValue(expression->op.bracketExp.expression);
        default: //TODO remove
            break;
    }

    return 0;
}

double getFloatExpressionValue(ast_exp *expression) {
    (void) expression;
    return 0.0f;
}

char *getStringExpressionValue(ast_exp *expression) {
    (void) expression;
    return "";
}