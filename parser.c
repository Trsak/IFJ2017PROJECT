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
bool nextToken(int *returnToken) {
    int token = getNextToken();

    if (token == -1) {
        returnError = ERROR_SCANNER;
        return false;
    }

    *returnToken = token;

    return true;
}


/**
 * @copydoc program
 */
bool program() {
    int token = getNextToken();

    if (functionFirst(token)) {
        previousToken = token;

        if (!functions()) {
            return false;
        }

    } else {
        previousToken = token;
    }

    if (!mainBody()) {
        return false;
    }

    return true;
}


/**
 * @copydoc functionFirst
 */
bool functionFirst(int token) {
    if (token == DECLARE || token == FUNCTION) {
        return true;
    }

    return false;
}


/**
 * @copydoc functions
 */
bool functions() {
    int token = previousToken;

    if (token != DECLARE) {
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
        tree[last] = token;

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
    int token = previousToken;

    if (token != FUNCTION) {
        token = getNextToken();

        if (token != FUNCTION) {
            printErrMsg(ERROR_SYNTAX, "'Function' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = token;

    //Here parser expects that identifier can be wrong formatted
    if (!nextToken(&token)) {
        return false;
    }

    if (token != ID) {
        printErrMsg(ERROR_SYNTAX, "'Identifier' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    token = getNextToken();

    if (token != BRACKET_LEFT) {
        printErrMsg(ERROR_SYNTAX, "'(' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!declareParams()) {
        return false;
    }

    token = previousToken;

    if (token != BRACKET_RIGHT) {
        printErrMsg(ERROR_SYNTAX, "')' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!functionAs()) {
        return false;
    }

    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc functionAs
 */
bool functionAs() {
    int token = getNextToken();

    if (token != AS) {
        printErrMsg(ERROR_SYNTAX, "'As' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!dataType()) {
        return false;
    }

    return true;
}


/**
 * @copydoc functionNext
 */
bool functionNext() {
    int token = getNextToken();

    if (!functionFirst(token)) {
        previousToken = token;

        return true;
    }

    previousToken = token;

    //only for debug
    last++;
    tree[last] = token;

    if (!functions()) {
        return false;
    }

    return true;
}


/**
 * @copydoc functionEnd
 */
bool functionEnd() {
    int token = previousToken;

    if (previousToken != END) {
        token = getNextToken();

        if (!end(token)) {
            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = token;

    token = getNextToken();

    if (token != FUNCTION) {
        printErrMsg(ERROR_SYNTAX, "'Function' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc declareParams
 */
bool declareParams() {
    int token;

    //Here parser expects that identifier can be wrong formatted
    if (!nextToken(&token)) {
        return false;
    }

    if (token != ID) {
        if (previousToken == COMMA) {
            printErrMsg(ERROR_SYNTAX, "'Identifier' was expected'");
            returnError = ERROR_SYNTAX;

            return false;
        }

        previousToken = token;

        return true;
    }

    //only for debug
    last++;
    tree[last] = token;

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
    int token = getNextToken();

    if (token != COMMA) {
        previousToken = token;

        return true;
    }

    previousToken = token;

    //only for debug
    last++;
    tree[last] = token;


    if (!declareParams()) {
        return false;
    }

    return true;
}


/**
 * @copydoc dataType
 */
bool dataType() {
    int token = getNextToken();

    if (token != INTEGER && token != DOUBLE && token != STRING) {
        printErrMsg(ERROR_SYNTAX, "Data type was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc statement
 */
bool statement() {
    int token;

    //Here parser expects that identifier can be wrong formatted
    if (!nextToken(&token)) {
        return false;
    }

    switch (token) {
        case DIM:
        case STATIC:
        case SHARED:
            //only for debug
            last++;
            tree[last] = token;

            //Here parser expects that identifier can be wrong formatted
            if (!nextToken(&token)) {
                return false;
            }

            if (token != ID) {
                printErrMsg(ERROR_SYNTAX, "'Identifier' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = token;

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
            tree[last] = token;

            //Here parser expects that identifier can be wrong formatted
            if (!nextToken(&token)) {
                return false;
            }

            if (token != ID) {
                printErrMsg(ERROR_SYNTAX, "'Identifier' was expected'");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = token;

            break;

        case PRINT:
            //only for debug
            last++;
            tree[last] = token;

            if (!expression()) {
                return false;
            }

            token = getNextToken();

            if (token != SEMICOLON) {
                printErrMsg(ERROR_SYNTAX, "';' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = token;

            if (!printNext()) {
                return false;
            }

            break;

        case DO:
            //only for bebug
            last++;
            tree[last] = token;

            token = getNextToken();

            if (token != WHILE) {
                printErrMsg(ERROR_SYNTAX, "'While' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for bebug
            last++;
            tree[last] = token;

            if (!expression()) { //TODO - expression
                return false;
            }

            token = getNextToken();

            if (!eol(token)) {
                return false;
            }

            if (!statement()) {
                return false;
            }

            token = previousToken;

            if (token != LOOP) {
                token = getNextToken();

                if (token != LOOP) {
                    printErrMsg(ERROR_SYNTAX, "'Loop' was expected");
                    returnError = ERROR_SYNTAX;

                    return false;
                }
            }

            //only for bebug
            last++;
            tree[last] = token;

            break;

        case IF:
            //only for debug
            last++;
            tree[last] = token;

            if (!expression()) {
                return false;
            }

            token = getNextToken();

            if (token != THEN) {
                printErrMsg(ERROR_SYNTAX, "'Then' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = token;

            token = getNextToken();

            if (!eol(token)) {
                return false;
            }

            //only for debug
            last++;
            tree[last] = token;

            if (!statement()) {
                return false;
            }

            if (!ifNext()) {
                return false;
            }

            token = previousToken;

            if (token != END) {
                token = getNextToken();

                if (!end(token)) {
                    return false;
                }
            }

            //only for debug
            last++;
            tree[last] = token;

            token = getNextToken();

            if (token != IF) {
                printErrMsg(ERROR_SYNTAX, "'If' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            //only for debug
            last++;
            tree[last] = token;

            break;

        default:
            previousToken = token;
            return true;
    }

    token = previousToken;

    if (token != EOL) {
        token = getNextToken();

        if (!eol(token)) {
            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!statement()) {
        return false;
    }

    return true;
}


/**
 * @copydoc printNext
 */
bool printNext() {
    int token = getNextToken();

    if (token == EOL) {
        previousToken = token;
        return true;
    }

    if (token == 999) {
        if (!expression()) {        //TODO - delete because expression
            return false;
        }
    }

    token = getNextToken();

    if (token != SEMICOLON) {
        printErrMsg(ERROR_SYNTAX, "';' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!printNext()) {
        return false;
    }

    return true;
}


/**
 * @copydoc ifNext
 */
bool ifNext() {
    int token = previousToken;

    if (token != ELSE && token != ELSEIF) {
        printErrMsg(ERROR_SYNTAX, "'Else' or 'ElseIf' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    if (token == ELSE) {
        token = getNextToken();

        if (!eol(token)) {
            return false;
        }

        //only for debug
        last++;
        tree[last] = token;


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

    int token = getNextToken();

    if (token != THEN) {
        printErrMsg(ERROR_SYNTAX, "'Then' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = token;


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
    int token = previousToken;

    if (token == ELSEIF) {
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
bool end(int token) {
    if (token != END) {
        printErrMsg(ERROR_SYNTAX, "'End' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    return true;
}


/**
 * @copydoc eol
 */
bool eol(int token) {
    if (token != EOL) {
        printErrMsg(ERROR_SYNTAX, "'End-Of-Line' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    return true;
}


/**
 * @copydoc assignment
 */
bool assignment() {
    int token = getNextToken();

    printf("%d\n\n\n", token);

    if (token != ASSIGNMENT && token != PLUS_ASSIGNMENT && token != MINUS_ASSIGNMENT &&
        token != BACKSLASH_ASSIGNMENT && token != DIVISION_ASSIGNMENT && token != MULTIPLY_ASSIGNMENT) {

        printf("Ahoj");
        previousToken = token;

        return true;
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!expression()) {
        return false;
    }

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
    int token = getNextToken();

    //only for debug
    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc  expressionFirst
 */
bool expressionFirst(int token) {
    //TODO - delete after implementation this function
    token = token + 1;
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

    int token = previousToken;

    if (token != END) {
        token = getNextToken();

        if (!end(token)) {
            return false;
        }
    }

    //only for debug
    last++;
    tree[last] = token;

    if (!mainBodyIt()) {
        return false;
    }

    return true;
}


/**
 * @copydoc mainBodyIt
 */
bool mainBodyIt() {
    int token = previousToken;

    if (previousToken != SCOPE) {
        token = getNextToken();

        if (token != SCOPE) {
            printErrMsg(ERROR_SYNTAX, "'Scope' was expected");
            return false;
        }

    } else {
        previousToken = 0;
    }

    //only for debug
    last++;
    tree[last] = token;

    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

    //only for debug
    last++;
    tree[last] = token;

    return true;
}
