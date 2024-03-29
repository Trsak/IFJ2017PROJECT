/**
 * @project IFJ17
 * @file generator.c
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#include <string.h>
#include <ctype.h>
#include "generator.h"
#include "symtable.h"
#include "parser.h"
#include "optimalization.h"

/**
 * @copydoc startGenerating
 */
void startGenerating() {
    //Set default values
    currentRegister = 0;
    currentHelpRegister = 0;
    currentLabel = 0;
    whileCount = 0;
    inScope = false;
    currentFunction = NULL;

    //Start generation instructions
    printf(".IFJcode17\n");

    //Get help registers
    hReg1 = getHelpRegister();
    hReg2 = getHelpRegister();
    printf("DEFVAR GF@%s\n", hReg1);
    printf("DEFVAR GF@%s\n", hReg2);

    frame = (char *) gcmalloc(3 * sizeof(char));
    strcpy(frame, "LF"); //In functions, we have to use LF

    printf("JUMP $$main\n"); //Jump to main label

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
                strcpy(frame, "GF"); //Use GF in main scope
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
            default:
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
    printf("GT GF@%s %s@%%BAi %s@%%retval\n", hReg1, getVarFrame(), getVarFrame());
    printf("LT GF@%s %s@%%BAi int@1\n", hReg2, getVarFrame());

    char *ascLabel = getNewLabel();
    printf("JUMPIFEQ %sF GF@%s bool@true\n", ascLabel, hReg1);
    printf("JUMPIFEQ %sF GF@%s bool@true\n", ascLabel, hReg2);

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
    if (var->data.isUsed) {

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
void generateFunction(BinaryTreePtr function, stmtArray block) {
    currentFunction = function;
    printf("LABEL %%FL%s\n", function->data.name);
    printf("PUSHFRAME\n");
    strcpy(frame, "LF");
    printf("DEFVAR LF@%%retval\n");

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
 * @copydoc generateWhile
 */
void generateWhile(ast_exp *condition, stmtArray block) {
    switch (condition->tag_exp) {
        case binaryExp: {
            char *whileLabel = getNewLabel();
            ++whileCount;

            printf("LABEL %%WL%d\n", currentLabel);
            strcpy(frame, "TF");
            printf("CREATEFRAME\n");

            generateExp(condition);
            generateCode(block);
            printf("JUMP %s\n", whileLabel);
            printf("LABEL %sN\n", whileLabel);
            --whileCount;

            if (whileCount == 0) {
                strcpy(frame, getVarFrame());
            } else {
                strcpy(frame, "TF");
            }

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
 * @copydoc generateReturn
 */
void generateReturn(ast_exp *expression) {
    int reg = currentRegister;
    generateExp(expression);
    printf("MOVE LF@%%retval %s@%%R%d\n", frame, reg);

    char *argS = (char *) gcmalloc(20 * sizeof(char));
    sprintf(argS, "LF@%%retval");

    generateImplicitConversion(argS, argS, expression->datatype, currentFunction->data.type);

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

    functionArgs *params = getFunctionParams(function->data.name);

    while (args != NULL) {
        int reg = currentRegister;
        generateExp(args->argument);
        sprintf(argS, "%s@%%R%d", frame, reg);

        generateImplicitConversion(argS, argS, args->argument->datatype, function->data.typeOfParams[arg]);

        printf("DEFVAR TF@%s\n", params->argument->op.variableExp->data.name);
        printf("MOVE TF@%s %s@%%R%d\n", params->argument->op.variableExp->data.name, frame, reg);
        args = args->next;
        if (params != NULL) {
            params = params->next;
        }
        ++arg;
    }

    printf("CALL %%FL%s\n", function->data.name);

    if (left->data.isUsed) {
        char *implicit = (char *) gcmalloc(20 * sizeof(char));
        sprintf(implicit, "TF@%%retval");
        generateImplicitConversion(implicit, implicit, function->data.type, left->data.type);

        printf("MOVE %s@%s TF@%%retval\n", getVarFrame(), left->data.name);
    }
}

/**
 * @copydoc getBuiltinFunction
 */
void getBuiltinFunction(BinaryTreePtr left, functionArgs *args, enum builtin_function function) {
    if (left->data.isUsed) {
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
                generateImplicitConversion(arg, arg, args->argument->datatype, TYPE_NUMBER);

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
                generateImplicitConversion(argI, argI, args->next->argument->datatype, TYPE_NUMBER);

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
                generateImplicitConversion(argI, argI, args->next->argument->datatype, TYPE_NUMBER);

                char *argN = (char *) gcmalloc(20 * sizeof(char));
                sprintf(argN, "%s@%%BAn", frame);
                generateImplicitConversion(argN, argN, args->next->argument->datatype, TYPE_NUMBER);

                printf("CALL %%BLS\n");
                printf("MOVE %s@%s TF@%%retval\n", getVarFrame(), left->data.name);
                break;
            }
        }
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
        default:
            break;
    }
}

/**
 * @copydoc varDeclare
 */
void varDeclare(BinaryTreePtr var) {
    if (var->data.isUsed) {
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
}

/**
 * @copydoc varAssign
 */
void varAssign(BinaryTreePtr var, ast_exp *expression) {
    if (var->data.isUsed) {
        char *varSymbol = getVarSymbol(var);

        switch (expression->tag_exp) {
            case integerExp: {
                printf("MOVE %s@%s %s@%d\n", getVarFrame(), var->data.name, varSymbol, expression->op.numberExp);
                break;
            }
            case doubleExp: {
                if (var->data.type == TYPE_NUMBER) {
                    printf("MOVE %s@%s %s@%d\n", getVarFrame(), var->data.name, varSymbol, floatToInt(expression->op.decimalExp));
                } else {
                    printf("MOVE %s@%s %s@%g\n", getVarFrame(), var->data.name, varSymbol, expression->op.decimalExp);
                }

                break;
            }
            case stringExp:
                printf("MOVE %s@%s %s@%s\n", getVarFrame(), var->data.name, varSymbol, expression->op.stringExp.str);
                break;
            case variableExp: {
                if ((expression->op.variableExp->data.type == TYPE_DECIMAL && var->data.type == TYPE_NUMBER) ||
                    (expression->op.variableExp->data.type == TYPE_NUMBER && var->data.type == TYPE_DECIMAL)) {
                    char *hReg = getHelpRegister();
                    printf("DEFVAR %s@%s\n", frame, hReg);
                    generateImplicitConversion(getWholeRegisterName(hReg, frame), getWholeRegisterName(expression->op.variableExp->data.name, getVarFrame()), expression->op.variableExp->data.type,
                                               var->data.type);
                    printf("MOVE %s@%s %s@%s\n", getVarFrame(), var->data.name, frame, hReg);
                } else {
                    printf("MOVE %s@%s %s@%s\n", getVarFrame(), var->data.name, getVarFrame(), expression->op.variableExp->data.name);
                }
                break;
            }
            case binaryExp: {
                int regN = currentRegister;
                generateExp(expression);
                char *reg = getWholeRegisterName(getRegisterByID(regN), frame);

                if ((expression->datatype == 2 && var->data.type == TYPE_NUMBER) ||
                    (expression->datatype == 1 && var->data.type == TYPE_DECIMAL)) {
                    generateImplicitConversion(reg, reg, expression->datatype, var->data.type);
                }

                printf("MOVE %s@%s %s\n", getVarFrame(), var->data.name, reg);
                break;
            }
            case bracketExp: {
                while (expression->op.bracketExp.expression->tag_exp == bracketExp) {
                    expression = expression->op.bracketExp.expression;
                }

                int regN = currentRegister;
                generateExp(expression->op.bracketExp.expression);
                char *reg = getWholeRegisterName(getRegisterByID(regN), frame);

                if ((expression->datatype == 2 && var->data.type == TYPE_NUMBER) ||
                    (expression->datatype == 1 && var->data.type == TYPE_DECIMAL)) {
                    generateImplicitConversion(reg, reg, expression->datatype, var->data.type);
                }

                printf("MOVE %s@%s %s\n", getVarFrame(), var->data.name, reg);
                break;
            }
            default:
                break;
        }
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

            if (strcmp(expression->op.binaryExp.oper.str, "\\") == 0) {
                expression->datatype = TYPE_NUMBER;
            }

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
        char *arg = (char *) gcmalloc(20 * sizeof(char));

        sprintf(arg, "%s@%s", frame, operand1);
        generateArgumentsConversion(arg, TYPE_DECIMAL, TYPE_NUMBER);

        sprintf(arg, "%s@%s", frame, operand2);
        generateArgumentsConversion(arg, TYPE_DECIMAL, TYPE_NUMBER);

        char *conversionLabel = getHelpRegister();

        printf("TYPE GF@%s %s@%s\n", hReg1, frame, operand1);
        printf("JUMPIFNEQ %s GF@%s string@int\n", conversionLabel, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand1, frame, operand1);
        printf("LABEL %s\n", conversionLabel);

        printf("TYPE GF@%s %s@%s\n", hReg1, frame, operand2);
        printf("JUMPIFNEQ %sS GF@%s string@int\n", conversionLabel, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand2, frame, operand2);
        printf("LABEL %sS\n", conversionLabel);

        printf("DIV %s@%s %s@%s %s@%s\n", frame, operand1, frame, operand1, frame, operand2);
        printf("FLOAT2INT %s@%s %s@%s\n", frame, destination, frame, operand1);
    } else if (strcmp(operatorStr, "<>") == 0) {
        printf("JUMPIFEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, operand1, frame,
               operand2);
    } else if (strcmp(operatorStr, "=") == 0) {
        printf("JUMPIFNEQ %%WL%dN %s@%s %s@%s \n", currentLabel, frame, operand1, frame,
               operand2);
    } else if (strcmp(operatorStr, ">") == 0) {
        printf("GT GF@%s %s@%s %s@%s\n", hReg1, frame, operand1, frame, operand2);
        printf("JUMPIFNEQ %%WL%dN GF@%s bool@true \n", currentLabel, hReg1);
    } else if (strcmp(operatorStr, "<") == 0) {
        printf("LT GF@%s %s@%s %s@%s\n", hReg1, frame, operand1, frame, operand2);
        printf("JUMPIFNEQ %%WL%dN GF@%s bool@true \n", currentLabel, hReg1);
    } else if (strcmp(operatorStr, "<=") == 0) {
        printf("LT GF@%s %s@%s %s@%s\n", hReg1, frame, operand1, frame, operand2);
        printf("EQ GF@%s %s@%s %s@%s\n", hReg2, frame, operand1, frame, operand2);
        printf("JUMPIFEQ %%WL%dN GF@%s GF@%s \n", currentLabel, hReg1, hReg2);
    } else if (strcmp(operatorStr, ">=") == 0) {
        printf("GT GF@%s %s@%s %s@%s\n", hReg1, frame, operand1, frame, operand2);
        printf("EQ GF@%s %s@%s %s@%s\n", hReg2, frame, operand1, frame, operand2);
        printf("JUMPIFEQ %%WL%dN GF@%s GF@%s \n", currentLabel, hReg1, hReg2);
    }
}

void generateDataConversion(char *operand1, char *operand2, char *operatorStr) {
    if (strcmp(operatorStr, "+") == 0 || strcmp(operatorStr, "-") == 0 || strcmp(operatorStr, "*") == 0 || strcmp(operatorStr, "<") == 0 || strcmp(operatorStr, "<=") == 0 ||
        strcmp(operatorStr, ">") == 0 || strcmp(operatorStr, ">=") == 0 || strcmp(operatorStr, "=") == 0 || strcmp(operatorStr, "<>") == 0) {

        printf("TYPE GF@%s %s@%s\n", hReg1, frame, operand1);
        printf("TYPE GF@%s %s@%s\n", hReg2, frame, operand2);

        char *conversionLabel = getHelpRegister();
        printf("JUMPIFEQ %s GF@%s GF@%s\n", conversionLabel, hReg1, hReg2);
        printf("JUMPIFEQ %sN GF@%s string@float\n", conversionLabel, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand1, frame, operand1);

        printf("LABEL %sN\n", conversionLabel);
        printf("JUMPIFEQ %s GF@%s string@float\n", conversionLabel, hReg2);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand2, frame, operand2);
        printf("LABEL %s\n", conversionLabel);

    } else if (strcmp(operatorStr, "/") == 0) {
        char *conversionLabel = getHelpRegister();
        printf("TYPE GF@%s %s@%s\n", hReg1, frame, operand1);
        printf("JUMPIFNEQ %s GF@%s string@int\n", conversionLabel, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand1, frame, operand1);
        printf("LABEL %s\n", conversionLabel);

        conversionLabel = getHelpRegister();
        printf("TYPE GF@%s %s@%s\n", hReg1, frame, operand2);
        printf("JUMPIFNEQ %sS GF@%s string@int\n", conversionLabel, hReg1);
        printf("INT2FLOAT %s@%s %s@%s\n", frame, operand2, frame, operand2);
        printf("LABEL %sS\n", conversionLabel);
    }
}

/**
 * @copydoc generateArgumentsConversion
 */
void generateArgumentsConversion(char *arg, datatype argType, datatype destType) {
    if (argType == TYPE_DECIMAL && destType == TYPE_NUMBER) {
        char *conversionLabel = getHelpRegister();
        printf("TYPE GF@%s %s\n", hReg1, arg);
        printf("JUMPIFNEQ %s GF@%s string@float\n", conversionLabel, hReg1);
        printf("FLOAT2R2EINT %s %s\n", arg, arg);
        printf("LABEL %s\n", conversionLabel);
    } else if (argType == TYPE_NUMBER && destType == TYPE_DECIMAL) {
        char *conversionLabel = getHelpRegister();
        printf("TYPE GF@%s %s\n", hReg1, arg);
        printf("JUMPIFNEQ %s GF@%s string@int\n", conversionLabel, hReg1);
        printf("INT2FLOAT %s %s\n", arg, arg);
        printf("LABEL %s\n", conversionLabel);
    }
}

/**
 * @copydoc generateImplicitConversion
 */
bool generateImplicitConversion(char *reg, char *value, datatype type, datatype destType) {
    if (type == TYPE_DECIMAL && destType == TYPE_NUMBER) {
        printf("FLOAT2R2EINT %s %s\n", reg, value);
    } else if (type == TYPE_NUMBER && destType == TYPE_DECIMAL) {
        printf("INT2FLOAT %s %s\n", reg, value);
    } else {
        return false;
    }

    return true;
}

/**
 * @copydoc generateOperationImplicitConversion
 */
bool generateOperationImplicitConversion(char *reg, datatype type, datatype destType) {
    if (type == TYPE_DECIMAL && destType == TYPE_NUMBER) {
        printf("FLOAT2R2EINT %s@%s %s@%s\n", frame, reg, frame, reg);
    } else if (type == TYPE_NUMBER && destType == TYPE_DECIMAL) {
        printf("INT2FLOAT %s@%s %s@%s\n", frame, reg, frame, reg);
    } else {
        return false;
    }

    return true;
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
 * @copydoc getNewLabel
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

/**
 * @copydoc getFunctionParams
 */
functionArgs *getFunctionParams(const char *name) {
    for (int i = 0; i < globalStmtArray.length; i++) {
        if (globalStmtArray.array[i].tag_stmt == function_definition_stmt) {
            if (globalStmtArray.array[i].op.function_definition_stmt.function->data.name == name) {
                return globalStmtArray.array[i].op.function_definition_stmt.args;
            }
        } else if (globalStmtArray.array[i].tag_stmt == scope_stmt) {
            break;
        }
    }

    return NULL;
}

/**
 * @copydoc getVarSymbol
 */
char *getVarSymbol(BinaryTreePtr var) {
    char *varSymbol = (char *) gcmalloc(10 * sizeof(char));

    switch (var->data.type) {
        case TYPE_NUMBER:
            strcpy(varSymbol, "int");
            break;
        case TYPE_DECIMAL:
            strcpy(varSymbol, "float");
            break;
        case TYPE_STRING:
            strcpy(varSymbol, "string");
            break;
        default:
            break;
    }
    return varSymbol;
}

/**
 * @copydoc getWholeRegisterName
 */
char *getWholeRegisterName(const char *reg, char *location) {
    char *fullReg = (char *) gcmalloc(30 * sizeof(char));
    sprintf(fullReg, "%s@%s", location, reg);
    return fullReg;
}