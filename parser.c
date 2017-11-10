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


#include "parser.h"

int last = -1; //TODO delete - only for debug



/**
 * @copydoc parse
 */
int parse() {

    if(!program()) {
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

    int token;

    token = getNextToken();

    if (functionFollow(token)) {

        last++;
        tree[last] = token;

    } else {

        if (!functionFirst(token)) {
            return false;
        }

        last++;
        tree[last] = token;

        if (!functions())
            return false;
    }

    if (!mainBody())
        return false;

    return true;
}


/**
 * @copydoc functionFirst
 */
bool functionFirst(int token) {
    if (token == DECLARE || token == FUNCTION)
        return true;


    printErrMsg(ERROR_SYNTAX, "Function statement was expected");
    returnError = ERROR_SYNTAX;

    return false;
}


/**
 * @copydoc functions
 */
bool functions() {

    int token = tree[last];

    if (token != DECLARE && token != FUNCTION) {
        token = getNextToken();
    }


    if (token != DECLARE) {

        if (token != FUNCTION) {
            printErrMsg(ERROR_SYNTAX, "'Declare' or 'Function' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }


        if (!functionHeader()) {
            return false;
        }

        if (!statement()) {
            return false;
        }

        if (!functionEnd()) {
            return false;
        }

        if (!functionNext()) {
            return false;
        }


        return true;
    }


    if (!functionHeader())
        return false;

    if (!functionNext())
        return false;


    return true;
}


/**
 * @copydoc functionHeader
 */
bool functionHeader() {
    int token = tree[last];

    if (token != FUNCTION) {

        token = getNextToken();

        if (token != FUNCTION) { // TODO skip this condition if token is in the tree DO-NOT-KNOW
            printErrMsg(ERROR_SYNTAX, "'Function' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }

        last++;
        tree[last] = token;
    }

    //Here parser expects that identifier can be wrong formatted
    if(!nextToken(&token)) {
        return false;
    }

    if (token != ID) {
        printErrMsg(ERROR_SYNTAX, "'Identifier' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    last++;
    tree[last] = token;


    token = getNextToken();

    if (!functionAsFirst(token)) {
        if (!functionItFirst(token)) {
            printErrMsg(ERROR_SYNTAX, "'(' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }

        last++;
        tree[last] = token;

        if (!functionIt())
            return false;

    } else {

        last++;
        tree[last] = token;
    }

    if (!functionAs())
        return false;


    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc functionIt
 */
bool functionIt() {

    if (!declareParams()) {
        return false;
    }

    int token = tree[last];

    if (token == BRACKET_RIGHT)
        return true;

    token = getNextToken();

    if (token != BRACKET_RIGHT) {
        printErrMsg(ERROR_SYNTAX, "')' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc functionItFirst
 */
bool functionItFirst(int token) {
    if (token == BRACKET_LEFT)
        return true;

    return false;
}


/**
 * @copydoc functionAsFirst
 */
bool functionAsFirst(int token) {
    if (token == AS)
        return true;

    return false;
}


/**
 * @copydoc functionAs
 */
bool functionAs() {

    if (tree[last] != AS) {
        int token = getNextToken();

        if (token != AS) {
            printErrMsg(ERROR_SYNTAX, "'As' was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }

        last++;
        tree[last] = token;

    }

    if (!dataType())
        return false;

    return true;
}


/**
 * @copydoc functionNext
 */
bool functionNext() {
    int token;

    do
        token = getNextToken();
    while (token == 10);            //TODO - this is not nice!!!


    if (functionFollow(token)) {
        last++;
        tree[last] = token;

        return true;
    }


    if (!functionFirst(token)) {
        return false;
    }

    last++;
    tree[last] = token;

    if (!functions())
        return false;

    return true;
}


/**
 * @copydoc functionEnd
 */
bool functionEnd() {

    int token = getNextToken();

    if (tree[last] != END) {

        if (!end(token)) {
            return false;
        }

        last++;
        tree[last] = token;

        token = getNextToken();
    }


    if (token != FUNCTION) {
        printErrMsg(ERROR_SYNTAX, "'Function' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

    last++;
    tree[last] = token;

    token = getNextToken();

    if (eol(token)) {
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}


/**
 * @copydoc functionFollow
 */
bool functionFollow(int token) {
    if (token == SCOPE)
        return true;

    return false;
}


/**
 * @copydoc declareParams
 */
bool declareParams() {
    int token;

    //Here parser expects that identifier can be wrong formatted
    if(!nextToken(&token)) {
        return false;
    }

    if (tree[last] != COMMA) {

        if (declareParamsFollow(token)) {

            last++;
            tree[last] = token;

            return true;
        }
    }

    if (token != ID) {
        printErrMsg(ERROR_SYNTAX, "'Identifier' was expected'");
        returnError = ERROR_SYNTAX;

        return false;
    }

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

    if (declareParamsFollow(token)) {

        last++;
        tree[last] = token;

        return true;
    }

    if (token != COMMA) {
        printErrMsg(ERROR_SYNTAX, "',' was expected"); //TODO - err msg
        returnError = ERROR_SYNTAX;

        return false;
    }

    last++;
    tree[last] = token;

    if (!declareParams())
        return false;

    return true;
}


/**
 * @copydoc declareParamsFollow
 */
bool declareParamsFollow(int token) {
    if (token == BRACKET_RIGHT)
        return true;

    return false;
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
    if(!nextToken(&token)) {
        return false;
    }

    if (statementFollow(token)) {
        return true;
    }

    switch (token) {
        case DIM:
        case STATIC:
        case SHARED:

            last++;
            tree[last] = token;

            if (!varDeclaration())
                return false;

            break;

        case INPUT:

            last++;
            tree[last] = token;

            //Here parser expects that identifier can be wrong formatted
            if(!nextToken(&token)) {
                return false;
            }

            if (token != ID) {
                printErrMsg(ERROR_SYNTAX, "'Identifier' was expected'");
                returnError = ERROR_SYNTAX;

                return false;
            }

            last++;
            tree[last] = token;

            break;

        case PRINT:

            last++;
            tree[last] = token;

            if (!expression()) {
                return false;
            }

            token = getNextToken();


            last++;
            tree[last] = token;

            token = getNextToken(); //TODO delete, only for debug  DO-NOT-KNOW


            if (token != SEMICOLON) {
                printErrMsg(ERROR_SYNTAX, "';' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            last++;
            tree[last] = token;

            if (!printNext()) {
                return false;
            }

            break;


        case DO:

            last++;
            tree[last] = token;

            token = getNextToken();

            if (token != WHILE) {
                printErrMsg(ERROR_SYNTAX, "'While' was expected");
                return false;
            }

            last++;
            tree[last] = token;

            if (!expression()) {
                return false;
            }

            token = getNextToken();

            if(!eol(token)) {
                return false;
            }

            last++;
            tree[last] = token;

            if (!statement()) {
                return false;
            }

            token = getNextToken();

            if (tree[last] != LOOP) {
                if (token != LOOP) {
                    printErrMsg(ERROR_SYNTAX, "'Loop' was expected");
                    returnError = ERROR_SYNTAX;

                    return false;
                }
            }

            last++;
            tree[last] = token;


            break;


        case IF:

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

            last++;
            tree[last] = token;

            token = getNextToken();

            if (!eol(token)) {
                return false;
            }

            last++;
            tree[last] = token;


            if (!statement()) {
                return false;
            }


            if (!ifNext()) {
                return false;
            }


            if (tree[last] != END) {
                token = getNextToken();

                if (!end(token)) {
                    return false;
                }

                last++;
                tree[last] = token;
            }

            token = getNextToken();


            if (token != IF) {
                printErrMsg(ERROR_SYNTAX, "'If' was expected");
                returnError = ERROR_SYNTAX;

                return false;
            }

            last++;
            tree[last] = token;


            break;

    }

    if (tree[last] != EOL) {
        token = getNextToken();

        if (!eol(token)) {
            return false;
        }

        last++;
        tree[last] = token;
    }


    if (!statement())
        return false;


    return true;
}


/**
 * @copydoc printNext
 */
bool printNext() {

    int token = getNextToken();

    if (eol(token)) {
        last++;
        tree[last] = token;

        return true;
    }


    if (expressionFirst(token)) {
        last++;
        tree[last] = token;

        if (!expression()) {
            return false;
        }

    } else {
        printErrMsg(ERROR_SYNTAX, ""); //TODO - it's expression and that's not done yet
        returnError = ERROR_SYNTAX;

        return false;

    }

    token = getNextToken();

    if (token != SEMICOLON) {
        printErrMsg(ERROR_SYNTAX, "';' was expected");
        returnError = ERROR_SYNTAX;

        return false;
    }

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

    int token = tree[last];


    if (token != ELSE && token != ELSEIF) {
        token = getNextToken();

        if (ifNextFollow(token)) {
            return true;
        }
    }

    if (token == ELSE) {

        token = getNextToken();

        if (!eol(token)) {
            return false;
        }

        last++;
        tree[last] = token;

        if (!statement()) {
            return false;
        }

    } else if (token == ELSEIF) {

        if (!elseIf()) {
            return false;
        }

    } else {

        printErrMsg(ERROR_SYNTAX, "'Else' or 'ElseIf' was expected");
        returnError = ERROR_SYNTAX;

        return false;
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

    last++;
    tree[last] = token;

    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

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

    int token = tree[last];

    if (token == ELSEIF) {

        if (!elseIf()) {
            return false;
        }

    } else {

        if (ifNextFollow(token)) {
            return true;

        } else {

            return false;
        }


    }

    return true;
}


/**
 * @copydoc ifNextFollow
 */
bool ifNextFollow(int token) {
    if (token == END) {

        last++;
        tree[last] = token;

        return true;
    }

    printErrMsg(ERROR_SYNTAX, "'End' was expected");
    returnError = ERROR_SYNTAX;

    return false;
}


/**
 * @copydoc statementFollow
 */
bool statementFollow(int token) {
    if (token == END || token == LOOP || token == ELSE || token == ELSEIF) {

        last++;
        tree[last] = token;

        return true;
    }

    return false;
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
 * @copydoc varDeclaration
 */
bool varDeclaration() {
    int token;

    //Here parser expects that identifier can be wrong formatted
    if(!nextToken(&token)) {
        return false;
    }

    if (token != ID) {
        printErrMsg(ERROR_SYNTAX, "'Identifier' was expected");
        return false;
    }

    last++;
    tree[last] = token;

    if (!functionAs()) {
        return false;
    }

    if (!assignment()) {
        return false;
    }

    return true;
}


/**
 * @copydoc assignment
 */
bool assignment() {
    int token;

    if (tree[last] != ASSIGNMENT) {
        token = getNextToken();

        if (eol(token)) {
            last++;
            tree[last] = token;

            return true;
        }


        if (assignmentMarkFirst(token)) {
            last++;
            tree[last] = token;

            if (!assignmentMark())
                return false;

        } else {

            printErrMsg(ERROR_SYNTAX, "Assignment was expected");
            returnError = ERROR_SYNTAX;

            return false;
        }
    } else {

        if (!assignmentMark())
            return false;
    }


    token = getNextToken();
    last++;
    tree[last] = token; //TODO delete this ... only for debuging (expression)


    if (!expression())
        return false;

    return true;
}


/**
 * @copydoc assignmentMarkFirst
 */
bool assignmentMarkFirst(int token) {
    if (token == ASSIGNMENT || token == PLUS || token == MINUS ||
        token == MULTIPLY || token == DIVISION || token == BACKSLASH)
        return true;


    return false;
}


/**
 * @copydoc assignmentMark
 */
bool assignmentMark() {
    int token = tree[last];


    if (token == PLUS || token == MINUS ||
        token == MULTIPLY || token == DIVISION || token == BACKSLASH) {

        token = getNextToken();

        if (token != ASSIGNMENT) {
            if (!expressionFirst(token)) {
                printErrMsg(ERROR_SYNTAX, ""); //TODO - expression is not done
                returnError = ERROR_SYNTAX;

                return false;
            }

            last++;
            tree[last] = token; //TODO concatenate these two tokens

            return true;

        }

        last++;
        tree[last] = token;
    }


    return true;
}


/**
 * @copydoc params
 */
bool params() {
    //int token = getNextToken();

    //dont know what this 'todo' is for
    //if() TODO


    if (!expression())
        return false;

    if (!paramsNext())
        return false;

    return true;
}


/**
 * @copydoc paramsNext
 */
bool paramsNext() {
    return true;
}


/**
 * @copydoc paramsFollow
 */
bool paramsFollow(int token) {
    if (token == BRACKET_RIGHT)
        return true;

    return false;
}


/**
 * @copydoc expression
 */
bool expression() {
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

    if (!mainBodyIt())
        return false;


    if (!statement()) {
        return false;
    }

    if (tree[last] != END) {
        int token = getNextToken();

        if (!end(token)) {
            return false;
        }

        last++;
        tree[last] = token;
    }

    if (!mainBodyIt())
        return false;


    return true;
}


/**
 * @copydoc mainBodyIt
 */
bool mainBodyIt() {
    int token = tree[last];

    if (token != SCOPE) {
        token = getNextToken();

        if (token != SCOPE) {
            printErrMsg(ERROR_SYNTAX, "'Scope' was expected");
            return false;
        }

        last++;
        tree[last] = token;
    }


    token = getNextToken();

    if (!eol(token)) {
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}
