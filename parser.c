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
        node->data.defined = node->data.declared || defined;
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


/**
 * @copydoc program
 */
void program() {
    token Token = getNextToken();

    if (functionFirst(Token.lexem)) {
        PreviousToken = Token;

        functions();

    } else {
        PreviousToken = Token;
    }

    inFunction = false;

    mainBody();
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

    if (Token.lexem != DECLARE) {
        functionHeader(false, true);

        statement();

        functionEnd();

    } else {

        functionHeader(true, false);
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

    char *name = Token.value.str;
    saveFunctionName(Token.value);

    /** Semantics: Check if identifier is already in symtable and it's declaration|definition */
    BinaryTreePtr node = btGetVariable(symtable, name);
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


    switch (Token.lexem) {
        case ID:
            name = Token.value.str;

            assignment(false, name);

            break;

        case DIM:
            Token = getNextToken();

            IdToken(Token.lexem);

            name = Token.value.str;

            datatype type;
            asDataType(&type);

            /** Semantics: Check if variable was already declared */
            BinaryTreePtr node;
            if(inFunction) {
                BinaryTreePtr node1;
                node1 = btGetVariable(symtable, functionName)->data.treeOfFunction;
                node = btGetVariable(node1, name);
                if(strcmp(name, functionName) == 0) {
                    printErrAndExit(ERROR_PROG_SEM, "Already exists function '%s'!", name);
                }
                if (node && node->data.declared) {
                    printErrAndExit(ERROR_PROG_SEM, "Variable '%s' already declared!", name);
                }

                //Create new node - declaration of variable
                BinaryTreePtr params = NULL;
                createNode(&node1, name, type, true, false, false, &params, NULL, 0); // Add new arguments
            }
            else {
                node = btGetVariable(symtable, name);
                if (node && node->data.declared) {
                    printErrAndExit(ERROR_PROG_SEM, "Variable '%s' already declared!", name);
                }

                //Create new node - declaration of variable
                BinaryTreePtr params = NULL;
                createNode(&symtable, name, type, true, false, false, &params, NULL, 0); // Add new arguments
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
            node->data.defined = true;

            break;

        case INPUT:
            Token = getNextToken();
            IdToken(Token.lexem);

            break;

        case PRINT:
            expression();

            Token = getNextToken();

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

            expression();

            Token = getNextToken();

            eol(Token.lexem);

            statement();

            Token = PreviousToken;

            if (Token.lexem != LOOP) {
                Token = getNextToken();
                if (Token.lexem != LOOP) {
                    printErrAndExit(ERROR_SYNTAX, "'Loop' was expected");
                }
            }

            break;

        case IF:
            expression();

            Token = getNextToken();

            if (Token.lexem != THEN) {
                printErrAndExit(ERROR_SYNTAX, "'Then' was expected");
            }

            Token = getNextToken();

            eol(Token.lexem);

            statement();

            ifNext();

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

            expression();

            break;

        default:
            PreviousToken = Token;
            return;
    }

    Token = PreviousToken;

    if (Token.lexem != EOL) {
        Token = getNextToken();

        eol(Token.lexem);

    } else {
        //Resetting token
        PreviousToken.lexem = -1;
    }

    statement();
}


/**
 * @copydoc printNext
 */
void printNext() {
    token Token = getNextToken();

    if (Token.lexem == EOL) {
        PreviousToken = Token;
        return;
    }

    if (Token.lexem == 999) { //TODO
        expression();
    }

    Token = getNextToken();

    if (Token.lexem != SEMICOLON) {
        printErrAndExit(ERROR_SYNTAX, "';' was expected");
    }

    printNext();
}


/**
 * @copydoc ifNext
 */
void ifNext() {
    token Token = PreviousToken;

    if (Token.lexem != ELSE && Token.lexem != ELSEIF) {
        return;
    }

    if (Token.lexem == ELSEIF) {

        elseIf();
        Token = PreviousToken;
    }


    if (Token.lexem == ELSE) {
        Token = getNextToken();

        eol(Token.lexem);

        statement();
    }
}


/**
 * @copydoc elseIf
 */
void elseIf() {
    expression();

    token Token = getNextToken();

    if (Token.lexem != THEN) {
        printErrAndExit(ERROR_SYNTAX, "'Then' was expected");
    }

    Token = getNextToken();

    eol(Token.lexem);

    statement();

    Token = PreviousToken;

    if (Token.lexem == ELSEIF) {
        elseIf();

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
    if (lexem != EOL) {
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

    expression();



    //TODO - store expression value into the symtable - expressions not done yet
    //it's an idea - maybe should be placed right in expression function
    BinaryTreePtr node = btGetVariable(symtable, name);
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
bool params() {
    //TODO

    expression();

    return true;
}


/**
 * @copydoc expression
 */
void expression() {
    //TODO expressions are not done ... delete this after
    getNextToken();
}


/**
 * @copydoc  expressionFirst
 */
bool expressionFirst(token Token) {
    //TODO - delete after implementation this function
    Token.lexem = Token.lexem + 1;
    return true;
}


/**
 * @copydoc mainBody
 */
void mainBody() {
    mainBodyIt();

    statement();

    token Token = PreviousToken;

    end(Token.lexem);

    mainBodyIt();

    Token = getNextToken();

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

    Token = getNextToken();

    eol(Token.lexem);
}
