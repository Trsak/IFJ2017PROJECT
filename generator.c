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

/**
 * @copydoc startGenerating
 */
void startGenerating() {
    printf(".IFJcode17\n");
    currentRegister = 0;
    currentHelpRegister = 0;
    currentLabel = 0;
    inScope = false;

    frame = (char *) gcmalloc(3 * sizeof(char));
    strcpy(frame, "TF");

    printf("JUMP $$main\n");

    generateCode(globalStmtArray);
}

/**
 * @copydoc generateCode
 */
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
            case if_stmt:
                generateIf(block.array[i].op.if_stmt.condition, block.array[i].op.if_stmt.ifBlock,
                           block.array[i].op.if_stmt.elseStmt);
                break;
            case function_definition_stmt:
                generateFunction(block.array[i].op.function_definition_stmt.function,
                                 block.array[i].op.function_definition_stmt.args,
                                 block.array[i].op.function_definition_stmt.block);
                break;
            case var_assign_function_stmt:
                assignFunction(block.array[i].op.var_assign_function_stmt.args,
                               block.array[i].op.var_assign_function_stmt.function,
                               block.array[i].op.var_assign_function_stmt.left);
                break;
            case var_assign_builtin_function_stmt:
                getBuiltinFunction(block.array[i].op.var_assign_builtin_function_stmt.left,
                                   block.array[i].op.var_assign_builtin_function_stmt.args,
                                   block.array[i].op.var_assign_builtin_function_stmt.function);
                break;
            case var_decl_stmt:
                varDeclare(block.array[i].op.var_decl_stmt.variable);
                break;
            case var_assign_stmt:
                varAssign(block.array[i].op.var_assign_stmt.left,
                          block.array[i].op.var_assign_stmt.expression);
                break;
            case while_stmt:
                generateWhile(block.array[i].op.while_stmt.condition,
                               block.array[i].op.while_stmt.block);
                break;
            case return_stmt:
                generateReturn(block.array[i].op.return_stmt.ret);
                break;
            case print_stmt:
                printStatement(block.array[i].op.print_stmt.expression);
                break;
            default: //TODO remove
                break;
        }
    }
}

/**
 * @copydoc getInput
 */
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

/**
 * @copydoc generateIf
 */
void generateIf(ast_exp *condition, stmtArray ifBlock, struct Stmt *elseStmt) {
    char *ifLabel = getNewLabel();

    if (condition != NULL) {
        generateExp(condition);
        generateCode(ifBlock);
        printf("JUMP %sNE\n", ifLabel);
        printf("LABEL %sN\n", ifLabel);

        if (elseStmt != NULL) {
            generateIf(elseStmt->op.if_stmt.condition, elseStmt->op.if_stmt.ifBlock, elseStmt->op.if_stmt.elseStmt);
        }
        printf("LABEL %sNE\n", ifLabel);
    } else {
        generateCode(ifBlock);
    }
}

/**
 * @copydoc generateFunction
 */
void generateFunction(BinaryTreePtr function, functionArgs *args, stmtArray block) {
    printf("LABEL %%FL%s\n", function->data.name);
    printf("PUSHFRAME\n");
    strcpy(frame, "LF");
    printf("DEFVAR LF@%%retval\n");

    int arg = 0;
    while (args != NULL) {
        printf("DEFVAR LF@%s\n", args->argument->op.variableExp->data.name);
        printf("MOVE LF@%s LF@%%arg%d\n", args->argument->op.variableExp->data.name, arg);
        args = args->next;
        ++arg;
    }

    switch (function->data.type) {
        case TYPE_NUMBER:
            printf("MOVE LF@%%retval int@0\n");
            break;
        case TYPE_DECIMAL:
            printf("MOVE LF@%%retval float@0.0\n");
            break;
        case TYPE_STRING:
            printf("MOVE LF@%%retval string@\n");
            break;
    }

    generateCode(block);

    printf("POPFRAME\n");
    printf("RETURN\n");
    strcpy(frame, "TF");
}

