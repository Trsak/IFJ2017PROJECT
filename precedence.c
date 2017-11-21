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


char *getOperator(precedStack symbol) {
	switch (symbol) {
		case PREC_DIVISION:
			return "/";
		case PREC_GREATER:
			return ">";
		case PREC_GREATER_EQUAL:
			return ">=";
		case PREC_LESS:
			return "<";
		case PREC_LESS_EQUAL:
			return "<=";
		case PREC_ASSIGNMENT:
			return "=";
		case PREC_MINUS:
			return "-";
		case PREC_MULTIPLY:
			return "*";
		case PREC_NOT_EQUAL:
			return "<>";
		case PREC_PLUS:
			return "+";
		case PREC_BACKSLASH:
			return "\\";
		default:
			return "";
	}
}

datatype getDatatype(datatype left, datatype right, string oper) {
	if(left == (datatype)exp_string) {
		if(!strcmp(oper.str, "-") || !strcmp(oper.str, "*") || !strcmp(oper.str, "/") || !strcmp(oper.str, "\\")) {
			printErrAndExit(ERROR_TYPE_SEM, "Can't do operation '%s' with string literal!", oper.str);
		}
		if(right != (datatype)exp_string) {
			printErrAndExit(ERROR_TYPE_SEM, "Expression must be string!");
		}
		return exp_string;
	}
	else if(left == (datatype)exp_integer) {
		if(right == (datatype)exp_string) {
			printErrAndExit(ERROR_TYPE_SEM, "Integer and string can't be in one expression!", oper.str);
		}
		if(strcmp(oper.str, "\\")) {
			if(right != (datatype)exp_integer) {
				printErrAndExit(ERROR_TYPE_SEM, "In integer division both operands must be integers!");
			}
			return exp_integer;
		}
		else if(strcmp(oper.str, "/")) {
			return exp_decimal;
		}
		if(right == (datatype)exp_decimal) {
			return exp_decimal;
		}
	}
	else if(left == (datatype)exp_decimal) {
		if(!strcmp(oper.str, "\\")) {
			printErrAndExit(ERROR_TYPE_SEM, "In integer division both operands must be integers!");
		}
		if(right == (datatype)exp_string) {
			printErrAndExit(ERROR_TYPE_SEM, "Double and string can't be in one expression!", oper.str);
		}
		return exp_decimal;
	}
	return exp_integer; // Never should come here.
}

/**
 * @copydoc parseExpression
 */
