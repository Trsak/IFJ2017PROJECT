/**
 * @file parser.c
 * @author Roman Bartl (xbartl06)
 * @author Tomas Odehnal (xodehn08)
 * @brief Parsing all blocks, statements and expressions
 */


//TODO expressions


#include "parser.h"


void saveFunctionName(string name) {
    functionName = (char *) gcmalloc((name.length+1) * sizeof(char));
    if(!functionName) {
        printErrAndExit(ERROR_INTERNAL, "Malloc error!");
    }
    strcpy(functionName, name.str);
}

/**
 * @copydoc idToken
 */
void IdToken(int lexem) {
    if (lexem != ID) {
        printErrAndExit(ERROR_SYNTAX, "'Identifier' was excepted");
    }
}


/**
 * @copydoc createNode
 */
void createNode(BinaryTreePtr *table, char *name, datatype type, bool declared, bool defined, bool isFunction, BinaryTreePtr *params, datatype *typeOfParams, int paramNumber) {
    BinaryTreePtr node = btGetVariable(*table, name);
    if(node != NULL) {
        node->data.declared = node->data.declared || declared;
        node->data.defined = node->data.defined || defined;
    }
    else {
        Values val = initValues(name);

        val.type = type;
        val.declared = declared;
        val.defined = defined;
        val.isFunction = isFunction;
        val.paramNumber = paramNumber;
        val.typeOfParams = typeOfParams;
        val.treeOfFunction = *params;

        btInsert(table, val);
    }
}


/**
 * @copydoc createParamsNode
 */
void createParamsNode(BinaryTreePtr *params, char *name, datatype type) {
    Values val = initValues(name);

    val.type = type;
    val.declared = true;
    val.defined = false;
    val.isFunction = false;
    val.treeOfFunction = NULL;

    btInsert(params, val);
}

void showAruments(functionArgs* args) {
	functionArgs* arg = args;
	while (arg != NULL) {
        if(arg->argument->tag_exp == variableExp)
		    printf("\t-arg: %s\n", arg->argument->op.variableExp->data.name);
        else if(arg->argument->tag_exp == integerExp)
            printf("\t-arg: %d\n", arg->argument->op.numberExp);
        else if(arg->argument->tag_exp == doubleExp)
            printf("\t-arg: %g\n", arg->argument->op.decimalExp);
        else if(arg->argument->tag_exp == stringExp)
            printf("\t-arg: %s\n", arg->argument->op.stringExp.str);
		arg = arg->next;
	}
}

void printAST(stmtArray globalStmtArray) {
	for(int i = 0; i < globalStmtArray.length; i++) {
		if(globalStmtArray.array[i].tag_stmt == function_definition_stmt) {
			printf("function: %s\n", globalStmtArray.array[i].op.function_definition_stmt.function->data.name);
			showAruments(globalStmtArray.array[i].op.function_definition_stmt.args);
			printAST(globalStmtArray.array[i].op.function_definition_stmt.block);
		}
        else if(globalStmtArray.array[i].tag_stmt == var_assign_function_stmt) {
            printf("%s=", globalStmtArray.array[i].op.var_assign_function_stmt.left->data.name);
            printf("%s()\n", globalStmtArray.array[i].op.var_assign_function_stmt.function->data.name);
            showAruments(globalStmtArray.array[i].op.var_assign_function_stmt.args);
        }
        else if(globalStmtArray.array[i].tag_stmt == var_assign_builtin_function_stmt) {
            printf("%s=", globalStmtArray.array[i].op.var_assign_builtin_function_stmt.left->data.name);
            printf("built-in()\n");
            showAruments(globalStmtArray.array[i].op.var_assign_builtin_function_stmt.args);
        }
        else if(globalStmtArray.array[i].tag_stmt == while_stmt) {
            printf("while:\n");
            // Add condition
            printAST(globalStmtArray.array[i].op.while_stmt.block);
        }
        else if(globalStmtArray.array[i].tag_stmt == optimalization_stmt) {
            printf("optimalization: %d\n", globalStmtArray.array[i].op.optimalization_stmt.nothing);
        }
		else if(globalStmtArray.array[i].tag_stmt == function_decl_stmt) {
			printf("function: %s\n", globalStmtArray.array[i].op.function_decl_stmt.function->data.name);
			showAruments(globalStmtArray.array[i].op.function_decl_stmt.args);
		}
		else if(globalStmtArray.array[i].tag_stmt == var_assign_stmt) {
			printf("%s=", globalStmtArray.array[i].op.var_assign_stmt.left->data.name);
			ast_exp* exp = globalStmtArray.array[i].op.var_assign_stmt.expression;
			if(exp->op.binaryExp.left != NULL && exp->op.binaryExp.left->tag_exp == bracketExp) {
				printf("%s", exp->op.binaryExp.left->op.bracketExp.leftBracket.str);
				printf("%d", exp->op.binaryExp.left->op.bracketExp.expression->op.binaryExp.left->op.numberExp);
				printf("%s", exp->op.binaryExp.left->op.bracketExp.expression->op.binaryExp.oper.str);
				printf("%d", exp->op.binaryExp.left->op.bracketExp.expression->op.binaryExp.right->op.numberExp);
				printf("%s", exp->op.binaryExp.left->op.bracketExp.rightBracket.str);

				printf("%s", exp->op.binaryExp.oper.str);
				printf("%d\n", exp->op.binaryExp.right->op.numberExp);
			}
		}
		else if(globalStmtArray.array[i].tag_stmt == print_stmt) {
			ast_exp* exp = globalStmtArray.array[i].op.print_stmt.expression;
			if(exp->tag_exp == variableExp) {
				printf("print %s;\n", exp->op.variableExp->data.name);
			}
			else if(exp->tag_exp == integerExp) {
				printf("print %d;\n", exp->op.numberExp);
			}
			else if(exp->tag_exp == stringExp) {
				printf("print %s\n", exp->op.stringExp.str);
			}
		}
		else if(globalStmtArray.array[i].tag_stmt == scope_stmt) {
			printf("SCOPE is on\n");
		}
		else if(globalStmtArray.array[i].tag_stmt == var_decl_stmt) {
			printf("Declaration of '%s'\n", globalStmtArray.array[i].op.var_decl_stmt.variable->data.name);
		}
		else if(globalStmtArray.array[i].tag_stmt == input_stmt) {
			printf("INPUT: '%s'\n", globalStmtArray.array[i].op.input_stmt.variable->data.name);
		}
        else if(globalStmtArray.array[i].tag_stmt == return_stmt) {
            //printf("RETURN:\n", globalStmtArray.array[i].op.input_stmt.variable->data.name);
            printf("RETURN: %d\n", globalStmtArray.array[i].op.return_stmt.ret->op.numberExp);
        }
        else if(globalStmtArray.array[i].tag_stmt == if_stmt) {
            printf("IF:\n");
            printAST(globalStmtArray.array[i].op.if_stmt.ifBlock);
            //printAST(globalStmtArray.array[i].op.if_stmt.elseStmt->op.if_stmt.ifBlock);
            //printAST(globalStmtArray.array[i].op.if_stmt.elseStmt->op.if_stmt.elseStmt->op.if_stmt.ifBlock);
            //printAST(globalStmtArray.array[i].op.if_stmt.elseStmt->op.if_stmt.elseStmt->op.if_stmt.elseStmt->op.if_stmt.ifBlock);
            // TODO: print else if and else
        }
		printf("\n");
	}
}