/**
 * @copydoc generateReturn
 */
void generateReturn(ast_exp *expression) {
    int reg = currentRegister;
    generateExp(expression);
    printf("MOVE LF@%%retval %s@%%R%d\n", frame, reg);
    printf("POPFRAME\n");
    printf("RETURN\n");
}

/**
 * @copydoc assignFunction
 */
void assignFunction(functionArgs *args, BinaryTreePtr function, BinaryTreePtr left) {
    printf("CREATEFRAME\n");

    int arg = 0;

    while (args != NULL) {
        int reg = currentRegister;
        generateExp(args->argument);
        printf("DEFVAR TF@%%arg%d\n", arg);
        args = args->next;
        printf("MOVE TF@%%arg%d %s@%%R%d\n", arg, frame, reg);
        printf("CALL %%FL%s\n", function->data.name);
        printf("MOVE %s@%s TF@%%retval\n", getVarFrame(), left->data.name);
        ++arg;
    }
}

/**
 * @copydoc getBuiltinFunction
 */
void getBuiltinFunction(BinaryTreePtr left, functionArgs *args, enum builtin_function function) {
    switch (function) {
        case Length: {
            int reg = currentRegister;
            generateExp(args->argument);
            printf("STRLEN %s@%s %s@%%R%d\n", getVarFrame(), left->data.name, frame, reg);
            break;
        }
        case Chr: {
            int reg = currentRegister;
            generateExp(args->argument);
            printf("INT2CHAR %s@%s %s@%%R%d\n", getVarFrame(), left->data.name, frame, reg);
            break;
        }
        case Asc: {
            int s = currentRegister;
            generateExp(args->argument);
            int i = currentRegister;
            generateExp(args->next->argument);
            printf("STRLEN %s@%s %s@%%R%d\n", getVarFrame(), left->data.name, frame, s);
            char *hReg1 = getHelpRegister();
            printf("DEFVAR %s@%s\n", frame, hReg1);
            printf("GT %s@%s %s@%%R%d %s@%s \n", frame, hReg1, frame, i, getVarFrame(), left->data.name);

            char *ascLabel = getNewLabel();
            printf("JUMPIFNEQ %s %s@%s bool@true\n", ascLabel, frame, hReg1);
            printf("MOVE %s@%s int@0\n", getVarFrame(), left->data.name);
            printf("JUMP %sE\n", ascLabel);

            printf("LABEL %s\n", ascLabel);
            printf("SUB %s@%%R%d %s@%%R%d int@1\n", frame, i, frame, i);
            printf("STRI2INT %s@%s %s@%%R%d %s@%%R%d\n", getVarFrame(), left->data.name, frame, s, frame, i);

            printf("LABEL %sE\n", ascLabel);
            break;
        }
        case SubStr: {
            int s = currentRegister;
            generateExp(args->argument);
            int i = currentRegister;
            generateExp(args->next->argument);
            int n = currentRegister;
            generateExp(args->next->next->argument);

            printf("MOVE %s@%s string@\n", getVarFrame(), left->data.name);
            printf("SUB %s@%%R%d %s@%%R%d int@1\n", frame, i, frame, i);
            printf("SUB %s@%%R%d %s@%%R%d int@1\n", frame, n, frame, n);

            int lenN = currentHelpRegister;
            char *len = getHelpRegister();
            printf("DEFVAR %s@%s\n", frame, len);
            printf("STRLEN %s@%s %s@%%R%d\n", frame, len, frame, s);

            char *substrLabel = getNewLabel();
            printf("JUMPIFEQ %sE %s@%s int@0\n", substrLabel, frame, len);

            char *limit = getHelpRegister();
            printf("DEFVAR %s@%s\n", frame, limit);
            printf("LT %s@%s %s@%%R%d int@0\n", frame, limit, frame, i);
            printf("JUMPIFEQ %sE %s@%s bool@true\n", substrLabel, frame, limit);

            printf("LABEL %s\n", substrLabel);
            strcpy(frame, "TF");
            printf("CREATEFRAME\n");

            char *less = getHelpRegister();
            printf("DEFVAR %s@%s\n", frame, less);

            char *leftC = getHelpRegister();
            printf("DEFVAR %s@%s\n", frame, leftC);

            char *charI = getHelpRegister();
            printf("DEFVAR %s@%s\n", frame, charI);
            printf("GETCHAR %s@%s %s@%%R%d %s@%%R%d\n", frame, charI, getVarFrame(), s, getVarFrame(), i);
            printf("ADD %s@%%R%d %s@%%R%d int@1\n", getVarFrame(), i, getVarFrame(), i);
            printf("CONCAT %s@%s %s@%s %s@%s\n", getVarFrame(), left->data.name, getVarFrame(), left->data.name, frame,
                   charI);

            printf("JUMPIFEQ %sE %s@%%R%d int@0\n", substrLabel, getVarFrame(), n);
            printf("SUB %s@%%R%d %s@%%R%d int@1\n", getVarFrame(), n, getVarFrame(), n);
            printf("LT %s@%s %s@%%R%d %s@%%HR%d\n", frame, less, getVarFrame(), i, getVarFrame(), lenN);
            printf("JUMPIFEQ %s %s@%s bool@true\n", substrLabel, frame, less);
            printf("LABEL %sE\n", substrLabel);

            if (inScope) {
                strcpy(frame, "GF");
            } else {
                strcpy(frame, "LF");
            }
            break;
        }
    }
}

