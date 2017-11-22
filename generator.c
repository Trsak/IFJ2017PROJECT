/**
 * @file generator.c
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */
//TODO:  KONVERZE PROMĚNNÝCH!!
//TODO: String + string
//TODO: VAR ASSIGN = getVarFrame()!

#include <string.h>
#include <ctype.h>
#include "generator.h"
#include "symtable.h"
#include "parser.h"

void startGenerating() {
    printf(".IFJcode17\n");
    currentRegister = 0;
    currentHelpRegister = 0;
    currentWhile = 0;
    inside = NOWHERE;
    inScope = false;

    frame = (char *) gcmalloc(3 * sizeof(char));
    strcpy(frame, "TF");

    printf("JUMP $$main\n");

    generateCode(globalStmtArray);
}

void generateCode(stmtArray block) {
    for (int i = 0; i < block.length; i++) {
        switch (block.array[i].tag_stmt) {
            case scope_stmt:
                inScope = true;
                strcpy(frame, "GF");
                printf("LABEL $$main\n");
                break;
            case input_stmt:
                getInput(block.array[i].op.input_stmt.variable);
                break;
            case var_decl_stmt:
                varDeclare(block.array[i].op.var_decl_stmt.variable);
                break;
            case var_assign_stmt:
                varAssign(block.array[i].op.var_assign_stmt.left,
                          block.array[i].op.var_assign_stmt.expression);
                break;
            case while_stmt:
                whileStatement(block.array[i].op.while_stmt.condition,
                               block.array[i].op.while_stmt.block);
                break;
            case print_stmt:
                printStatement(block.array[i].op.print_stmt.expression);
                break;
            default: //TODO remove
                break;
        }
    }
}

void getInput(BinaryTreePtr var) {
    switch (var->data.type) {
        case TYPE_NUMBER:
            printf("READ %s@%s int\n", getVarFrame(), var->data.name);
            break;
        case TYPE_DECIMAL:
            printf("READ %s@%s float\n", getVarFrame(), var->data.name);
            break;
        case TYPE_STRING:
            printf("READ %s@%s string\n", getVarFrame(), var->data.name);
            break;
    }
}

void whileStatement(ast_exp *condition, stmtArray block) {
    switch (condition->tag_exp) {
        case binaryExp: {
            char *whileLabel = getNewWhile();
            inside = IN_WHILE;

            printf("LABEL %%WL%d\n", currentWhile);
            strcpy(frame, "TF");
            printf("CREATEFRAME\n");

            generateBinaryExp(condition);
            inside = NOWHERE;

            generateCode(block);

            if (inScope) {
                strcpy(frame, "GF");
            }

            printf("JUMP %s\n", whileLabel);
            printf("LABEL %sN\n", whileLabel);
            break;
        }
        default:
            break;
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
            printf("WRITE %s@%s\n", getVarFrame(), expression->op.variableExp->data.name);
            break;
        case binaryExp: {
            char *reg = getRegister();
            int nextReg = currentRegister;

            printf("DEFVAR %s@%s\n", frame, reg);
            generateBinaryExp(expression);
            printf("MOVE %s@%s %s@%%R%d\n", frame, reg, frame, nextReg);
            printf("WRITE %s@%s\n", frame, reg); //TODO VAR!!
            break;
        }
        default: //TODO remove
            break;
    }
}

void varDeclare(BinaryTreePtr var) {
    printf("DEFVAR %s@%s\n", getVarFrame(), var->data.name);

    switch (var->data.type) {
        case TYPE_NUMBER:
            printf("MOVE %s@%s int@0\n", getVarFrame(), var->data.name);
            break;
        case TYPE_DECIMAL:
            printf("MOVE %s@%s float@0\n", getVarFrame(), var->data.name);
            break;
        case TYPE_STRING:
            printf("MOVE %s@%s string@\n", getVarFrame(), var->data.name);
            break;
    }
}

