/**
 * @file parser.c
 * @author Roman Bartl (xbartl06)
 * @brief Parsing all blocks, statements and expressions
 */


//TODO expressions
//TODO adding identifiers into symtable binary tree
//TODO function for id check


#include "parser.h"

int last = -1; //TODO delete - only for debug

/*
 * TODO
 * @copydoc idToken
 *
void IdToken() {
    token Token = getNextToken();

    if (Token.lexem != ID) {
        printErrAndExit(ERROR_SYNTAX, "'Identifier' was excepted");
    }
}


*
 * @copydoc createNode
 *
void createNode(char *name, datatype type, bool declared, bool defined) { //TODO value
    BinaryTreePtr *root = NULL;
    Values val;

    val.name = name;
    val.type = type;
    val.declared = declared;
    val.defined = defined;

    btInsert(root, val);
}
*/

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
        functionHeader();

        statement();

        functionEnd();

    } else {
        //only for debug
        last++;
        tree[last] = Token.lexem;

        functionHeader();
    }

    functionNext();
}


/**
 * @copydoc functionHeader
 */
void functionHeader() {
    token Token = PreviousToken;

    if (Token.lexem != FUNCTION) {
        Token = getNextToken();

        if (Token.lexem != FUNCTION) {
            printErrAndExit(ERROR_SYNTAX, "'Function' was expected");
        }
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (Token.lexem != ID) {
        printErrAndExit(ERROR_SYNTAX, "'Identifier' was expected");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (Token.lexem != BRACKET_LEFT) {
        printErrAndExit(ERROR_SYNTAX, "'(' was expected");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    declareParams();

    Token = PreviousToken;

    if (Token.lexem != BRACKET_RIGHT) {
        printErrAndExit(ERROR_SYNTAX, "')' was expected");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    asDataType();

    Token = getNextToken();

    eol(Token);

    //only for debug
    last++;
    tree[last] = Token.lexem;
}


/**
 * @copydoc asDataType
 */
void asDataType() {
    token Token = getNextToken();

    if (Token.lexem != AS) {
        printErrAndExit(ERROR_SYNTAX, "'As' was expected");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    dataType();
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

    //only for debug
    last++;
    tree[last] = Token.lexem;

    functions();
}


/**
 * @copydoc functionEnd
 */
void functionEnd() {
    token Token = PreviousToken;

    if (PreviousToken.lexem != END) {
        Token = getNextToken();

        end(Token);
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (Token.lexem != FUNCTION) {
        printErrAndExit(ERROR_SYNTAX, "'Function' was expected");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    eol(Token);

    //only for debug
    last++;
    tree[last] = Token.lexem;
}


/**
 * @copydoc declareParams
 */
void declareParams() {
    token Token = getNextToken();

    if (Token.lexem != ID) {
        if (PreviousToken.lexem == COMMA) {
            printErrAndExit(ERROR_SYNTAX, "'Identifier' was expected'");
        }

        PreviousToken = Token;
        return;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    asDataType();

    declareParamsNext();
}


/**
 * @copydoc declareParamsNext
 */
void declareParamsNext() {
    token Token = getNextToken();

    if (Token.lexem != COMMA) {
        PreviousToken = Token;
        return;
    }

    PreviousToken = Token;

    //only for debug
    last++;
    tree[last] = Token.lexem;

    declareParams();
}


/**
 * @copydoc dataType
 */
void dataType() {
    token Token = getNextToken();

    if (Token.lexem != INTEGER && Token.lexem != DOUBLE && Token.lexem != STRING) {
        printErrAndExit(ERROR_SYNTAX, "Data type was expected");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;
}


/**
 * @copydoc statement
 */
void statement() {
    token Token = getNextToken();

    switch (Token.lexem) {
        case ID:
            unaryOp = true; // set flag on true - here can be unary operation

            //only for debug
            last++;
            tree[last] = Token.lexem;

            assignment(false);

            break;

        case DIM:

            unaryOp = false; // set flag on false - no unary operation can occur

            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != ID) {
                printErrAndExit(ERROR_SYNTAX, "'Identifier' was expected");
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            asDataType();

            assignment(true);

            break;

        case INPUT:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != ID) {
                printErrAndExit(ERROR_SYNTAX, "'Identifier' was expected'");
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            break;

        case PRINT:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            expression();

            Token = getNextToken();

            if (Token.lexem != SEMICOLON) {
                printErrAndExit(ERROR_SYNTAX, "';' was expected");
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            printNext();

            break;

        case DO:
            //only for bebug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != WHILE) {
                printErrAndExit(ERROR_SYNTAX, "'While' was expected");
            }

            //only for bebug
            last++;
            tree[last] = Token.lexem;

            expression();

            Token = getNextToken();

            eol(Token);

            //only for bebug
            last++;
            tree[last] = Token.lexem;

            statement();

            Token = PreviousToken;

            if (Token.lexem != LOOP) {
                Token = getNextToken();
                if (Token.lexem != LOOP) {
                    printErrAndExit(ERROR_SYNTAX, "'Loop' was expected");
                }
            }

            //only for bebug
            last++;
            tree[last] = Token.lexem;

            break;

        case IF:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            expression();

            Token = getNextToken();

            if (Token.lexem != THEN) {
                printErrAndExit(ERROR_SYNTAX, "'Then' was expected");
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            eol(Token);

            //only for debug
            last++;
            tree[last] = Token.lexem;

            statement();

            ifNext();

            Token = PreviousToken;

            if (Token.lexem != END) {
                Token = getNextToken();

                end(Token);
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != IF) {
                printErrAndExit(ERROR_SYNTAX, "'If' was expected");
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            break;

        case RETURN:
            if (!inFunction) {
                printErrAndExit(ERROR_SYNTAX, "'Return' statement not in function!");
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            expression();

            break;

        default:
            PreviousToken = Token;
            return;
    }

    Token = PreviousToken;

    if (Token.lexem != EOL) {
        Token = getNextToken();

        eol(Token);

    } else {
        PreviousToken.lexem = 90000; //TODO
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

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

    //only for debug
    last++;
    tree[last] = Token.lexem;

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


    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (Token.lexem == ELSEIF) {

        elseIf();
        Token = PreviousToken;
    }


    if (Token.lexem == ELSE) {
        Token = getNextToken();

        eol(Token);

        //only for debug
        last++;
        tree[last] = Token.lexem;

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

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    eol(Token);

    //only for debug
    last++;
    tree[last] = Token.lexem;

    statement();

    Token = PreviousToken;

    if (Token.lexem == ELSEIF) {
        //only for debug
        last++;
        tree[last] = Token.lexem;

        elseIf();

    } else {
        PreviousToken = Token;
    }
}


/**
 * @copydoc end
 */
void end(token Token) {
    if (Token.lexem != END) {
        printErrAndExit(ERROR_SYNTAX, "'End' was expected");
    }
}


/**
 * @copydoc eol
 */
void eol(token Token) {
    if (Token.lexem != EOL) {
        printErrAndExit(ERROR_SYNTAX, "'End-Of-Line' was expected");
    }
}


/**
 * @copydoc assignment
 */
void assignment(bool isDeclaration) {
    token Token = getNextToken();

    if (Token.lexem != ASSIGNMENT && !unaryOperation(Token)) {
        if(!isDeclaration) {
            printErrAndExit(ERROR_SYNTAX, "'Identifier' cannot stand alone");
        }

        PreviousToken = Token;
        return;
    }

    if (unaryOperation(Token) && !unaryOp) {
        printErrAndExit(ERROR_SYNTAX, "Cannot do unary operation in declaration statement");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    expression();
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
    token Token = getNextToken();

    //only for debug
    last++;
    tree[last] = Token.lexem;
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

    end(Token);

    //only for debug
    last++;
    tree[last] = Token.lexem;

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
        PreviousToken.lexem = 1000; // TODO
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    eol(Token);

    //only for debug
    last++;
    tree[last] = Token.lexem;
}
