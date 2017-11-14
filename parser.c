/**
 * @file parser.c
 * @author Roman Bartl (xbartl06)
 * @brief Parsing all blocks, statements and expressions
 */


//TODO expressions
//TODO adding identifiers into symtable binary tree
//TODO function for id check
//TODO comments

//TODO - end function takes argument with statement of what end is expected


#include "parser.h"

int last = -1; //TODO delete - only for debug



/**
 * @copydoc parse
 */
int parse() {
    if (!program()) {
        return returnError;
    }

    return 0;
}


/**
 * @copydoc program
 */
bool program() {
    token Token = getNextToken();

    if (functionFirst(Token.lexem)) {
        PreviousToken = Token;

        if (!functions()) {
            return false;
        }

    } else {
        PreviousToken = Token;
    }

    inFunction = false;

    if (!mainBody()) {
        return false;
    }

    return true;
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
bool functions() {
    token Token = PreviousToken;

    inFunction = true;

    if (Token.lexem != DECLARE) {
        if (!functionHeader()) {
            return false;
        }

        if (!statement()) {
            return false;
        }

        if (!functionEnd()) {
            return false;
        }

    } else {
        //only for debug
        last++;
        tree[last] = Token.lexem;

        if (!functionHeader()) {
            return false;
        }
    }

    if (!functionNext()) {
        return false;
    }

    return true;
}


/**
 * @copydoc functionHeader
 */
bool functionHeader() {
    token Token = PreviousToken;

    if (Token.lexem != FUNCTION) {
        Token = getNextToken();

        if (Token.lexem != FUNCTION) {
            printErrAndExit (ERROR_SYNTAX, "'Function' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (Token.lexem != ID) {
        printErrAndExit (ERROR_SYNTAX, "'Identifier' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (Token.lexem != BRACKET_LEFT) {
        printErrAndExit (ERROR_SYNTAX, "'(' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!declareParams()) {
        return false;
    }

    Token = PreviousToken;

    if (Token.lexem != BRACKET_RIGHT) {
        printErrAndExit (ERROR_SYNTAX, "')' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!asDataType()) {
        return false;
    }

    Token = getNextToken();

    if (!eol(Token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    return true;
}


/**
 * @copydoc asDataType
 */
bool asDataType() {
    token Token = getNextToken();

    if (Token.lexem != AS) {
        printErrAndExit (ERROR_SYNTAX, "'As' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!dataType()) {
        return false;
    }

    return true;
}


/**
 * @copydoc functionNext
 */
bool functionNext() {
    token Token = getNextToken();

    if (!functionFirst(Token.lexem)) {
        PreviousToken = Token;

        return true;
    }

    PreviousToken = Token;

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!functions()) {
        return false;
    }

    return true;
}


/**
 * @copydoc functionEnd
 */
bool functionEnd() {
    token Token = PreviousToken;

    if (PreviousToken.lexem != END) {
        Token = getNextToken();

        if (!end(Token)) {
            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (Token.lexem != FUNCTION) {
        printErrAndExit (ERROR_SYNTAX, "'Function' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (!eol(Token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    return true;
}


/**
 * @copydoc declareParams
 */
bool declareParams() {
    token Token = getNextToken();

    if (Token.lexem != ID) {
        if (PreviousToken.lexem == COMMA) {
            printErrAndExit (ERROR_SYNTAX, "'Identifier' was expected'");
            returnError = ERROR_SYNTAX;

            return false;
        }

        PreviousToken = Token;

        return true;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!asDataType()) {
        return false;
    }

    if (!declareParamsNext()) {
        return false;
    }

    return true;
}


/**
 * @copydoc declareParamsNext
 */
bool declareParamsNext() {
    token Token = getNextToken();

    if (Token.lexem != COMMA) {
        PreviousToken = Token;
        return true;
    }

    PreviousToken = Token;

    //only for debug
    last++;
    tree[last] = Token.lexem;


    if (!declareParams()) {
        return false;
    }

    return true;
}


/**
 * @copydoc dataType
 */
bool dataType() {
    token Token = getNextToken();

    if (Token.lexem != INTEGER && Token.lexem != DOUBLE && Token.lexem != STRING) {
        printErrAndExit (ERROR_SYNTAX, "Data type was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    return true;
}


/**
 * @copydoc statement
 */
bool statement() {
    token Token = getNextToken();

    switch (Token.lexem) {
        case ID:
            unaryOp = true; // set flag on true - here can be unary operation

            //only for debug
            last++;
            tree[last] = Token.lexem;

            if(!assignment()) {
                return false;
            }

            break;

        case DIM:
        case STATIC:
        case SHARED:

            unaryOp = false; // set flag on false - no unary operation can occur

            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != ID) {
                printErrAndExit (ERROR_SYNTAX, "'Identifier' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            if (!asDataType()) {
                return false;
            }

            if(!assignment()) {
                return false;
            }

            break;

        case INPUT:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != ID) {
                printErrAndExit (ERROR_SYNTAX, "'Identifier' was expected'");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            break;

        case PRINT:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            if (!expression()) {
                return false;
            }

            Token = getNextToken();

            if (Token.lexem != SEMICOLON) {
                printErrAndExit (ERROR_SYNTAX, "';' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            if (!printNext()) {
                return false;
            }

            break;

        case DO:
            //only for bebug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != WHILE) {
                printErrAndExit (ERROR_SYNTAX, "'While' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for bebug
            last++;
            tree[last] = Token.lexem;

            if (!expression()) { //TODO - expression
                return false;
            }

            Token = getNextToken();

            if (!eol(Token)) {
                return false;
            }

            if (!statement()) {
                return false;
            }

            Token = PreviousToken;

            if (Token.lexem != LOOP) {
                Token = getNextToken();
                if (Token.lexem != LOOP) {
                    printErrAndExit (ERROR_SYNTAX, "'Loop' was expected");
                    returnError = ERROR_SYNTAX;

                    return false;
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

            if (!expression()) {
                return false;
            }

            Token = getNextToken();

            if (Token.lexem != THEN) {
                printErrAndExit (ERROR_SYNTAX, "'Then' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (!eol(Token)) {
                return false;
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            if (!statement()) {
                return false;
            }

            if (!ifNext()) {
                return false;
            }

            Token = PreviousToken;

            if (Token.lexem != END) {
                Token = getNextToken();

                if (!end(Token)) {
                    return false;
                }
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            Token = getNextToken();

            if (Token.lexem != IF) {
                printErrAndExit (ERROR_SYNTAX, "'If' was expected");
                returnError = ERROR_SYNTAX;

                return false;
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

            if(!expression()) {
                return false;
            }

            break;

        default:
            PreviousToken = Token;
            return true;
    }

    Token = PreviousToken;

    if (Token.lexem != EOL) {
        Token = getNextToken();

        if (!eol(Token)) {
            return false;
        }
    } else {
        PreviousToken.lexem = 90000; //TODO
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!statement()) {
        return false;
    }

    return true;
}


/**
 * @copydoc printNext
 */
bool printNext() {
    token Token = getNextToken();

    if (Token.lexem == EOL) {
        PreviousToken = Token;
        return true;
    }

    if (Token.lexem == 999) {
        if (!expression()) {        //TODO - delete because expression
            return false;
        }
    }

    Token = getNextToken();

    if (Token.lexem != SEMICOLON) {
        printErrAndExit (ERROR_SYNTAX, "';' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!printNext()) {
        return false;
    }

    return true;
}


/**
 * @copydoc ifNext
 */
bool ifNext() {
    token Token = PreviousToken;

    if (Token.lexem != ELSE && Token.lexem != ELSEIF) {
        return true;
    }


    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (Token.lexem == ELSEIF) {

        if (!elseIf()) {
            return false;
        }

        Token = PreviousToken;
    }



    if (Token.lexem == ELSE) {
        Token = getNextToken();

        if (!eol(Token)) {
            return false;
        }

        //only for debug
        last++;
        tree[last] = Token.lexem;

        if (!statement()) {
            return false;
        }
    }

    return true;
}


/**
 * @copydoc elseIf
 */
bool elseIf() {
    if (!expression()) {
        return false;
    }

    token Token = getNextToken();

    if (Token.lexem != THEN) {
        printErrAndExit (ERROR_SYNTAX, "'Then' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (!eol(Token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;


    if (!statement()) {
        return false;
    }

    Token = PreviousToken;

    if (Token.lexem == ELSEIF) {
        //only for debug
        last++;
        tree[last] = Token.lexem;

        if(!elseIf()) {
            return false;
        }

    } else {
        PreviousToken = Token;
    }

    return true;
}


/**
 * @copydoc end
 */
bool end(token Token) {
    if (Token.lexem != END) {
        printErrAndExit (ERROR_SYNTAX, "'End' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    return true;
}


/**
 * @copydoc eol
 */
bool eol(token Token) {
    if (Token.lexem != EOL) {
        printErrAndExit (ERROR_SYNTAX, "'End-Of-Line' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    return true;
}


/**
 * @copydoc assignment
 */
bool assignment() {
    token Token = getNextToken();

    if(Token.lexem != ASSIGNMENT && !unaryOperation(Token)) {
        PreviousToken = Token;
        return true;
    }

    if(unaryOperation(Token) && !unaryOp) {
        printErrAndExit(ERROR_SYNTAX, "Cannot do unary operation in declaration statement");
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!expression()) {
        return false;
    }

    return true;
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

    if (!expression())
        return false;

    return true;
}


/**
 * @copydoc expression
 */
bool expression() {
    //TODO expressions are not done ... delete this after
    token Token = getNextToken();

    //only for debug
    last++;
    tree[last] = Token.lexem;

    return true;
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
bool mainBody() {
    if (!mainBodyIt()) {
        return false;
    }

    if (!statement()) {
        return false;
    }

    token Token = PreviousToken;
    
    if (!end(Token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!mainBodyIt()) {
        return false;
    }

    Token = getNextToken();

    if (Token.lexem != EOF) {
        printErrAndExit(ERROR_SYNTAX, "'Scope' block should be last");
    }

    return true;
}


/**
 * @copydoc mainBodyIt
 */
bool mainBodyIt() {
    token Token = PreviousToken;

    if (PreviousToken.lexem != SCOPE) {
        Token = getNextToken();

        if (Token.lexem != SCOPE) {
            printErrAndExit (ERROR_SYNTAX, "'Scope' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }

    } else {
        PreviousToken.lexem = 1000; // TODO
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    Token = getNextToken();

    if (!eol(Token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    return true;
}
