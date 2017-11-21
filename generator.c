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
            if (!expression->op.variableExp->data.defined) {
                switch (expression->op.variableExp->data.type) {
                    case TYPE_NUMBER:
                        printf("WRITE int@0\n");
                        break;
                    case TYPE_DECIMAL:
                        printf("WRITE float@0\n");
                        break;
                    case TYPE_STRING:
                        printf("WRITE string@\n");
                        break;
                }
            } else {
                printf("WRITE GF@%s\n", expression->op.variableExp->data.name);
            }
            break;
        case binaryExp:
            //TODO
            break;
        default: //TODO remove
            break;
    }
}

void varAssign(BinaryTreePtr var, ast_exp *expression) {
    printf("DEFVAR GF@%s\n", var->data.name);

    switch (expression->tag_exp) {
        case integerExp:
            printf("MOVE GF@%s int@%d\n", var->data.name, expression->op.numberExp);
            break;
        case doubleExp:
            printf("MOVE GF@%s float@%g\n", var->data.name, expression->op.decimalExp);
            break;
        case stringExp:
            printf("MOVE GF@%s string@%s\n", var->data.name, expression->op.stringExp.str);
            break;
        case variableExp:
            printf("MOVE GF@%s GF@%s\n", var->data.name, expression->op.variableExp->data.name);
            break;
        case binaryExp: {
            string lastOp = expression->op.binaryExp.oper;
            ast_exp *lastExpression = expression;

            while (expression->op.binaryExp.right->tag_exp == binaryExp) {
                lastExpression = expression;
                expression = expression->op.binaryExp.right;

                if (expression->tag_exp == binaryExp) {
                    datatype var1;
                    datatype var2;

                    if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("DEFVAR GF@%s\n", "dd");

                        switch (expression->op.binaryExp.left->tag_exp) {
                            case integerExp:
                                var1 = TYPE_NUMBER;
                                break;
                            case doubleExp:
                                var1 = TYPE_DECIMAL; //TODO VAR
                                break;
                            default:
                                break;
                        }

                        switch (expression->op.binaryExp.right->tag_exp) {
                            case integerExp:
                                var2 = TYPE_NUMBER;
                                printf("YEY\n");
                                break;
                            case doubleExp:
                                var2 = TYPE_DECIMAL;
                                break;
                            default:
                                break;
                        }
                        if (var2 == TYPE_NUMBER && var1 == TYPE_NUMBER) { //Both are integers
                            printf("MUL GF@%s %s %s\n", "dd",
                                   generateSymbol(TYPE_NUMBER, "5"),
                                   generateSymbol(TYPE_NUMBER, "6"));
                        }
                    }
                }

                lastOp = expression->op.binaryExp.oper;
            }

            break;
        }
    }
}

char *generateSymbol(datatype type, char *value) {
    char *typeString;

    switch (type) {
        case TYPE_NUMBER:
            typeString = "int@";
            break;
        case TYPE_DECIMAL:
            typeString = "float@";
            break;
        case TYPE_STRING:
            typeString = "string@";
            break;
    }
    sprintf(value, "%s%s", typeString, value);

    return value;
}
/*
void integerVarAssign(const char *variable, ast_exp *expression) {
    switch (expression->tag_exp) {
        case integerExp:
            printf("MOVE GF@%s int@%d\n", variable, expression->op.numberExp);
            break;
        case variableExp:
            printf("MOVE GF@%s GF@%s\n", variable, expression->op.variableExp->data.name);
            break;

    }
}

void getIntegerExpressionValue(const char *variable, ast_exp *expression) {
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
            } else if (strcmp(expression->op.binaryExp.oper.str, "\\") == 0) {
                return getIntegerExpressionValue(expression->op.binaryExp.left) /
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
}*/