void builtinFunctionsInit() {
    builtinFunctions[0].argsNum = 1;
    builtinFunctions[0].name = "length";
    builtinFunctions[0].types[0] = exp_string;

    builtinFunctions[1].argsNum = 3;
    builtinFunctions[1].name = "substr";
    builtinFunctions[1].types[0] = exp_string;
    builtinFunctions[1].types[1] = exp_integer;
    builtinFunctions[1].types[2] = exp_decimal;

    builtinFunctions[2].argsNum = 2;
    builtinFunctions[2].name = "asc";
    builtinFunctions[2].types[0] = exp_string;
    builtinFunctions[2].types[1] = exp_integer;

    builtinFunctions[3].argsNum = 1;
    builtinFunctions[3].name = "chr";
    builtinFunctions[3].types[0] = exp_integer;
}

/**
 * @copydoc program
 */
void program() {
	stackInit(&stmtStack);

	stmtArrayInit(&globalStmtArray);
    addStmtToArray(&globalStmtArray, make_optimalizationStmt());

    builtinFunctionsInit();

    token Token = getNextToken();

    if (Token.lexem == EOL_ENUM) {
        Token = getNextToken();
    }

    if (functionFirst(Token.lexem)) {
        PreviousToken = Token;

        functions();

    } else {
        PreviousToken = Token;
    }

    inFunction = false;

    mainBody();

	//printAST(globalStmtArray);

}


/**
 * @copydoc functionFirst
 */
bool functionFirst(int lexem) {
    if (lexem == DECLARE || lexem == FUNCTION) {
        return true;
    }

    return false;
}


/**
 * @copydoc functions
 */
void functions() {
    token Token = PreviousToken;

    inFunction = true;

	stackItem item;
    if (Token.lexem != DECLARE) {

		stmtArray functionStmtBlock;
		stmtArrayInit(&functionStmtBlock);

		stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, make_functionDefStmt(NULL, NULL, functionStmtBlock));

        functionHeader(false, true);

        statement();

		stackTop(&stmtStack, &item);

		addStmtToArray(&globalStmtArray, item.stmt);
		stackPop(&stmtStack);

        functionEnd();

    } else {
		stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, make_functionDeclStmt(NULL, NULL));

        functionHeader(true, false);

		stackTop(&stmtStack, &item);

		addStmtToArray(&globalStmtArray, item.stmt);
		stackPop(&stmtStack);
    }

    functionNext();
}


/**
 * @copydoc functionHeader
 */
void functionHeader(bool isDeclared, bool isDefined) {
    token Token = PreviousToken;

    if (Token.lexem != FUNCTION) {
        Token = getNextToken();

        if (Token.lexem != FUNCTION) {
            printErrAndExit(ERROR_SYNTAX, "'Function' was expected");
        }
    }

    Token = getNextToken();

    IdToken(Token.lexem);

    if (strcmp(Token.value.str, "length") == 0 || strcmp(Token.value.str, "substr") == 0 ||
        strcmp(Token.value.str, "asc") == 0 || strcmp(Token.value.str, "chr") == 0) {

        printErrAndExit(ERROR_SYNTAX, "Redefined built in function");
    }

    char *name = Token.value.str;
    saveFunctionName(Token.value);

    /** Semantics: Check if identifier is already in symtable and it's declaration|definition */
    BinaryTreePtr node = btGetVariable(symtable, name);
	stackItem item;
    if(node != NULL) {
        if(node->data.declared && isDeclared) {
            printErrAndExit(ERROR_PROG_SEM, "Function '%s' already declared!", name);
        }
        else if(node->data.defined && isDefined) {
            printErrAndExit(ERROR_PROG_SEM, "Function '%s' already defined!", name);
        }
        else if(node->data.defined && isDeclared) {
            printErrAndExit(ERROR_PROG_SEM, "Can't declare already defined function '%s'!", name);
        }

		/** Add function statement to stack */
		stackTop(&stmtStack, &item);
		item.stmt->op.function_definition_stmt.function = node;
    }

    Token = getNextToken();

    if (Token.lexem != BRACKET_LEFT) {
        printErrAndExit(ERROR_SYNTAX, "'(' was expected");
    }

    /** Semantics: check for valid function parameters */
    BinaryTreePtr params = NULL;
    datatype *typeOfParams = NULL;
    int paramNumber = 0;

    if(node != NULL) {
        params = node->data.treeOfFunction;
        typeOfParams = node->data.typeOfParams;
        declareParams(node, &params, &typeOfParams, &paramNumber);

        if(paramNumber != node->data.paramNumber) {
            printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d arguments and less were given!", node->data.name, node->data.paramNumber);
        }
    }
    else {
        declareParams(NULL, &params, &typeOfParams, &paramNumber);
    }

    Token = PreviousToken;

    if (Token.lexem != BRACKET_RIGHT) {
        printErrAndExit(ERROR_SYNTAX, "')' was expected");
    }

    datatype type;
    asDataType(&type);

    Token = getNextToken();

    eol(Token.lexem);


    //Creates new node of function in symtable
    createNode(&symtable, name, type, isDeclared, isDefined, true, &params, typeOfParams, paramNumber);

	node = btGetVariable(symtable, name);
	if(isDeclared) {
		/** Add function statement to stack (declaration) */
		stackTop(&stmtStack, &item);
		item.stmt->op.function_decl_stmt.function = node;
	}
	else if(isDefined && !node->data.declared) {
		// Add function statement to stack (definition) without declaration
		stackTop(&stmtStack, &item);
		item.stmt->op.function_definition_stmt.function = node;
	}
}


