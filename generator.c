/**
 * @file generator.c
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */
//TODO:  KONVERZE PROMĚNNÝCH!!

#include <string.h>
#include <ctype.h>
#include "generator.h"
#include "symtable.h"
#include "parser.h"

void startGenerating() {
    printf(".IFJcode17\n");
    currentRegister = 0;

    frame = (char *) gcmalloc(3 * sizeof(char));
    strcpy(frame, "TF");

    printf("JUMP $$main\n");

    for (int i = 0; i < globalStmtArray.length; i++) {
        switch (globalStmtArray.array[i].tag_stmt) {
            case scope_stmt:
                strcpy(frame, "GF");
                printf("LABEL $$main\n");
                break;
            case var_assign_stmt:
                varAssign(globalStmtArray.array[i].op.var_assign_stmt.left,
                          globalStmtArray.array[i].op.var_assign_stmt.expression);
                break;
            case print_stmt:
                printStatement(globalStmtArray.array[i].op.print_stmt.expression);
                break;
            default: //TODO remove
                break;
        }
    }
}

void printStatement(ast_exp *expression) {
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
            printf("WRITE %s@%s\n", frame, expression->op.variableExp->data.name);
            break;
        case binaryExp: {
            char *reg = getRegister();
            int nextReg = currentRegister;

            printf("DEFVAR %s@%s\n", frame, reg);
            generateBinaryExp(expression);
            printf("MOVE %s@%s %s@R%d\n", frame, reg, frame, nextReg);
            printf("WRITE %s@%s\n", frame, reg);
            break;
        }
        default: //TODO remove
            break;
    }
}

void varAssign(BinaryTreePtr var, ast_exp *expression) {
    printf("DEFVAR %s@%s\n", frame, var->data.name);

    switch (expression->tag_exp) {
        case integerExp:
            printf("MOVE %s@%s int@%d\n", frame, var->data.name, expression->op.numberExp);
            break;
        case doubleExp:
            printf("MOVE %s@%s float@%g\n", frame, var->data.name, expression->op.decimalExp);
            break;
        case stringExp:
            printf("MOVE %s@%s string@%s\n", frame, var->data.name, expression->op.stringExp.str);
            break;
        case variableExp:
            printf("MOVE %s@%s %s@%s\n", frame, var->data.name, frame, expression->op.variableExp->data.name);
            break;
        case binaryExp: {
            int nextReg = currentRegister;
            printf("DEFVAR %s@%s\n", frame, var->data.name);
            generateBinaryExp(expression);
            printf("MOVE %s@%s %s@R%d\n", frame, var->data.name, frame, nextReg);
            break;
        }
        default:
            break;
    }
}

void generateBinaryExp(ast_exp *expression) {
    //int saveToReg = currentRegister;
    char *reg = getRegister();
    printf("DEFVAR %s@%s\n", frame, reg);

    switch (expression->tag_exp) {
        case binaryExp: {
            ast_exp *left = expression->op.binaryExp.left;

            switch (left->tag_exp) {
                case integerExp:
                    generateBinaryExp(expression->op.binaryExp.right);
                    printf("MOVE %s@%s %s\n", frame, reg, generateIntegerSymbol(left->op.numberExp));

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "/") == 0) {
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    }

                    break;
                case doubleExp:
                    printf("MOVE %s@%s %s\n", frame, reg, generateFloatSymbol(left->op.decimalExp));
                    break;
                default: //TODO OTHER
                    break;
            }

            break;
        }
        case integerExp:
            printf("MOVE %s@%s %s\n", frame, reg, generateIntegerSymbol(expression->op.numberExp));
            break;
        default:
            break;
    }
}

char *generateSymbol(datatype type, char *value) {
    char *symbolString = (char *) gcmalloc((strlen(value) + 10) * sizeof(char));

    switch (type) {
        case TYPE_NUMBER:
            strcpy(symbolString, "int@");
            break;
        case TYPE_DECIMAL:
            strcpy(symbolString, "float@");
            break;
        case TYPE_STRING:
            strcpy(symbolString, "string@");
            break;
    }

    strcat(symbolString, value);
    return symbolString;
}

char *generateIntegerSymbol(int value) {
    char *symbolString = (char *) gcmalloc((25) * sizeof(char));

    sprintf(symbolString, "int@%d", value);
    return symbolString;
}

char *generateFloatSymbol(double value) {
    char *symbolString = (char *) gcmalloc((80) * sizeof(char));

    sprintf(symbolString, "float@%g", value);
    return symbolString;
}

char *getRegister() {
    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%R%d", currentRegister++);
    return reg;
}

char *getNextRegister(char *nextReg) {
    long val = 0;
    char *p = nextReg;

    while (*p) {
        if (isdigit(*p)) {
            val = strtol(p, &p, 10);
        } else {
            p++;
        }
    }

    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%R%ld", val + 1);
    return reg;
}