/**
 * @file generator.c
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

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
    currentFunction = NULL;
    inScope = false;

    frame = (char *) gcmalloc(3 * sizeof(char));
    strcpy(frame, "LF");

    printf("JUMP $$main\n");

    generateBuiltInFunctions();

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
 * @copydoc generateBuiltInFunctions
 */
void generateBuiltInFunctions() {
    /** SUBSTR BULTIN FUNCTION **/
    printf("LABEL %%BLS\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%retval\n");
    printf("MOVE LF@%%retval string@\n");

    printf("SUB %s@%%BAi %s@%%BAi int@1\n", frame, frame);
    printf("SUB %s@%%BAn %s@%%BAn int@1\n", frame, frame);

    int lenN = currentHelpRegister;
    char *len = getHelpRegister();
    printf("DEFVAR %s@%s\n", frame, len);
    printf("STRLEN %s@%s %s@%%BAs\n", frame, len, frame);

    char *substrLabel = getNewLabel();
    printf("JUMPIFEQ %sE %s@%s int@0\n", substrLabel, frame, len);

    char *limit = getHelpRegister();
    printf("DEFVAR %s@%s\n", frame, limit);
    printf("LT %s@%s %s@%%BAi int@0\n", frame, limit, frame);
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
    printf("GETCHAR %s@%s %s@%%BAs %s@%%BAi\n", frame, charI, getVarFrame(), getVarFrame());
    printf("ADD %s@%%BAi %s@%%BAi int@1\n", getVarFrame(), getVarFrame());
    printf("CONCAT %s@%%retval %s@%%retval %s@%s\n", getVarFrame(), getVarFrame(), frame, charI);

    printf("JUMPIFEQ %sE %s@%%BAn int@0\n", substrLabel, getVarFrame());
    printf("SUB %s@%%BAn %s@%%BAn int@1\n", getVarFrame(), getVarFrame());
    printf("LT %s@%s %s@%%BAi %s@%%HR%d\n", frame, less, getVarFrame(), getVarFrame(), lenN);
    printf("JUMPIFEQ %s %s@%s bool@true\n", substrLabel, frame, less);
    printf("LABEL %sE\n", substrLabel);

    printf("POPFRAME\n");
    printf("RETURN\n");
    strcpy(frame, "LF");

    /** ASC BULTIN FUNCTION **/
    printf("LABEL %%BLA\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%%retval\n");
    printf("MOVE LF@%%retval int@0\n");

    printf("STRLEN %s@%%retval %s@%%BAs\n", getVarFrame(), getVarFrame());
    char *hReg1 = getHelpRegister();
    char *hReg2 = getHelpRegister();
    printf("DEFVAR %s@%s\n", frame, hReg1);
    printf("DEFVAR %s@%s\n", frame, hReg2);
    printf("GT %s@%s %s@%%BAi %s@%%retval\n", frame, hReg1, getVarFrame(), getVarFrame());
    printf("LT %s@%s %s@%%BAi int@1\n", frame, hReg2, getVarFrame());

    char *ascLabel = getNewLabel();
    printf("JUMPIFEQ %sF %s@%s bool@true\n", ascLabel, frame, hReg1);
    printf("JUMPIFEQ %sF %s@%s bool@true\n", ascLabel, frame, hReg2);

    printf("LABEL %s\n", ascLabel);
    printf("SUB %s@%%BAi %s@%%BAi int@1\n", getVarFrame(), getVarFrame());
    printf("STRI2INT %s@%%retval %s@%%BAs %s@%%BAi\n", getVarFrame(), getVarFrame(), getVarFrame());

    printf("LABEL %sE\n", ascLabel);

    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL %sF\n", ascLabel);
    printf("MOVE %s@%%retval int@0\n", getVarFrame());
    printf("JUMP %sE\n", ascLabel);

    strcpy(frame, "LF");
}

/**
 * @copydoc getInput
 */