/**
 * @copydoc asDataType
 */
void asDataType(datatype *type) {
    token Token = getNextToken();

    if (Token.lexem != AS) {
        printErrAndExit(ERROR_SYNTAX, "'As' was expected");
    }

    dataType(type);
}


/**
 * @copydoc functionNext
 */
void functionNext() {
    token Token = getNextToken();

    if (!functionFirst(Token.lexem)) {
        PreviousToken = Token;
        return;
    }

    PreviousToken = Token;

    functions();
}


/**
 * @copydoc functionEnd
 */
void functionEnd() {
    token Token = PreviousToken;

    if (PreviousToken.lexem != END) {
        Token = getNextToken();

        end(Token.lexem);
    }

    Token = getNextToken();

    if (Token.lexem != FUNCTION) {
        printErrAndExit(ERROR_SYNTAX, "'Function' was expected");
    }

    Token = getNextToken();

    gcfree(functionName);

    eol(Token.lexem);
}


/**
 * @copydoc declareParams
 */
void declareParams(BinaryTreePtr node, BinaryTreePtr *params, datatype **typeOfParams, int *paramNumber) {
    static int currentArgumentNum = -1;
    currentArgumentNum++;
    token Token = getNextToken();

    if (Token.lexem != ID) {
        if (PreviousToken.lexem == COMMA) {
            printErrAndExit(ERROR_SYNTAX, "'Identifier' was expected'");
        }

        PreviousToken = Token;
        return;
    }

    char *name = Token.value.str;


    datatype type;
    asDataType(&type);

	stackItem item;

	if(node != NULL) {
		if(currentArgumentNum >= node->data.paramNumber) {
			printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and more were given!", node->data.name, node->data.paramNumber);
		}
        else if(!btGetVariable(node->data.treeOfFunction, name)) {
			printErrAndExit(ERROR_TYPE_SEM, "In function '%s' %d. parameter has unexpected identifier '%s'!", node->data.name, currentArgumentNum+1, name);
		}
        else if(type != node->data.typeOfParams[currentArgumentNum]) {
			printErrAndExit(ERROR_TYPE_SEM, "In function '%s' %d. parameter has bad datatype!", node->data.name, currentArgumentNum+1);
		}

		/** Add argument to function statement on top of stack */
		stackTop(&stmtStack, &item);
		addArgumentToArray(&item.stmt->op.function_definition_stmt.args, make_variableExp(btGetVariable(node->data.treeOfFunction, name)));

		/*
		stackItem it;
		stackTop(&stmtStack, &it);
		printf("function: %s\n", it.stmt->op.function_definition_stmt.function->data.name);
		if(item.stmt->op.function_definition_stmt.args == NULL) {
			printf("nefunguje\n");
		}
		else {
			functionArgs *arg = item.stmt->op.function_definition_stmt.args;
			while (arg != NULL) {
				printf("--arg: %s\n", arg->argument->op.variableExp->data.name);
				arg = arg->next;
			}
		}*/
	}
    else {
        if(*typeOfParams == NULL) {
            *typeOfParams = gcmalloc(sizeof(datatype));
        }
        else {
            *typeOfParams = gcrealloc(*typeOfParams, (currentArgumentNum+1) * sizeof(datatype));
        }
        (*typeOfParams)[currentArgumentNum] = type;
        //Creates new node in tree of parameters
        createParamsNode(params, name, type);

		stackTop(&stmtStack, &item);
		addArgumentToArray(&item.stmt->op.function_definition_stmt.args, make_variableExp(btGetVariable(*params, name)));
    }

    *paramNumber = currentArgumentNum + 1;

    declareParamsNext(node, params, typeOfParams, paramNumber);
    currentArgumentNum = -1;
    return;
}


/**
 * @copydoc declareParamsNext
 */
void declareParamsNext(BinaryTreePtr node, BinaryTreePtr *params, datatype **typeOfParams, int *paramNumber) {
    token Token = getNextToken();

    if (Token.lexem != COMMA) {
        PreviousToken = Token;
        return;
    }

    PreviousToken = Token;

    declareParams(node, params, typeOfParams, paramNumber);
    return;
}

char* getTypeString(datatype type) {
	if(type == (datatype)exp_integer)
		return "integer";
	else if(type == (datatype)exp_decimal)
		return "double";
	else
		return "string";
}