/**
 * @copydoc generateWhile
 */
void generateWhile(ast_exp *condition, stmtArray block) {
    switch (condition->tag_exp) {
        case binaryExp: {
            char *whileLabel = getNewLabel();

            printf("LABEL %%WL%d\n", currentLabel);
            strcpy(frame, "TF");
            printf("CREATEFRAME\n");

            generateExp(condition);
            generateCode(block);
            printf("JUMP %s\n", whileLabel);
            printf("LABEL %sN\n", whileLabel);

            break;
        }
        case bracketExp:
            generateWhile(condition->op.bracketExp.expression, block);
            break;
        default:
            break;
    }
}

/**
 * @copydoc printStatement
 */
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
            generateExp(expression);
            printf("MOVE %s@%s %s@%%R%d\n", frame, reg, frame, nextReg);
            printf("WRITE %s@%s\n", frame, reg);
            break;
        }
        case bracketExp: {
            char *reg = getRegister();
            int nextReg = currentRegister;

            printf("DEFVAR %s@%s\n", frame, reg);
            generateExp(expression->op.bracketExp.expression);
            printf("MOVE %s@%s %s@%%R%d\n", frame, reg, frame, nextReg);
            printf("WRITE %s@%s\n", frame, reg);
            break;
        }
        default: //TODO remove
            break;
    }
}

/**
 * @copydoc varDeclare
 */
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

/**
 * @copydoc varAssign
 */
void varAssign(BinaryTreePtr var, ast_exp *expression) {
    switch (expression->tag_exp) {
        case integerExp:
            if(var->data.type == TYPE_DECIMAL) {
                printf("MOVE %s@%s float@%d\n", getVarFrame(), var->data.name, expression->op.numberExp);
            }
            else {
                printf("MOVE %s@%s int@%d\n", getVarFrame(), var->data.name, expression->op.numberExp);
            }
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
            generateExp(expression);
            printf("MOVE %s@%s %s@%%R%d\n", getVarFrame(), var->data.name, frame, nextReg);
            break;
        }
        case bracketExp: {
            int nextReg = currentRegister;
            generateExp(expression->op.bracketExp.expression);
            printf("MOVE %s@%s %s@%%R%d\n", getVarFrame(), var->data.name, frame, nextReg);
            break;
        }
        default:
            break;
    }
}

/**
 * @copydoc generateExp
 */