void getInput(BinaryTreePtr var) {
    printf("WRITE string@?\\032\n");

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
    currentFunction = function;
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

    char *argS = (char *) gcmalloc(20 * sizeof(char));
    sprintf(argS, "LF@%%retval");
    generateArgumentsConversion(argS, expression->datatype, currentFunction->data.type);

    printf("POPFRAME\n");
    printf("RETURN\n");
}

/**
 * @copydoc assignFunction
 */
void assignFunction(functionArgs *args, BinaryTreePtr function, BinaryTreePtr left) {
    printf("CREATEFRAME\n");

    int arg = 0;
    char *argS = (char *) gcmalloc(20 * sizeof(char));

    while (args != NULL) {
        int reg = currentRegister;
        generateExp(args->argument);
        sprintf(argS, "%s@%%R%d", frame, reg);

        generateArgumentsConversion(argS, args->argument->datatype, function->data.typeOfParams[arg]);

        printf("DEFVAR TF@%%arg%d\n", arg);
        printf("MOVE TF@%%arg%d %s@%%R%d\n", arg, frame, reg);

        args = args->next;
        ++arg;
    }

    printf("CALL %%FL%s\n", function->data.name);

    char *implicit = (char *) gcmalloc(20 * sizeof(char));
    sprintf(implicit, "TF@%%retval");
    generateArgumentsConversion(implicit, function->data.type, left->data.type);

    printf("MOVE %s@%s TF@%%retval\n", getVarFrame(), left->data.name);
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

            char *arg = (char *) gcmalloc(20 * sizeof(char));
            sprintf(arg, "%s@%%R%d", frame, reg);
            generateArgumentsConversion(arg, args->argument->datatype, TYPE_NUMBER);

            printf("INT2CHAR %s@%s %s@%%R%d\n", getVarFrame(), left->data.name, frame, reg);
            break;
        }
        case Asc: {
            printf("CREATEFRAME\n");
            strcpy(frame, "TF");
            int s = currentRegister;
            generateExp(args->argument);
            int i = currentRegister;
            generateExp(args->next->argument);
            printf("DEFVAR %s@%%BAs\n", frame);
            printf("DEFVAR %s@%%BAi\n", frame);
            printf("MOVE %s@%%BAs %s@%%R%d\n", frame, frame, s);
            printf("MOVE %s@%%BAi %s@%%R%d\n", frame, frame, i);

            char *argI = (char *) gcmalloc(20 * sizeof(char));
            sprintf(argI, "%s@%%BAi", frame);
            generateArgumentsConversion(argI, args->next->argument->datatype, TYPE_NUMBER);

            printf("CALL %%BLA\n");
            printf("MOVE %s@%s TF@%%retval\n", getVarFrame(), left->data.name);
            break;
        }
        case SubStr: {
            printf("CREATEFRAME\n");
            strcpy(frame, "TF");
            int s = currentRegister;
            generateExp(args->argument);
            int i = currentRegister;
            generateExp(args->next->argument);
            int n = currentRegister;
            generateExp(args->next->next->argument);
            printf("DEFVAR %s@%%BAs\n", frame);
            printf("DEFVAR %s@%%BAi\n", frame);
            printf("DEFVAR %s@%%BAn\n", frame);
            printf("MOVE %s@%%BAs %s@%%R%d\n", frame, frame, s);
            printf("MOVE %s@%%BAi %s@%%R%d\n", frame, frame, i);
            printf("MOVE %s@%%BAn %s@%%R%d\n", frame, frame, n);

            char *argI = (char *) gcmalloc(20 * sizeof(char));
            sprintf(argI, "%s@%%BAi", frame);
            generateArgumentsConversion(argI, args->next->argument->datatype, TYPE_NUMBER);

            char *argN = (char *) gcmalloc(20 * sizeof(char));
            sprintf(argN, "%s@%%BAn", frame);
            generateArgumentsConversion(argN, args->next->next->argument->datatype, TYPE_NUMBER);

            printf("CALL %%BLS\n");
            printf("MOVE %s@%s TF@%%retval\n", getVarFrame(), left->data.name);
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
        case integerExp: {
            char *reg = getRegister();
            printf("DEFVAR %s@%s\n", frame, reg);
            printf("MOVE %s@%s int@%d\n", frame, reg, expression->op.numberExp);

            char *implicit = (char *) gcmalloc(20 * sizeof(char));
            sprintf(implicit, "%s@%s", frame, reg);
            generateArgumentsConversion(implicit, TYPE_NUMBER, var->data.type);

            printf("MOVE %s@%s %s\n", getVarFrame(), var->data.name, implicit);
            break;
        }
        case doubleExp: {
            char *reg = getRegister();
            printf("DEFVAR %s@%s\n", frame, reg);
            printf("MOVE %s@%s float@%g\n", frame, reg, expression->op.decimalExp);

            char *implicit = (char *) gcmalloc(20 * sizeof(char));
            sprintf(implicit, "%s@%s", frame, reg);
            generateArgumentsConversion(implicit, TYPE_DECIMAL, var->data.type);

            printf("MOVE %s@%s %s\n", getVarFrame(), var->data.name, implicit);
            break;
        }
        case stringExp:
            printf("MOVE %s@%s string@%s\n", getVarFrame(), var->data.name, expression->op.stringExp.str);
            break;
        case variableExp: {
            char *reg = getRegister();
            printf("DEFVAR %s@%s\n", frame, reg);
            printf("MOVE %s@%s %s@%s\n", frame, reg, getVarFrame(), expression->op.variableExp->data.name);

            char *implicit = (char *) gcmalloc(20 * sizeof(char));
            sprintf(implicit, "%s@%s", frame, reg);
            generateArgumentsConversion(implicit, expression->op.variableExp->data.type, var->data.type);

            printf("MOVE %s@%s %s\n", getVarFrame(), var->data.name, implicit);
            break;
        }
        case binaryExp: {
            int nextReg = currentRegister;
            generateExp(expression);

            char *implicit = (char *) gcmalloc(20 * sizeof(char));
            sprintf(implicit, "%s@%%R%d", frame, nextReg);
            generateArgumentsConversion(implicit, expression->datatype, var->data.type);

            printf("MOVE %s@%s %s@%%R%d\n", getVarFrame(), var->data.name, frame, nextReg);
            break;
        }
        case bracketExp: {
            int nextReg = currentRegister;
            generateExp(expression->op.bracketExp.expression);

            char *implicit = (char *) gcmalloc(20 * sizeof(char));
            sprintf(implicit, "%s@%%R%d", frame, nextReg);
            generateArgumentsConversion(implicit, expression->datatype, var->data.type);

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
                    generateOperation(reg, reg, getNextRegister(reg), expression->op.binaryExp.oper.str, left->datatype, expression->datatype);
                    break;
                case doubleExp:
                    printf("MOVE %s@%s %s\n", frame, reg, generateFloatSymbol(left->op.decimalExp));
                    generateOperation(reg, reg, getNextRegister(reg), expression->op.binaryExp.oper.str, left->datatype, expression->datatype);
                    break;
                case stringExp:
                    printf("MOVE %s@%s %s\n", frame, reg, generateSymbol(TYPE_STRING, left->op.stringExp.str));
                    generateOperation(reg, reg, getNextRegister(reg), expression->op.binaryExp.oper.str, left->datatype, expression->datatype);
                    break;
                case variableExp:
                    printf("MOVE %s@%s %s@%s\n", frame, reg, getVarFrame(), left->op.variableExp->data.name);
                    generateOperation(reg, reg, getNextRegister(reg), expression->op.binaryExp.oper.str, left->datatype, left->op.variableExp->data.type);
                    break;
                case bracketExp: {
                    while (left->op.bracketExp.expression->tag_exp == bracketExp) {
                        left = left->op.bracketExp.expression;
                    }

                    int first = currentRegister;
                    generateExp(left->op.bracketExp.expression);
                    int second = currentRegister;

                    printf("MOVE %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg));

                    if (left->op.bracketExp.expression->tag_exp == binaryExp) {
                        printf("MOVE %s@%s %s@%%R%d\n", frame, getNextRegister(reg), frame, first);
                    } else if (left->op.bracketExp.expression->tag_exp == variableExp ||
                               left->op.bracketExp.expression->tag_exp == integerExp ||
                               left->op.bracketExp.expression->tag_exp == doubleExp) {
                        printf("MOVE %s@%s %s@%%R%d\n", frame, getNextRegister(reg), frame, second - 1);
                    } else {
                        printf("MOVE %s@%s %s@%s\n", frame, getNextRegister(reg), frame,
                               getNextRegister(getNextRegister(reg)));
                    }

                    generateOperation(reg, getNextRegister(reg), reg, expression->op.binaryExp.oper.str, left->datatype, expression->datatype);
                    break;
                }
                case binaryExp: {
                    int first = currentRegister;
                    generateExp(left->op.binaryExp.left);
                    int second = currentRegister;
                    generateExp(left->op.binaryExp.right);

                    generateOperation(reg, getRegisterByID(first), getRegisterByID(second), left->op.binaryExp.oper.str, left->datatype, expression->datatype);
                    generateOperation(reg, reg, getNextRegister(reg), expression->op.binaryExp.oper.str, left->datatype, expression->datatype);
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
            while (expression->op.bracketExp.expression->tag_exp == bracketExp) {
                expression = expression->op.bracketExp.expression;
            }
            generateExp(expression->op.bracketExp.expression);
            printf("MOVE %s@%s %s@%s\n", frame, reg, frame, getNextRegister(reg));
            break;
        default:
            break;
    }
}

void generateOperation(char *destination, char *operand1, char *operand2, char *operatorStr, datatype operand1Type, datatype operand2Type) {
    if (operand1Type != TYPE_STRING && operand2Type != TYPE_STRING) {
        generateDataConversion(operand1, operand2, operatorStr);
    }

    if (strcmp(operatorStr, "+") == 0) {
        if (operand1Type == TYPE_STRING || operand2Type == TYPE_STRING) {
            printf("CONCAT %s@%s %s@%s %s@%s\n", frame, destination, frame, operand1, frame, operand2);
        } else {
            printf("ADD %s@%s %s@%s %s@%s\n", frame, destination, frame, operand1, frame, operand2);
        }
    } else if (strcmp(operatorStr, "-") == 0) {
        printf("SUB %s@%s %s@%s %s@%s\n", frame, destination, frame, operand1, frame, operand2);
    } else if (strcmp(operatorStr, "*") == 0) {
        printf("MUL %s@%s %s@%s %s@%s\n", frame, destination, frame, operand1, frame, operand2);
    } else if (strcmp(operatorStr, "/") == 0) {
        printf("DIV %s@%s %s@%s %s@%s\n", frame, destination, frame, operand1, frame, operand2);
    } else if (strcmp(operatorStr, "\\") == 0) {
        char *hReg1 = getHelpRegister();
        char *hReg2 = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("DEFVAR %s@%s\n", frame, hReg2);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg1, frame, operand1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, hReg2, frame, operand2);
        printf("DIV %s@%s %s@%s %s@%s\n", frame, hReg1, frame, hReg1, frame, hReg2);
        printf("FLOAT2INT %s@%s %s@%s\n", frame, operand1, frame, hReg1);
    } else if (strcmp(operatorStr, "<>") == 0) {
        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, operand1, frame,
               operand2);
    } else if (strcmp(operatorStr, "=") == 0) {
        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, operand1, frame,
               operand2);
    } else if (strcmp(operatorStr, ">") == 0) {
        char *hReg1 = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, operand1, frame, operand2);
        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
    } else if (strcmp(operatorStr, "<") == 0) {
        char *hReg1 = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, operand1, frame, operand2);
        printf("JUMPIFNEQ %%WL%dN %s@%s bool@true \n", currentLabel, frame, hReg1);
    } else if (strcmp(operatorStr, "<=") == 0) {
        char *hReg1 = getHelpRegister();
        char *hReg2 = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("DEFVAR %s@%s\n", frame, hReg2);
        printf("LT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, operand1, frame, operand2);
        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, operand1, frame, operand2);
        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
    } else if (strcmp(operatorStr, ">=") == 0) {
        char *hReg1 = getHelpRegister();
        char *hReg2 = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("DEFVAR %s@%s\n", frame, hReg2);
        printf("GT %s@%s %s@%s %s@%s\n", frame, hReg1, frame, operand1, frame, operand2);
        printf("EQ %s@%s %s@%s %s@%s\n", frame, hReg2, frame, operand1, frame, operand2);
        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, hReg1, frame, hReg2);
    }
}