/**
 * @copydoc dataType
 */
void dataType(datatype *type) {
    token Token = getNextToken();

    if (Token.lexem != INTEGER && Token.lexem != DOUBLE && Token.lexem != STRING) {
        printErrAndExit(ERROR_SYNTAX, "Data type was expected");
    }

    datatype convType = TYPE_NUMBER;

    switch (Token.lexem) {
        case INTEGER:
            convType = TYPE_NUMBER;
            break;

        case DOUBLE:
            convType = TYPE_DECIMAL;
            break;

        case STRING:
            convType = TYPE_STRING;
            break;
        default:
            break; //This will never happen, but compiler would return lots of warnings
    }

    *type = convType;
}


/**
 * @copydoc statement
 */
void statement() {
    token Token = getNextToken();
    char *name = "";

	BinaryTreePtr node;
	ast_exp* expressionTree = NULL;
    int pushCounter = 0;

    switch (Token.lexem) {
        case ID:

            name = Token.value.str;

			/** Semantics: Check if variable was declared */
			if(inFunction) {
				BinaryTreePtr node1;
				node1 = btGetVariable(symtable, name);
				if(node1 && node1->data.isFunction) {
					printErrAndExit(ERROR_OTHER_SEM, "Can't do assignment to function '%s'!", name);
				}
				node1 = btGetVariable(symtable, functionName)->data.treeOfFunction;
				node = btGetVariable(node1, name);
				if(node == NULL || !node->data.declared) {
					printErrAndExit(ERROR_PROG_SEM, "Undeclared variable '%s'!", name);
				}
			}
			else {
				node = btGetVariable(symtable, name);
				if(node && node->data.isFunction) {
					printErrAndExit(ERROR_OTHER_SEM, "Can't do assignment to function '%s'!", name);
				}
				if(node == NULL || !node->data.declared) {
					printErrAndExit(ERROR_PROG_SEM, "Undeclared variable '%s'!", name);
				}
			}

            assignment(false, name);

            break;

        case DIM:
            Token = getNextToken();

            IdToken(Token.lexem);

            name = Token.value.str;

            datatype type;
            asDataType(&type);

			// TODO: check if id == built-in function
            /** Semantics: Check if variable was already declared */
            if(inFunction) {
                BinaryTreePtr node1;
                node1 = btGetVariable(symtable, functionName)->data.treeOfFunction;
                node = btGetVariable(node1, name);
                if(strcmp(name, functionName) == 0) {
                    printErrAndExit(ERROR_PROG_SEM, "Can't declare variable, already exists function '%s'!", name);
                }
                if (node && node->data.declared) {
                    printErrAndExit(ERROR_PROG_SEM, "Variable '%s' already declared!", name);
                }

                //Create new node - declaration of variable
                BinaryTreePtr params = NULL;
                createNode(&node1, name, type, true, false, false, &params, NULL, 0); // Add new arguments
				node = btGetVariable(node1, name);
            }
            else {
                node = btGetVariable(symtable, name);
				if(node && node->data.isFunction) {
					printErrAndExit(ERROR_PROG_SEM, "Function '%s' already declared!", name);
				}
                if (node && node->data.declared) {
                    printErrAndExit(ERROR_PROG_SEM, "Variable '%s' already declared!", name);
                }

                //Create new node - declaration of variable
                BinaryTreePtr params = NULL;
                createNode(&symtable, name, type, true, false, false, &params, NULL, 0); // Add new arguments
				node = btGetVariable(symtable, name);
            }

			ast_stmt* var_declStmt = make_varDeclStmt(node);

			if(!stackEmpty(&stmtStack)) {
				stackItem item;
				stackTop(&stmtStack, &item);
				if(item.stmt->tag_stmt == function_definition_stmt) {
					addStmtToArray(&item.stmt->op.function_definition_stmt.block, var_declStmt);
				}
				else {
					printErrAndExit(ERROR_SYNTAX, "Can't declare variable '%s' here!", node->data.name);
				}
			}
			else {
				addStmtToArray(&globalStmtArray, var_declStmt);
			}


			assignment(true, name);

			/** Add to variable definition to true */
			if(inFunction) {
				node = btGetVariable(symtable, functionName)->data.treeOfFunction;
				node = btGetVariable(node, name);
			}
            else {
				node = btGetVariable(symtable, name);
			}

            break;

        case INPUT:
            Token = getNextToken();
            IdToken(Token.lexem);

			name = Token.value.str;

			// TODO: check if id == built-in function
			if(inFunction) {
				BinaryTreePtr node1;
				node = btGetVariable(symtable, name);
				if(node && node->data.isFunction) {
					printErrAndExit(ERROR_PROG_SEM, "Can't assign to function '%s'!", name);
				}
				node1 = btGetVariable(symtable, functionName)->data.treeOfFunction;
				node = btGetVariable(node1, name);
				if (!node) {
					printErrAndExit(ERROR_PROG_SEM, "Can't assign into undeclared variable '%s'!", name);
				}
			}
			else {
				node = btGetVariable(symtable, name);
				if(node && node->data.isFunction) {
					printErrAndExit(ERROR_OTHER_SEM, "Can't assign to function '%s'!", name);
				}
				if (!node) {
					printErrAndExit(ERROR_PROG_SEM, "Can't assign into undeclared variable '%s'!", name);
				}
			}

			ast_stmt* inputStmt = make_inputStmt(node);

			if(!stackEmpty(&stmtStack)) {
				stackItem item;
				stackTop(&stmtStack, &item);
				if(item.stmt->tag_stmt == function_definition_stmt) {
					addStmtToArray(&item.stmt->op.function_definition_stmt.block, inputStmt);
				}
				else if(item.stmt->tag_stmt == while_stmt) {
					addStmtToArray(&item.stmt->op.while_stmt.block, inputStmt);
				}
				else if(item.stmt->tag_stmt == if_stmt) {
					addStmtToArray(&item.stmt->op.if_stmt.ifBlock, inputStmt);
				}
			}
			else {
				addStmtToArray(&globalStmtArray, inputStmt);
			}

            break;

        case PRINT:
			parseExpression(&Token, &expressionTree);

			ast_stmt* printStmt = make_printStmt(expressionTree);

			stackItem item;
			if(!stackEmpty(&stmtStack)) {
				stackTop(&stmtStack, &item);
				if(item.stmt->tag_stmt == function_definition_stmt) {
					addStmtToArray(&item.stmt->op.function_definition_stmt.block, printStmt);
				}
				else if(item.stmt->tag_stmt == while_stmt) {
					addStmtToArray(&item.stmt->op.while_stmt.block, printStmt);
                }
				else if(item.stmt->tag_stmt == if_stmt) {
					addStmtToArray(&item.stmt->op.if_stmt.ifBlock, printStmt);
				}
			}
			else {
				addStmtToArray(&globalStmtArray, printStmt);
			}

            if (Token.lexem != SEMICOLON) {
                printErrAndExit(ERROR_SYNTAX, "';' was expected");
            }

            printNext();

            break;

        case DO:
            Token = getNextToken();

            if (Token.lexem != WHILE) {
                printErrAndExit(ERROR_SYNTAX, "'While' was expected");
            }

            parseExpression(&Token, &expressionTree);

            stmtArray whileStmtBlock;
            stmtArrayInit(&whileStmtBlock);

            ast_stmt* whileStmt = make_whileStmt(expressionTree, whileStmtBlock);

            stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, whileStmt);

            eol(Token.lexem);

            //Reset prev. token
            PreviousToken.lexem = -1;


            statement();

            Token = PreviousToken;

            if (Token.lexem != LOOP) {
                Token = getNextToken();
                if (Token.lexem != LOOP) {
                    printErrAndExit(ERROR_SYNTAX, "'Loop' was expected");
                }
            }

            stackTop(&stmtStack, &item);

            stackPop(&stmtStack);

            if(!stackEmpty(&stmtStack)) {
                stackTop(&stmtStack, &item);
                if(item.stmt->tag_stmt == function_definition_stmt) {
                    addStmtToArray(&item.stmt->op.function_definition_stmt.block, whileStmt);
                }
                else if(item.stmt->tag_stmt == while_stmt) {
                    addStmtToArray(&item.stmt->op.while_stmt.block, whileStmt);
                }
                else if(item.stmt->tag_stmt == if_stmt) {
                    addStmtToArray(&item.stmt->op.if_stmt.ifBlock, whileStmt);
                }
            }
            else {
                addStmtToArray(&globalStmtArray, whileStmt);
            }
            break;

        case IF:
            parseExpression(&Token, &expressionTree);

            stmtArray ifBlock;
            stmtArrayInit(&ifBlock);

            ast_stmt* ifStmt= make_ifStmt(expressionTree, ifBlock, NULL);

            stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, ifStmt);

            if (Token.lexem != THEN) {
                printErrAndExit(ERROR_SYNTAX, "'Then' was expected");
            }

            //Reset prev. token
            PreviousToken.lexem = -1;

            Token = getNextToken();

            eol(Token.lexem);

            statement();

            ifNext(&pushCounter);

            while(pushCounter != 0) {
                stackPop(&stmtStack);
                pushCounter--;
            }

            stackTop(&stmtStack, &item);

            stackPop(&stmtStack);

            if(!stackEmpty(&stmtStack)) {
                stackTop(&stmtStack, &item);
                if(item.stmt->tag_stmt == function_definition_stmt) {
                    addStmtToArray(&item.stmt->op.function_definition_stmt.block, ifStmt);
                }
                else if(item.stmt->tag_stmt == while_stmt) {
                    addStmtToArray(&item.stmt->op.while_stmt.block, ifStmt);
                }
                else if(item.stmt->tag_stmt == if_stmt) {
                    addStmtToArray(&item.stmt->op.if_stmt.ifBlock, ifStmt);
                }
            }
            else {
                addStmtToArray(&globalStmtArray, ifStmt);
            }

            Token = PreviousToken;

            if (Token.lexem != END) {
                Token = getNextToken();

                end(Token.lexem);
            }

            Token = getNextToken();

            if (Token.lexem != IF) {
                printErrAndExit(ERROR_SYNTAX, "'If' was expected");
            }

            break;

        case RETURN:
            if (!inFunction) {
                printErrAndExit(ERROR_SYNTAX, "'Return' statement not in function!");
            }

            parseExpression(&Token, &expressionTree);
            PreviousToken = Token;

            node = btGetVariable(symtable, functionName);
            if(node->data.type == (datatype)exp_integer || node->data.type == (datatype)exp_decimal) {
                if(expressionTree->datatype == exp_string) {
                    printErrAndExit(ERROR_TYPE_SEM, "Function should return '%s', but return '%s'!", getTypeString(node->data.type), getTypeString(expressionTree->datatype));
                }
            }
            else if(node->data.type == (datatype)exp_string && expressionTree->datatype != exp_string) {
                printErrAndExit(ERROR_TYPE_SEM, "Function should return '%s', but return '%s'!", getTypeString(node->data.type), getTypeString(expressionTree->datatype));
            }

            ast_stmt* returnStmt = make_returnStmt(expressionTree);
            if(!stackEmpty(&stmtStack)) {
                stackTop(&stmtStack, &item);
                if(item.stmt->tag_stmt == function_definition_stmt) {
                    // TODO: what datatypes can return?
                    int i = stmtStack.top;
                    while(item.stmt->tag_stmt != function_definition_stmt) {
                        i = i - 1;
                        item = stmtStack.item[i];
                    }
                    addStmtToArray(&item.stmt->op.function_definition_stmt.block, returnStmt);
                }
                else {
                    printf("Never should come here!\n");
                }
            }
            else {
                addStmtToArray(&globalStmtArray, returnStmt);
            }

            break;

        default:
            PreviousToken = Token;
            return;
    }

    Token = PreviousToken;

    if (Token.lexem != EOL_ENUM) {
        if(isExpression) {
            printErrAndExit(ERROR_SYNTAX, "'End-Of-Line' was expected");
        }

        Token = getNextToken();

        eol(Token.lexem);

    } else {
        //Resetting token
        PreviousToken.lexem = -1;
    }

    isExpression = false;

    statement();
}


