/**
 * @file precedence.c
 * @author Roman Bartl (xbartl06)
 * @brief Precedence table for syntax analysis of expressions
 */


#include "precedence.h"


//B stands for 'Blank'
const char precedenceTable[MAX_VALUE][MAX_VALUE] = {
        ">>>>>>>>>>><><<<S>",    //rules for  '*'   0
        ">>>>>>>>>>><><<<S>",    //ruses for  '/'   1
        "<<>>>>>>>>><><<IS>",    //rules for  '\'   2
        "<<<>>>>>>>><><<<<>",    //rules for  '+'   3
        "<<<>>>>>>>><><<<S>",    //rules for  '-'   4
        "<<<<<>>>>>><><<<<>",    //rules for  '='   5
        "<<<<<>>>>>><><<<<>",    //rules for  '<>'  6
        "<<<<<>>>>>><><<<<>",    //rules for  '<'   7
        "<<<<<>>>>>><><<<<>",    //rules for  '<='  8
        "<<<<<>>>>>><><<<<>",    //rules for  '=>'  9
        "<<<<<>>>>>><><<<<>",    //rules for  '>'   10
        "<<<<<<<<<<<<=<<<<B",    //rules for  '('   11
        ">>>>>>>>>>>B>BBBB>",    //rules for  ')'   12
        ">>>>>>>>>>>B>BBBB>",    //rules for  'id'  13
        ">>>>>>>>>>>B>BBBB>",    //rules for  'int' 14
        ">>I>>>>>>>>B>BBBB>",    //rules for  'dbl' 15
        "SSS>S>>>>>>B>BBBB>",    //rules for  'str' 16
        "<<<<<<<<<<<<-<<<<B"     //rules for  '$'   17
};


/**
 * @copydoc getPositionInTable
 */
int getPositionInTable(int lexem) {
    switch (lexem) {
        case MULTIPLY:
            return 0;
        case DIVISION:
            return 1;
        case BACKSLASH:
            return 2;
        case PLUS:
            return 3;
        case MINUS:
            return 4;
        case ASSIGNMENT:
            return 5;
        case NOT_EQUAL:
            return 6;
        case LESS:
            return 7;
        case LESS_EQUAL:
            return 8;
        case GREATER:
            return 9;
        case GREATER_EQUAL:
            return 10;
        case BRACKET_LEFT:
            return 11;
        case BRACKET_RIGHT:
            return 12;
        case ID:
            return 13;
        case NUMBER:
            return 14;
        case DECIMAL_NUMBER:
            return 15;
        case STRING_EXPRESSION:
            return 16;
        default:
            return 17;
    }
}


/**
 * @copydoc isOperator
 */
bool isOperator(precedStack symbol) {
    if (symbol == PREC_MULTIPLY || symbol == PREC_DIVISION ||
            symbol == PREC_BACKSLASH || symbol == PREC_PLUS || symbol == PREC_MINUS)
        return true;

    return false;
}


/**
 * @copydoc parseExpression
 */
void parseExpression(token *PreviousToken) {
    token Token = *PreviousToken;

    if ((*PreviousToken).lexem == COMMA) {
        Token = getNextToken();

        if (Token.lexem == BRACKET_RIGHT) {
            printErrAndExit(ERROR_SYNTAX, "Expression was expected after ','");
        }

    } else if (Token.lexem == -1) {
        Token = getNextToken();

    }



    Stack stack;
    stackItem item;
    stackInit(&stack);

    char operation;
    unsigned int PrecTabRow = 0;
    unsigned int PrecTabCol = 0;

    operation = '0';

    stackPush(&stack, NULL, NULL, NULL, PREC_DOLLAR);
    stack.maxTerm++;



    if (isOperator(getPositionInTable(Token.lexem))) {
        printErrAndExit(ERROR_OTHER_SEM, "After '=' cannot follow any operation symbol");
    }


    while (42) {

        stackTopTerminal(&stack, &item);
        PrecTabRow = item.symbol; //The most top terminal
        PrecTabCol = getPositionInTable(Token.lexem);


        operation = precedenceTable[PrecTabRow][PrecTabCol];
        //printf("stack: %d\n", PrecTabRow);
        //printf("znak: %d\n", PrecTabCol);
        //printf("operace: %c\n", operation);


        //if more operators are next to each other, then error
        stackTop(&stack, &item);
        if (isOperator(item.symbol) && isOperator(PrecTabCol)) {
            printErrAndExit(ERROR_SYNTAX, "Wrong expression");
        }


        switch (operation) {
            case '=':
                //Rule ( E ) -> E
                stackPush(&stack, NULL, NULL, NULL, PREC_BRACKET_RIGHT);
                stack.maxTerm = stack.top;
                Token = getNextToken();

                break;

            case '<':
                stackTop(&stack, &item);

                if (item.symbol == PREC_E) {
                    //If E is on the top, then pop it
                    stackPop(&stack);

                    //Then push '<' and E non-term
                    stackPush(&stack, NULL, NULL, NULL, PREC_LT);
                    stackPush(&stack, NULL, NULL, NULL, PREC_E);

                    //Then push terminal (id, ..)
                    stackPush(&stack, NULL, NULL, NULL, PrecTabCol);
                } else {

                    if (item.symbol == PREC_BRACKET_LEFT && isOperator(PrecTabCol)) {
                        printErrAndExit(ERROR_SYNTAX, "Cannot put operator after '('");
                    }

                    //Here push '<' non-term and terminal like ID or constant or '(' .. etc.
                    stackPush(&stack, NULL, NULL, NULL, PREC_LT);
                    stackPush(&stack, NULL, NULL, &Token, PrecTabCol);
                    //Now item on top is also a terminal
                }

                //The term is now on top of the stack
                stack.maxTerm = stack.top;
                Token = getNextToken();

                break;


            case '>':
                //TODO - semantics - before the rule is applied, it needs to be checked if the data types of two operands are compatible (string and int)
                //TODO - for example C(integer)  = A (string) + B (integer)

                stackTop(&stack, &item);

                //until it finds '<' in stack
                do {
                    stackPop(&stack);
                    stackTop(&stack, &item);
                } while (item.symbol != PREC_LT);

                stackPop(&stack);
                stack.maxTerm = stack.top;
                //Replace all by 'E'
                stackPush(&stack, NULL, NULL, NULL, PREC_E);

                break;


            case 'B':
                //If there's end of parsing expression
                if (stack.item[stack.maxTerm].symbol == PREC_DOLLAR) {
                    *PreviousToken = Token;
                    return ;
                }

                printErrAndExit(ERROR_SYNTAX, "Bad expression.");
                return ;

            case '-':
                //More ')' then expected .. return it to parser to handle with
                //It could be closing bracket to calling a function
                *PreviousToken = Token;
                return ;


            case 'S':
                printErrAndExit(ERROR_TYPE_SEM, "Cannot do this operation with string");
                return ;

            case 'I':
                printErrAndExit(ERROR_TYPE_SEM, "Cannot do operation '\\' with this data type");
                return ;
        }


        //printf("stack:\n");
        //for (int i = 0; i < MAX_VALUE; i++) {
        //    printf("%d,", stack.item[i].symbol);
        //}
        //printf("\n\n");

    }
}