void generateExp(ast_exp *expression) {
    char *reg = getRegister();
    printf("DEFVAR %s@%s\n", frame, reg);

    switch (expression->tag_exp) {
        case binaryExp: {
            ast_exp *left = expression->op.binaryExp.left;
            generateExp(expression->op.binaryExp.right);

            switch (left->tag_exp) {
                case integerExp:
                    printf("MOVE %s@%s %s\n", frame, reg, generateIntegerSymbol(left->op.numberExp));
                    if (expression->datatype == exp_decimal) {
                        printf("INT2FLOAT %s@%s %s@%s\n", frame, reg, frame, reg);

                        if (expression->op.binaryExp.right->tag_exp == integerExp) {
                            printf("INT2FLOAT %s@%s %s@%s\n", frame, getNextRegister(reg), frame, getNextRegister(reg));
                        }
                    }

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
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
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
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    }

                    break;
                case variableExp:
                    printf("MOVE %s@%s %s@%s\n", frame, reg, getVarFrame(), left->op.variableExp->data.name);
                    if (expression->datatype == exp_decimal) {
                        if (left->op.variableExp->data.type == TYPE_NUMBER) {
                            printf("INT2FLOAT %s@%s %s@%s\n", frame, reg, frame, reg);
                        }

                        if (expression->op.binaryExp.right->tag_exp == integerExp || (expression->op.binaryExp.right->tag_exp == variableExp && expression->op.binaryExp.right->op.variableExp->data.type == TYPE_NUMBER)) {
                            printf("INT2FLOAT %s@%s %s@%s\n", frame, getNextRegister(reg), frame, getNextRegister(reg));
                        }
                    }

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        if (left->op.variableExp->data.type == TYPE_STRING) {
                            printf("CONCAT %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                        } else {
                            printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                        }
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
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    }

                    break;
                case bracketExp:
                    generateExp(left->op.bracketExp.expression);
                    printf("MOVE %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg));
                    printf("MOVE %s@%s %s@%s\n", frame, getNextRegister(reg), frame,
                           getNextRegister(getNextRegister(reg)));

                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg), frame, reg);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg), frame, reg);
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
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    }

                    break;
                case stringExp:
                    if (strcmp(expression->op.binaryExp.oper.str, "+") == 0) {
                        printf("CONCAT %s@%s %s@%s %s@%s\n", frame, reg, frame, reg, frame, getNextRegister(reg));
                    }
                    break;
                case binaryExp: {
                    int first = currentRegister;
                    generateExp(left->op.binaryExp.left);
                    int second = currentRegister;
                    generateExp(left->op.binaryExp.right);

                    if (strcmp(left->op.binaryExp.oper.str, "+") == 0) {
                        printf("ADD %s@%s %s@%%R%d %s@%%R%d\n", frame, reg, frame, first, frame, second);
                    } else if (strcmp(left->op.binaryExp.oper.str, "-") == 0) {
                        printf("SUB %s@%s %s@%%R%d %s@%%R%d\n", frame, reg, frame, first, frame, second);
                    } else if (strcmp(left->op.binaryExp.oper.str, "*") == 0) {
                        printf("MUL %s@%s %s@%%R%d %s@%%R%d\n", frame, reg, frame, first, frame, second);
                    } else if (strcmp(left->op.binaryExp.oper.str, "/") == 0) {
                        printf("DIV %s@%s %s@%%R%d %s@%%R%d\n", frame, reg, frame, first, frame, second);
                    } else if (strcmp(left->op.binaryExp.oper.str, "\\") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("INT2FLOAT %s@%s %s@%%R%d\n", frame, hReg1, frame, first);
                        printf("INT2FLOAT %s@%s %s@%%R%d\n", frame, hReg2, frame, second);
                        printf("DIV %s@%s %s@%s %s@%s\n", frame, hReg1, frame, hReg1, frame, hReg2);
                        printf("FLOAT2INT %s@%%R%d %s@%s\n", frame, first, frame, hReg1);
                    } else if (strcmp(left->op.binaryExp.oper.str, "<>") == 0) {
                        printf("JUMPIFEQ %%WL%dN %s@%%R%d %s@%%R%d \n", currentLabel, frame, first, frame, second);
                    } else if (strcmp(left->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%%R%d %s@%%R%d \n", currentLabel, frame, first, frame, second);
                    } else if (strcmp(left->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%%R%d %s@%%R%d\n", frame, hReg1, frame, first, frame, second);
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(left->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%%R%d %s@%%R%d\n", frame, hReg1, frame, first, frame, second);
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(left->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%%R%d %s@%%R%d\n", frame, hReg1, frame, first, frame, second);
                        printf("EQ %s@%s %s@%%R%d %s@%%R%d\n", frame, hReg2, frame, first, frame, second);
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(left->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%%R%d %s@%%R%d\n", frame, hReg1, frame, first, frame, second);
                        printf("EQ %s@%s %s@%%R%d %s@%%R%d\n", frame, hReg2, frame, first, frame, second);
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    }

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
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, "=") == 0) {
                        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, reg, frame,
                               getNextRegister(reg));
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<") == 0) {
                        char *hReg1 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
                    } else if (strcmp(expression->op.binaryExp.oper.str, "<=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    } else if (strcmp(expression->op.binaryExp.oper.str, ">=") == 0) {
                        char *hReg1 = getHelpRegister();
                        char *hReg2 = getHelpRegister();
                        printf("DEFVAR %s@%s\n", frame, hReg1);
                        printf("DEFVAR %s@%s\n", frame, hReg2);
                        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, reg, frame, getNextRegister(reg));
                        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, reg, frame, getNextRegister(reg));
                        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
                    }

                    break;
                }
                default:
                    break;
            }

            break;
        }
        case integerExp:
            printf("MOVE %s@%s %s\n", frame, reg, generateIntegerSymbol(expression->op.numberExp));
            break;
        case doubleExp:
            printf("MOVE %s@%s %s\n", frame, reg, generateFloatSymbol(expression->op.decimalExp));
            break;
        case stringExp:
            printf("MOVE %s@%s %s\n", frame, reg, generateSymbol(TYPE_STRING, expression->op.stringExp.str));
            break;
        case variableExp:
            printf("MOVE %s@%s %s@%s\n", frame, reg, getVarFrame(), expression->op.variableExp->data.name);
            break;
        case bracketExp:
            generateExp(expression->op.bracketExp.expression);
            printf("MOVE %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg));
            break;
        default:
            break;
    }
}