/**
 * @copydoc printNext
 */
void printNext() {
	ast_exp* expressionTree;

    token Token = getNextToken();

    if (Token.lexem == EOL_ENUM) {
        PreviousToken = Token;
        return;
    }

    parseExpression(&Token, &expressionTree);

	ast_stmt* print_stmt = make_printStmt(expressionTree);

	stackItem item;
	if(!stackEmpty(&stmtStack)) {
		stackTop(&stmtStack, &item);
		if(item.stmt->tag_stmt == function_definition_stmt) {
			addStmtToArray(&item.stmt->op.function_definition_stmt.block, print_stmt);
		}
		else if(item.stmt->tag_stmt == while_stmt) {
			addStmtToArray(&item.stmt->op.while_stmt.block, print_stmt);
		}
		else if(item.stmt->tag_stmt == if_stmt) {
			addStmtToArray(&item.stmt->op.if_stmt.ifBlock, print_stmt);
		}
	}
	else {
		addStmtToArray(&globalStmtArray, print_stmt);
	}

    if (Token.lexem != SEMICOLON) {
        printErrAndExit(ERROR_SYNTAX, "';' was expected");
    }

    printNext();
}


/**
 * @copydoc ifNext
 */
void ifNext(int* pushCounter) {
    token Token = PreviousToken;

    if (Token.lexem != ELSE && Token.lexem != ELSEIF) {
        return;
    }

    if (Token.lexem == ELSEIF) {
        elseIf(pushCounter);
        Token = PreviousToken;
    }


    if (Token.lexem == ELSE) {
        PreviousToken.lexem = -1;
        Token = getNextToken();

        eol(Token.lexem);

        stmtArray ifBlock;
        stmtArrayInit(&ifBlock);

        ast_stmt* ifStmt = make_ifStmt(NULL, ifBlock, NULL);

        stackItem item;
        stackTop(&stmtStack, &item);

        item.stmt->op.if_stmt.elseStmt = ifStmt;

        (*pushCounter)++;
        stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, ifStmt);

        statement();

        while(*pushCounter != 0) {
            stackPop(&stmtStack);
            (*pushCounter)--;
        }
    }
}