void parseExpression(token *PreviousToken, ast_exp** expressionTree) {
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

	ast_exp* exp = NULL;
	ast_exp* expTmp;
	BinaryTreePtr node;

    char operation;
    unsigned int PrecTabRow = 0;
    unsigned int PrecTabCol = 0;

    operation = '0';

    stackPush(&stack, NULL, NULL, NULL, PREC_DOLLAR, NULL);
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
                stackPush(&stack, NULL, NULL, NULL, PREC_BRACKET_RIGHT, NULL);
                stack.maxTerm = stack.top;
                Token = getNextToken();

                break;

            case '<':
                stackTop(&stack, &item);

                if (item.symbol == PREC_E) {
                    //If E is on the top, then pop it
					stackItem item;
					stackTop(&stack, &item);
					expTmp = item.Exp;

                    stackPop(&stack);

                    //Then push '<' and E non-term
                    stackPush(&stack, NULL, NULL, NULL, PREC_LT, NULL);
                    stackPush(&stack, NULL, expTmp, NULL, PREC_E, NULL);

                    //Then push terminal (id, ..)
                    stackPush(&stack, NULL, NULL, NULL, PrecTabCol, NULL);
                } else {

                    if (item.symbol == PREC_BRACKET_LEFT && isOperator(PrecTabCol)) {
                        printErrAndExit(ERROR_SYNTAX, "Cannot put operator after '('");
                    }

                    //Here push '<' non-term and terminal like ID or constant or '(' .. etc.
                    stackPush(&stack, NULL, NULL, NULL, PREC_LT, NULL);
					/*if(isOperator(item.symbol)) {
						printf("%s\n", getOperator(item.symbol));
					}
					printf("push: %s\n", Token.value.str);*/
                    stackPush(&stack, NULL, NULL, &Token, PrecTabCol, NULL);
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
/*
				if(isOperator(item.symbol)) {
					printf("%s\n", getOperator(item.symbol));
				}*/

				string oper;

				if(item.symbol == PREC_ID) {
					if(inFunction) {
						node = btGetVariable(symtable, functionName)->data.treeOfFunction;
						//BinaryTreePtr nodeTmp = btGetVariable(node, item.Token.value.str);
						node = btGetVariable(node, item.Token.value.str);
						//node = gcmalloc(sizeof(struct BinaryTree));
						//memcpy(node, nodeTmp, sizeof(struct BinaryTree));
					}
					else {
						node = symtable;
						node = btGetVariable(node, item.Token.value.str);
					}

					if(node != NULL) {
						exp = make_variableExp(node);
					}
					else {
						printErrAndExit(ERROR_PROG_SEM, "Undeclared variable '%s'!", item.Token.value.str);
					}
				}
				else if(item.symbol == PREC_E) {
					//printf("==REDUCTION E<OPER>E->E==\n");
					//printf("add right operand\n");
					exp = make_binaryExp(oper, NULL, exp, exp_integer);
				}
				else if(item.symbol == PREC_NUMBER) {
					//printf("reduction number\n");
					exp = make_numberExp(atoi(item.Token.value.str));
				}
				else if(item.symbol == PREC_DECIMAL_NUMBER) {
					exp = make_decimalExp(strtod(item.Token.value.str, NULL));
				}
				else if(item.symbol == PREC_STRING_EXPRESSION) {
					exp = make_stringExp(item.Token.value);
				}
				else if(item.symbol == PREC_BRACKET_RIGHT) {
					oper.str = ")";
					exp = make_bracketExp(oper, NULL, oper, exp_integer);
				}
                //until it finds '<' in stack
                do {
                    stackPop(&stack);
                    stackTop(&stack, &item);
					if(isOperator(item.symbol)) {
						//printf("add operator\n");
						exp->op.binaryExp.oper.str = getOperator(item.symbol);
					}
					else if(item.symbol == PREC_E) {
						if(exp->tag_exp == binaryExp) {
							//printf("add left operand\n\n");
							exp->op.binaryExp.left = item.Exp;
							exp->datatype = getDatatype(exp->op.binaryExp.left->datatype, exp->op.binaryExp.right->datatype, exp->op.binaryExp.oper);
						}
						else if(exp->tag_exp == bracketExp) {
							exp->op.bracketExp.expression = item.Exp;
							exp->datatype = exp->op.bracketExp.expression->datatype;
						}
					}
					else if(item.symbol == PREC_BRACKET_LEFT) {
						exp->op.bracketExp.leftBracket.str = "(";
					}
                } while (item.symbol != PREC_LT);

                stackPop(&stack);
                stack.maxTerm = stack.top;
                //Replace all by 'E'
                stackPush(&stack, NULL, exp, NULL, PREC_E, NULL);

				/** for 5 * 4 / 3 */
				/*
				if(exp != NULL && exp->op.binaryExp.left != NULL) {
					if(exp->op.binaryExp.left->op.binaryExp.left != NULL) {
						printf("%d", exp->op.binaryExp.left->op.binaryExp.left->op.numberExp);
						printf("%s", exp->op.binaryExp.left->op.binaryExp.oper.str);
						printf("%d", exp->op.binaryExp.left->op.binaryExp.right->op.numberExp);

					}
					printf("%s", exp->op.binaryExp.oper.str);
					printf("%d\n", exp->op.binaryExp.right->op.numberExp);
				}*/

				/** for 5+4*8 */
				/*
				if(exp != NULL && exp->op.binaryExp.left != NULL) {
					printf("%d", exp->op.binaryExp.left->op.numberExp);
					printf("%s", exp->op.binaryExp.oper.str);
					if(exp->op.binaryExp.right->op.binaryExp.left != NULL) {
						printf("%d", exp->op.binaryExp.right->op.binaryExp.left->op.numberExp);
						printf("%s", exp->op.binaryExp.right->op.binaryExp.oper.str);
						printf("%d\n", exp->op.binaryExp.right->op.binaryExp.right->op.numberExp);

					}
				}*/
				/** for (5+4)*8 */
				/*
				if(exp != NULL && exp->op.binaryExp.left != NULL && exp->op.binaryExp.left->tag_exp == bracketExp) {
					printf("%s", exp->op.binaryExp.left->op.bracketExp.leftBracket.str);
					printf("%d", exp->op.binaryExp.left->op.bracketExp.expression->op.binaryExp.left->op.numberExp);
					printf("%s", exp->op.binaryExp.left->op.bracketExp.expression->op.binaryExp.oper.str);
					printf("%d", exp->op.binaryExp.left->op.bracketExp.expression->op.binaryExp.right->op.numberExp);
					printf("%s", exp->op.binaryExp.left->op.bracketExp.rightBracket.str);

					printf("%s", exp->op.binaryExp.oper.str);
					printf("%d\n", exp->op.binaryExp.right->op.numberExp);
				}
				 */
                break;


            case 'B':
                //If there's end of parsing expression
                if (stack.item[stack.maxTerm].symbol == PREC_DOLLAR) {
                    *PreviousToken = Token;
					/*
					if(exp->datatype == exp_integer) {
						printf("exp is INTEGER\n");
					}
					else if(exp->datatype == exp_decimal) {
						printf("exp is DOUBLE\n");
					}
					else {
						printf("exp is STRING\n");
					}
					 */
					*expressionTree = exp;
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