/**
 * @copydoc generateSymbol
 */
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

/**
 * @copydoc generateIntegerSymbol
 */
char *generateIntegerSymbol(int value) {
    char *symbolString = (char *) gcmalloc((25) * sizeof(char));

    sprintf(symbolString, "int@%d", value);
    return symbolString;
}

/**
 * @copydoc generateFloatSymbol
 */
char *generateFloatSymbol(double value) {
    char *symbolString = (char *) gcmalloc((80) * sizeof(char));

    sprintf(symbolString, "float@%g", value);
    return symbolString;
}

/**
 * @copydoc v
 */
char *getRegister() {
    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%R%d", currentRegister++);
    return reg;
}

/**
 * @copydoc getNextRegister
 */
char *getNextRegister(char *reg) {
    long val = 0;
    char *p = reg;

    while (*p) {
        if (isdigit(*p)) {
            val = strtol(p, &p, 10);
        } else {
            p++;
        }
    }

    char *nextReg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(nextReg, "%%R%ld", val + 1);
    return nextReg;
}

/**
 * @copydoc getHelpRegister
 */
char *getHelpRegister() {
    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%HR%d", currentHelpRegister++);
    return reg;
}

/**
 * @copydoc v
 */
char *getNewLabel() {
    char *newLabel = (char *) gcmalloc(30 * sizeof(char));
    sprintf(newLabel, "%%WL%d", ++currentLabel);
    return newLabel;
}

/**
 * @copydoc getVarFrame
 */
char *getVarFrame() {
    char *frameTP = (char *) gcmalloc(30 * sizeof(char));
    if (!inScope) {
        strcpy(frameTP, "LF");
    } else {
        strcpy(frameTP, "GF");
    }

    return frameTP;
}