/**
 * @copydoc elseIf
 */
void elseIf(int* pushCounter) {
	ast_exp* expressionTree;
    token Token = PreviousToken;

    parseExpression(&Token, &expressionTree);

    stmtArray ifBlock;
    stmtArrayInit(&ifBlock);

    ast_stmt* ifStmt = make_ifStmt(expressionTree, ifBlock, NULL);

    stackItem item;
    stackTop(&stmtStack, &item);

    item.stmt->op.if_stmt.elseStmt = ifStmt;

    (*pushCounter)++;
    stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, ifStmt);

    if (Token.lexem != THEN) {
        printErrAndExit(ERROR_SYNTAX, "'Then' was expected");
    }

    PreviousToken.lexem = -1;

    Token = getNextToken();

    eol(Token.lexem);

    statement();

    Token = PreviousToken;

    if (Token.lexem == ELSEIF) {
        elseIf(pushCounter);
    } else {
        PreviousToken = Token;
    }
}


/**
 * @copydoc end
 */
void end(int lexem) {
    if (lexem != END) {
        printErrAndExit(ERROR_SYNTAX, "'End' was expected");
    }
}


/**
 * @copydoc eol
 */
void eol(int lexem) {
    if (lexem != EOL_ENUM) {
        printErrAndExit(ERROR_SYNTAX, "'End-Of-Line' was expected");
    }
}


/**
 * @copydoc assignment
 */
