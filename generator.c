/**
 * @file generator.c
 * @author Petr Sopf (xsopfp00)
 * @brief Generating IFJCode2017 from AST
 */

#include "generator.h"
#include "parser.h"

/**
 * @copydoc startGenerating
 */
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
        default: //TODO remove
            break;
    }
}

void varAssign(BinaryTreePtr var, ast_exp *expression) {
    printf("%s\n", var->data.name);
    (void) expression;
}