void generateDataConversion(char *operand1, char *operand2, char *operatorStr) {
    if (strcmp(operatorStr, "+") == 0 || strcmp(operatorStr, "-") == 0 || strcmp(operatorStr, "*") == 0 || strcmp(operatorStr, "<") == 0 || strcmp(operatorStr, "<=") == 0 ||
        strcmp(operatorStr, ">") == 0 || strcmp(operatorStr, ">=") == 0 || strcmp(operatorStr, "=") == 0 || strcmp(operatorStr, "<>") == 0) {

        char *hReg1 = getHelpRegister();
        char *hReg2 = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("DEFVAR %s@%s\n", frame, hReg2);

        printf("TYPE %s@%s %s@%s\n", frame, hReg1, frame, operand1);
        printf("TYPE %s@%s %s@%s\n", frame, hReg2, frame, operand2);

        char *conversionLabel = getHelpRegister();
        printf("JUMPIFEQ %s %s@%s %s@%s\n", conversionLabel, frame, hReg1, frame, hReg2);
        printf("JUMPIFEQ %sN %s@%s string@float\n", conversionLabel, frame, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand1, frame, operand1);

        printf("LABEL %sN\n", conversionLabel);
        printf("JUMPIFEQ %s %s@%s string@float\n", conversionLabel, frame, hReg2);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand2, frame, operand2);
        printf("LABEL %s\n", conversionLabel);

    } else if (strcmp(operatorStr, "/") == 0) {
        char *hReg1 = getHelpRegister();
        char *conversionLabel = getHelpRegister();
        printf("DEFVAR %s@%s\n", frame, hReg1);
        printf("TYPE %s@%s %s@%s\n", frame, hReg1, frame, operand1);
        printf("JUMPIFNEQ %s %s@%s string@int\n", conversionLabel, frame, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand1, frame, operand1);
        printf("LABEL %s\n", conversionLabel);

        conversionLabel = getHelpRegister();
        printf("TYPE %s@%s %s@%s\n", frame, hReg1, frame, operand2);
        printf("JUMPIFNEQ %s %s@%s string@int\n", conversionLabel, frame, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand2, frame, operand2);
        printf("LABEL %s\n", conversionLabel);
    }
}

/**
 * @copydoc generateArgumentsConversion
 */
void generateArgumentsConversion(char *arg, datatype argType, datatype destType) {
    if (argType == TYPE_DECIMAL && destType == TYPE_NUMBER) {
        printf("FLOAT2R2EINT %s %s\n", arg, arg);
    } else if (argType == TYPE_NUMBER && destType == TYPE_DECIMAL) {
        printf("INT2FLOAT %s %s\n", arg, arg);
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
 * @copydoc getRegister
 */
char *getRegister() {
    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%R%d", currentRegister++);
    return reg;
}

/**
 * @copydoc getRegister
 */
char *getRegisterByID(int id) {
    char *reg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(reg, "%%R%d", id);
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