void assignment(bool isDeclaration, char *name) {
    token Token = getNextToken();


    if (Token.lexem != ASSIGNMENT && !unaryOperation(Token)) {
        if(!isDeclaration) {
            printErrAndExit(ERROR_SYNTAX, "'Identifier' cannot stand alone");
        }

        PreviousToken = Token;
        return;
    }

    if (unaryOperation(Token) && isDeclaration) {
        printErrAndExit(ERROR_SYNTAX, "Cannot do unary operation in declaration statement");
    }


    Token = getNextToken();

    if (Token.lexem == EOL_ENUM) {
        //TODO - is this syntax or semantics??
        printErrAndExit(ERROR_SYNTAX, "Assignment withnout expression");
    }

    if(Token.lexem == ID) {
        bool function = false;
        bool builtIn = false;
        string funcName;

        BinaryTreePtr ptr = btGetVariable(symtable, Token.value.str); //Find this identifier in symtable


        //if(inFunction) {
        //    ptr = btGetVariable(symtable, functionName)->data.treeOfFunction;
        //    ptr = btGetVariable(ptr, Token.value.str);
        //}
        //else {
        //    ptr = btGetVariable(symtable, Token.value.str);
        //}

        //BinaryTreePtr ptr = btGetVariable(symtable, Token.value.str);


        if(ptr == NULL) {
            //check if called function isn't built in function

            if (inFunction) {
                ptr = btGetVariable(symtable, functionName)->data.treeOfFunction;
                ptr = btGetVariable(ptr, Token.value.str);

            } else {

                if (strcmp(Token.value.str, "length") == 0 || strcmp(Token.value.str, "substr") == 0 ||
                        strcmp(Token.value.str, "asc") == 0 || strcmp(Token.value.str, "chr") == 0) {

                    function = true;
                    builtIn = true;
                    funcName = Token.value;

                } else {
                    printErrAndExit(ERROR_PROG_SEM, "Undefined %s", Token.value.str);
                }
            }

        } else {
            if(ptr->data.isFunction) {
                function = true;
            }
        }

        //if this one is a function, then brackets are expected
        if (function) {
			if(!builtIn) {
                if(!ptr->data.defined) {
                    printErrAndExit(ERROR_PROG_SEM, "Try to call undefined function '%s'!", ptr->data.name);
                }
			}

            Token = getNextToken();

            if (Token.lexem != BRACKET_LEFT) {
                //TODO - is this syntax or semantics??
                printErrAndExit(ERROR_SYNTAX, "Try to call function without params. '(' was expected");
            }

            ast_stmt* varAssignFunction;

            BinaryTreePtr node;
            if(inFunction) {
                node = btGetVariable(symtable, functionName)->data.treeOfFunction;
                node = btGetVariable(node, name);
            }
            else {
                node = btGetVariable(symtable, name);
            }

            if(builtIn) {
                enum builtin_function en;
                if(strcmp(funcName.str, "length") == 0) {
                    en = Length;
                }
                else if(strcmp(funcName.str, "substr") == 0) {
                    en = SubStr;
                }
                else if(strcmp(funcName.str, "asc") == 0) {
                    en = Asc;
                }
                else
                    en = Chr;
                varAssignFunction = make_varAssignBuiltinFunctionStmt(node, en, NULL);
            }
            else {
                varAssignFunction = make_varAssignFunctionStmt(node, ptr, NULL);
            }
            // TODO: check args (number, datatypes, ...)
            // TODO: valid return datype of function in assign.

            stackPush(&stmtStack, NULL, NULL, NULL, PREC_E, varAssignFunction);

            int paramNumber = 0;
            stackItem item;
            params(builtIn, &paramNumber);

            if(builtIn) {
                stackTop(&stmtStack, &item);
                builtinFunction fc = builtinFunctions[item.stmt->op.var_assign_builtin_function_stmt.function];
                if(paramNumber != fc.argsNum && fc.argsNum != 0) {
                    printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and less were given!", fc.name, fc.argsNum);
                }
            }
            else {
                stackTop(&stmtStack, &item);
                BinaryTreePtr fc = item.stmt->op.var_assign_function_stmt.function;
                if(paramNumber != fc->data.paramNumber && fc->data.paramNumber != 0) {
                    printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and less were given!", fc->data.name, fc->data.paramNumber);
                }
            }

            stackTop(&stmtStack, &item);

            stackPop(&stmtStack);

            if(!stackEmpty(&stmtStack)) {
                stackTop(&stmtStack, &item);
                if(item.stmt->tag_stmt == function_definition_stmt) {
                    addStmtToArray(&item.stmt->op.function_definition_stmt.block, varAssignFunction);
                }
                else if(item.stmt->tag_stmt == while_stmt) {
                    addStmtToArray(&item.stmt->op.while_stmt.block, varAssignFunction);
                }
                else if(item.stmt->tag_stmt == if_stmt) {
                    addStmtToArray(&item.stmt->op.if_stmt.ifBlock, varAssignFunction);
                }
            }
            else {
                addStmtToArray(&globalStmtArray, varAssignFunction);
            }

            Token = PreviousToken;

            if (Token.lexem != BRACKET_RIGHT) {
                //TODO - is this syntax or semantics??
                printErrAndExit(ERROR_SYNTAX, "Try to call function. ')' was expected");
            }

            return;
        }
    }


	ast_exp* expressionTree;
	parseExpression(&Token, &expressionTree);

	BinaryTreePtr node;
	if(inFunction) {
		node = btGetVariable(symtable, functionName)->data.treeOfFunction;
		node = btGetVariable(node, name);
	}
	else {
		node = btGetVariable(symtable, name);
	}

	if(node->data.type == (datatype)exp_integer || node->data.type == (datatype)exp_decimal) {
        if(expressionTree->datatype == exp_string) {
            printErrAndExit(ERROR_TYPE_SEM, "Can't assign '%s' to '%s'!", getTypeString(expressionTree->datatype), getTypeString(node->data.type));
        }
	}
    else if(node->data.type == (datatype)exp_string && expressionTree->datatype != exp_string) {
        printErrAndExit(ERROR_TYPE_SEM, "Can't assign '%s' to '%s'!", getTypeString(expressionTree->datatype), getTypeString(node->data.type));
    }

	node->data.defined = true;

	ast_stmt* assign_stmt = make_varAssignStmt(node, expressionTree);

	stackItem item;
	if(!stackEmpty(&stmtStack)) {
		stackTop(&stmtStack, &item);
		if(item.stmt->tag_stmt == function_definition_stmt) {
			addStmtToArray(&item.stmt->op.function_definition_stmt.block, assign_stmt);
		}
		else if(item.stmt->tag_stmt == while_stmt) {
			addStmtToArray(&item.stmt->op.while_stmt.block, assign_stmt);
		}
		else if(item.stmt->tag_stmt == if_stmt) {
			addStmtToArray(&item.stmt->op.if_stmt.ifBlock, assign_stmt);
		}
	}
	else {
		addStmtToArray(&globalStmtArray, assign_stmt);
	}


    isExpression = true;
    PreviousToken = Token;



    /*
    //TODO - store expression value into the symtable - expressions not done yet
    //it's an idea - maybe should be placed right in expression function
    node = btGetVariable(symtable, name);
    datatype type = node->data.type;

    char *value;

    //find out what data type the variable is and then convert it to string
    switch(type) {
        case TYPE_NUMBER:
            //value = gcmalloc(sizeof(int));
            //TODO - convert to string

            break;

        case TYPE_DECIMAL:
            //value = gcmalloc(sizeof(double));
            //TODO - convert to string

            break;

        default:
            value = "";
            break;
    }

    if(node != NULL)
        node->data.value.str = value;
        */
}


