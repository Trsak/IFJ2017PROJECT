/**
 * @file parser.c
 * @author Roman Bartl (xbartl06)
 * @brief Parsing all blocks, statements and expressions
 */


//TODO instead of array, use global variable
//TODO expressions
//TODO return errors (even from scanner) to main
//TODO adding identifiers into symtable binary tree
//TODO return statement
//TODO function for id check
//TODO add statement (id = expr)
//TODO declare assignment
//TODO rename functionAs() to something else
//TODO comments

//TODO - end function takes argument with statement of what end is expected

//TODO - free every Token to avoid memory leaks

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
 * @copydoc nextToken
 */
bool nextToken(token *ReturnToken) {
    token Token = getNextToken();

    if (Token.lexem == -1) {
        returnError = ERROR_SCANNER;
        return false;
    }

    *ReturnToken = Token;

    return true;
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

    //Here parser expects that identifier can be wrong formatted
    if (!nextToken(&Token)) {
        return false;
    }

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

    if (!functionAs()) {
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
 * @copydoc functionAs
 */
bool functionAs() {
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
    token Token;

    //Here parser expects that identifier can be wrong formatted
    if (!nextToken(&Token)) {
        return false;
    }

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

    if (!functionAs()) {
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
    token Token;

    //Here parser expects that identifier can be wrong formatted
    if (!nextToken(&Token)) {
        return false;
    }

    switch (Token.lexem) {
        case DIM:
        case STATIC:
        case SHARED:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            //Here parser expects that identifier can be wrong formatted
            if (!nextToken(&Token)) {
                return false;
            }

            if (Token.lexem != ID) {
                printErrAndExit (ERROR_SYNTAX, "'Identifier' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = Token.lexem;

            if (!functionAs()) {
                return false;
            }

            //if (!assignment()) { TODO
            //   return false;
            //}

            break;

        case INPUT:
            //only for debug
            last++;
            tree[last] = Token.lexem;

            //Here parser expects that identifier can be wrong formatted
            if (!nextToken(&Token)) {
                return false;
            }

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
        printErrAndExit (ERROR_SYNTAX, "'Else' or 'ElseIf' was expected");
        returnError = ERROR_SYNTAX;

        return false;
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

    } else {
        //Always will be ELSEIF
        if (!elseIf()) {
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

    if (!elseIfNext()) {
        return false;
    }

    return true;
}


/**
 * @copydoc elseIfNext
 */
bool elseIfNext() {
    token Token = PreviousToken;

    if (Token.lexem == ELSEIF) {
        if (!elseIf()) {
            return false;
        }

    } else {
        return true;
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
    /*
    int token = getNextToken();

    printf("%d\n\n\n", token);

    if (token != ASSIGNMENT && token != PLUS_ASSIGNMENT && token != MINUS_ASSIGNMENT &&
        token != BACKSLASH_ASSIGNMENT && token != DIVISION_ASSIGNMENT && token != MULTIPLY_ASSIGNMENT) {

        printf("Ahoj");
        PreviousToken = token;

        return true;
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!expression()) {
        return false;
    }
    */
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

    if (Token.lexem != END) {
        Token = getNextToken();

        if (!end(Token)) {
            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = Token.lexem;

    if (!mainBodyIt()) {
        return false;
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