void varAssign(BinaryTreePtr var, ast_exp *expression) {
    switch (expression->tag_exp) {
        case integerExp:
            printf("MOVE %s@%s int@%d\n", getVarFrame(), var->data.name, expression->op.numberExp);
            break;
        case doubleExp:
            printf("MOVE %s@%s float@%g\n", getVarFrame(), var->data.name, expression->op.decimalExp);
            break;
        case stringExp:
            printf("MOVE %s@%s string@%s\n", getVarFrame(), var->data.name, expression->op.stringExp.str);
            break;
        case variableExp:
            printf("MOVE %s@%s %s@%s\n", getVarFrame(), var->data.name, getVarFrame(),
                   expression->op.variableExp->data.name);
            break;
        case binaryExp: {
            int nextReg = currentRegister;
            generateBinaryExp(expression);
            printf("MOVE %s@%s %s@%%R%d\n", getVarFrame(), var->data.name, frame, nextReg);
            break;
        }
        default:
            break;
    }
}

void generateBinaryExp(ast_exp *expression) {
    char *reg = getRegister();
    printf("DEFVAR %s@%s\n", frame, reg);

    switch (expression->tag_exp) {
        case binaryExp: {
            ast_exp *left = expression->op.binaryExp.left;
            generateBinaryExp(expression->op.binaryExp.right);

            switch (left->tag_exp) {
                case integerExp:
                    printf("MOVE %s@%s %s\n", frame, reg, generateIntegerSymbol(left->op.numberExp));

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "\\") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg1, frame, reg);
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg2, frame, getNextRegister(reg));
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, hReg1, frame, hReg1, frame, hReg2);
                        printf("FLOAT2INT %s@%s %s@%s\n", frame, reg, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    }

                    break;
                case doubleExp:
                    printf("MOVE %s@%s %s\n", frame, reg, generateFloatSymbol(left->op.decimalExp));

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "/") == 0) {
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    }

                    break;
                case variableExp:
                    printf("MOVE %s@%s %s@%s\n", frame, reg, getVarFrame(), left->op.variableExp->data.name);

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "/") == 0) {
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "\\") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg1, frame, reg);
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg2, frame, getNextRegister(reg));
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, hReg1, frame, hReg1, frame, hReg2);
                        printf("FLOAT2INT %s@%s %s@%s\n", frame, reg, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    }

                    break;
                case bracketExp:
                    generateBinaryExp(left->op.bracketExp.expression);
                    printf("MOVE %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg));
                    printf("MOVE %s@%s %s@%s\n", frame, getNextRegister(reg), frame,
                           getNextRegister(getNextRegister(reg)));

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "/") == 0) {
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "\\") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg1, frame, reg);
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg2, frame, getNextRegister(reg));
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, hReg1, frame, hReg1, frame, hReg2);
                        printf("FLOAT2INT %s@%s %s@%s\n", frame, reg, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentWhile, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentWhile, frame, hReg1, frame, hReg2);
                    }

                    break;
                default: //TODO OTHER
                    break;
            }

            break;
        }
        case integerExp:
            printf("MOVE %s@%s %s\n", frame, reg, generateIntegerSymbol(expression->op.numberExp));
            break;
        case variableExp:
            printf("MOVE %s@%s %s@%s\n", frame, reg, getVarFrame(), expression->op.variableExp->data.name);
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

char *getHelpRegister() {
    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%HR%d", currentHelpRegister++);
    return reg;
}

char *getNewWhile() {
    char *whileLabel = (char *) gcmalloc(30 * sizeof(char));
    sprintf(whileLabel, "%%WL%d", ++currentWhile);
    return whileLabel;
}

char *getVarFrame() {
    char *frameTP = (char *) gcmalloc(30 * sizeof(char));
    if (!inScope) {
        strcpy(frameTP, "TF");
    } else {
        strcpy(frameTP, "GF");
    }

    return frameTP;
}