/**
 * @copydoc unaryOperation
 */
bool unaryOperation(token Token) {
    if (Token.lexem != PLUS_ASSIGNMENT && Token.lexem != MINUS_ASSIGNMENT && Token.lexem != MULTIPLY_ASSIGNMENT &&
        Token.lexem != DIVISION_ASSIGNMENT && Token.lexem != BACKSLASH_ASSIGNMENT)
        return false;

    return true;
}


/**
 * @copydoc params
 */
void params(bool builtIn, int *paramNumber) {
    static int currentArgumentNum = -1;
    stackItem item;

    currentArgumentNum++;

	ast_exp* expressionTree;
	parseExpression(&PreviousToken, &expressionTree);

    if(builtIn) {
        stackTop(&stmtStack, &item);
        builtinFunction fc = builtinFunctions[item.stmt->op.var_assign_builtin_function_stmt.function];
        if(expressionTree == NULL && fc.argsNum == 0) {
            paramsNext(builtIn, paramNumber);
        }
        else if(expressionTree == NULL && currentArgumentNum != fc.argsNum) {
            printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and less were given!", fc.name, fc.argsNum);
        }
        else if(currentArgumentNum >= fc.argsNum) {
            printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and more were given!", fc.name, fc.argsNum);
        }
        else if(fc.types[currentArgumentNum] == (datatype)exp_integer || fc.types[currentArgumentNum] == (datatype)exp_decimal) {
            if(expressionTree->datatype == exp_string) {
                printErrAndExit(ERROR_TYPE_SEM, "In function '%s' %d. parameter has bad datatype!", fc.name, currentArgumentNum + 1);
            }
        }
        else if(fc.types[currentArgumentNum] == (datatype)exp_string && expressionTree->datatype != exp_string) {
            printErrAndExit(ERROR_TYPE_SEM, "In function '%s' %d. parameter has bad datatype!", fc.name, currentArgumentNum + 1);
        }
    }
    else {
        stackTop(&stmtStack, &item);
        BinaryTreePtr fc = item.stmt->op.var_assign_function_stmt.function;
        if(expressionTree == NULL && fc->data.paramNumber == 0) {
            paramsNext(builtIn, paramNumber);
        }
        else if(expressionTree == NULL && currentArgumentNum != fc->data.paramNumber) {
            printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and less were given!", fc->data.name, fc->data.paramNumber);
        }
        else if(currentArgumentNum >= fc->data.paramNumber) {
            printErrAndExit(ERROR_TYPE_SEM, "Function '%s' expected %d parameters and more were given!", fc->data.name, fc->data.paramNumber);
        }
        else if(fc->data.typeOfParams[currentArgumentNum] == (datatype)exp_integer || fc->data.typeOfParams[currentArgumentNum] == (datatype)exp_decimal) {
            if(expressionTree->datatype == exp_string) {
                printErrAndExit(ERROR_TYPE_SEM, "In function '%s' %d. parameter has bad datatype!", fc->data.name, currentArgumentNum + 1);
            }
        }
        else if(fc->data.typeOfParams[currentArgumentNum] == (datatype)exp_string && expressionTree->datatype != exp_string) {
            printErrAndExit(ERROR_TYPE_SEM, "In function '%s' %d. parameter has bad datatype!", fc->data.name, currentArgumentNum + 1);
        }
    }


    stackTop(&stmtStack, &item);

    if(item.stmt->tag_stmt == var_assign_function_stmt) {
        addArgumentToArray(&item.stmt->op.var_assign_function_stmt.args, expressionTree);
        //printf("%d\n", item.stmt->op.var_assign_function_stmt.args->argument->op.numberExp);
    }
    else {
        addArgumentToArray(&item.stmt->op.var_assign_builtin_function_stmt.args, expressionTree);
    }

    *paramNumber = currentArgumentNum + 1;

    paramsNext(builtIn, paramNumber);

    currentArgumentNum = -1;
}


void paramsNext(bool builtIn, int *paramNumber) {
    if (PreviousToken.lexem != COMMA) {
        return;
    }

    params(builtIn, paramNumber);
}


/**
 * @copydoc mainBody
 */
void mainBody() {
    mainBodyIt();

    token Token = getNextToken();
    eol(Token.lexem);

	addStmtToArray(&globalStmtArray, make_scopeStmt());

    statement();

    Token = PreviousToken;

    end(Token.lexem);

    mainBodyIt();

    Token = getNextToken();

    if (Token.lexem == EOL_ENUM) {
        Token = getNextToken();
    }

    if (Token.lexem != EOF) {
        printErrAndExit(ERROR_SYNTAX, "'Scope' block should be last");
    }
}


/**
 * @copydoc mainBodyIt
 */
void mainBodyIt() {
    token Token = PreviousToken;

    if (PreviousToken.lexem != SCOPE) {
        Token = getNextToken();

        if (Token.lexem != SCOPE) {
            printErrAndExit(ERROR_SYNTAX, "'Scope' was expected");
        }

    } else {
        //Resetting token
        PreviousToken.lexem = -1;
    }

    //Token = getNextToken();

    //eol(Token.lexem);
}
