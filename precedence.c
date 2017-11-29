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
        "<<>>>>>>>>><><<<S>",    //rules for  '\'   2
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
        ">>>>>>>>>>>B>BBBB>",    //rules for  'dbl' 15
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
            symbol == PREC_BACKSLASH || symbol == PREC_PLUS || symbol == PREC_MINUS ||
			symbol == PREC_GREATER || symbol == PREC_GREATER_EQUAL || symbol == PREC_LESS ||
			symbol == PREC_LESS_EQUAL || symbol == PREC_NOT_EQUAL || symbol == PREC_ASSIGNMENT)
        return true;

    return false;
}


/**
 * @copydoc isRelationalOperator
 */
bool isRelationalOperator(precedStack symbol) {
	if (symbol == PREC_GREATER || symbol == PREC_GREATER_EQUAL || symbol == PREC_LESS ||
		symbol == PREC_LESS_EQUAL || symbol == PREC_NOT_EQUAL || symbol == PREC_ASSIGNMENT)
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

datatype getDatatype(datatype left, datatype right, string oper, token Token) {
	if(left == (datatype)exp_string) {
		if(!strcmp(oper.str, "-") || !strcmp(oper.str, "*") || !strcmp(oper.str, "/") || !strcmp(oper.str, "\\")) {
			printErrAndExit(ERROR_TYPE_SEM, "On line %d: Can't do operation '%s' with string literal!", Token.line, oper.str);
		}
		if(right != (datatype)exp_string) {
			printErrAndExit(ERROR_TYPE_SEM, "On line %d: Expression must be string!", Token.line);
		}
		return exp_string;
	}
	else if(left == (datatype)exp_integer) {
		if(right == (datatype)exp_string) {
			printErrAndExit(ERROR_TYPE_SEM, "On line %d: Integer and string can't be in one expression!", Token.line, oper.str);
		}
		if(!strcmp(oper.str, "\\")) {
			return exp_integer;
		}
		else if(!strcmp(oper.str, "/")) {
			return exp_decimal;
		}
		if(right == (datatype)exp_decimal) {
			return exp_decimal;
		}
	}
	else if(left == (datatype)exp_decimal) {
		if(right == (datatype)exp_string) {
			printErrAndExit(ERROR_TYPE_SEM, "On line %d: Double and string can't be in one expression!", Token.line, oper.str);
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
            printErrAndExit(ERROR_SYNTAX, "On line %d: Expression was expected after ','", Token.line);
        }

    } else if ((*PreviousToken).lexem == PRINT || (*PreviousToken).lexem == SEMICOLON) {
		Token = getNextToken();

		if (Token.lexem == SEMICOLON || Token.lexem == EOL_ENUM) {
			printErrAndExit(ERROR_SYNTAX, "On line %d: Expression was expected in 'print' statement", Token.line);
		}

	} else if ((*PreviousToken).lexem == WHILE) {
		Token = getNextToken();

		if (Token.lexem == EOL_ENUM) {
			printErrAndExit(ERROR_SYNTAX, "On line %d: Expression was expected in 'while' statement", Token.line);
		}

	} else if ((*PreviousToken).lexem == IF) {
		Token = getNextToken();

		if (Token.lexem == THEN) {
			printErrAndExit(ERROR_SYNTAX, "On line %d: Expression was expected in 'if' statement", Token.line);
		}

	} else if ((*PreviousToken).lexem == ELSEIF) {
		Token = getNextToken();

		if (Token.lexem == THEN) {
			printErrAndExit(ERROR_SYNTAX, "On line %d: Expression was expected in 'elseif' statement", Token.line);
		}

	} else if ((*PreviousToken).lexem == RETURN) {
		Token = getNextToken();

		if (Token.lexem == EOL_ENUM) {
			printErrAndExit(ERROR_SYNTAX, "On line %d: Expression was expected in 'return' statement", Token.line);
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


	precedStack symbol = getPositionInTable(Token.lexem);
    if (isOperator(symbol) && (symbol != PREC_MINUS && symbol != PREC_PLUS)) {
        printErrAndExit(ERROR_SYNTAX, "On line %d: Expression cannot start with operator except + or -", Token.line);
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

			if (!isRelationalOperator(item.symbol) /*&& item.symbol != PREC_MINUS && item.symbol != PREC_PLUS*/
                && PrecTabCol != PREC_MINUS && PrecTabCol != PREC_PLUS) {
				printErrAndExit(ERROR_SYNTAX, "On line %d: More operations next to each other", Token.line);
			}
        }


        if ((item.symbol == PREC_MINUS || item.symbol == PREC_PLUS) &&
            (PrecTabCol == PREC_MINUS || PrecTabCol == PREC_PLUS)) {

            //change operation to plus
            if ((item.symbol == PREC_MINUS && PrecTabCol == PREC_MINUS)) {

                stackPop(&stack);
                stackPush(&stack, NULL, NULL, NULL, PREC_PLUS, NULL);

            } else if (item.symbol == PREC_PLUS && PrecTabCol == PREC_MINUS) {

                stackPop(&stack);
                stackPush(&stack, NULL, NULL, NULL, PREC_MINUS, NULL);
            }

            Token = getNextToken();
            continue;
        }

        switch (operation) {
            case '=':
                //Rule ( E ) -> E

				stackTop(&stack, &item);

				if (item.symbol == PREC_BRACKET_LEFT) {
					printErrAndExit(ERROR_SYNTAX, "On line %d: Expression in brackets cannot be empty", Token.line);
				}

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

                    if (item.symbol == PREC_BRACKET_LEFT && isOperator(PrecTabCol) &&
							(PrecTabCol != PREC_MINUS && PrecTabCol != PREC_PLUS)) {
                        printErrAndExit(ERROR_SYNTAX, "On line %d: Cannot put operator after '('", Token.line);
                    }


					if ((PrecTabCol == PREC_MINUS || PrecTabCol == PREC_PLUS) &&
							(item.symbol == PREC_BRACKET_LEFT || item.symbol == PREC_DOLLAR || isRelationalOperator(item.symbol))) {

						token ZeroToken;

						if (tokenInit(&ZeroToken) == ERROR_INTERNAL) {
							printErrAndExit(ERROR_INTERNAL, "On line %d: There was a memory error while allocating token structure.", Token.line);
						}

						ZeroToken.lexem = INTEGER;
						strAddChar(&ZeroToken.value, '0');

						//paste 0 before unary operator
						stackPush(&stack, NULL, NULL, NULL, PREC_LT, NULL);
						stackPush(&stack, NULL, NULL, &ZeroToken, PREC_NUMBER, NULL);
						stack.maxTerm = stack.top;

						break;
					} else {
						//Here push '<' non-term and terminal like ID or constant or '(' .. etc.
						stackPush(&stack, NULL, NULL, NULL, PREC_LT, NULL);
						/*if(isOperator(item.symbol)) {
                            printf("%s\n", getOperator(item.symbol));
                        }
                        printf("push: %s\n", Token.value.str);*/
						stackPush(&stack, NULL, NULL, &Token, PrecTabCol, NULL);
						//Now item on top is also a terminal
					}
                }

                //The term is now on top of the stack
                stack.maxTerm = stack.top;
                Token = getNextToken();

                break;


            case '>':
                //TODO - semantics - before the rule is applied, it needs to be checked if the data types of two operands are compatible (string and int)
                //TODO - for example C(integer)  = A (string) + B (integer)

                stackTop(&stack, &item);

				if(isOperator(item.symbol) && PrecTabCol == PREC_DOLLAR) {
					printErrAndExit(ERROR_SYNTAX, "On line %d: Expression ended with operator", Token.line);
				}


				string oper;

				if(item.symbol == PREC_ID) {
					if(inFunction) {
                        if(strcmp(item.Token.value.str, functionName) == 0) {
                            printErrAndExit(ERROR_PROG_SEM, "On line %d: Can't use function '%s' as variable!", Token.line, item.Token.value.str);
                        }
						node = btGetVariable(symtable, functionName)->data.treeOfFunction;
						//BinaryTreePtr nodeTmp = btGetVariable(node, item.Token.value.str);
						node = btGetVariable(node, item.Token.value.str);
						//node = gcmalloc(sizeof(struct BinaryTree));
						//memcpy(node, nodeTmp, sizeof(struct BinaryTree));
					}
					else {
						node = symtable;
						node = btGetVariable(node, item.Token.value.str);
                        if(node && node->data.isFunction) {
                            printErrAndExit(ERROR_PROG_SEM, "On line %d: Can't use function '%s' as variable!", Token.line, item.Token.value.str);
                        }
					}

					if(node != NULL) {
						exp = make_variableExp(node);
					}
					else {
						printErrAndExit(ERROR_PROG_SEM, "On line %d: Undeclared variable '%s'!", Token.line, item.Token.value.str);
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
                        if(isRelationalOperator(item.symbol)) {
                            isRelativeOper = true;
                        }
						exp->op.binaryExp.oper.str = getOperator(item.symbol);
						//printf("operator: %s\n", getOperator(item.symbol));
					}
					else if(item.symbol == PREC_E) {
						if(exp->tag_exp == binaryExp) {
							//printf("add left operand\n\n");
							exp->op.binaryExp.left = item.Exp;
							exp->datatype = getDatatype(exp->op.binaryExp.left->datatype, exp->op.binaryExp.right->datatype, exp->op.binaryExp.oper, Token);
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

                printErrAndExit(ERROR_SYNTAX, "On line %d: Bad expression.", Token.line);
                return ;

            case '-':
                //More ')' then expected .. return it to parser to handle with
                //It could be closing bracket to calling a function
                *PreviousToken = Token;
				*expressionTree = exp;
                return ;


            case 'S':
                printErrAndExit(ERROR_TYPE_SEM, "On line %d: Cannot do this operation with string", Token.line);
                return ;

            case 'I':
                printErrAndExit(ERROR_TYPE_SEM, "On line %d: Cannot do operation '\\' with this data type", Token.line);
                return ;
        }


        //printf("stack:\n");
        //for (int i = 0; i < MAX_VALUE; i++) {
        //    printf("%d,", stack.item[i].symbol);
        //}
        //printf("\n\n");

    }
}

