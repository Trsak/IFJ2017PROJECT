/**
 * @file parser.c
 * @author Roman Bartl (xbartl06)
 * @brief TODO
 */


#include "parser.h"


static int last = -1;
static int tree[50];


bool program() {
    int token = getNextToken();

    if(functionFollow(token)) {     // block 'functions' is empty (epsilon)
        //TODO - add to derivation tree

        last++;
        tree[last] = token;


    } else {
        if(!functionFirst(token)) {
            printErrMsg(ERROR_SYNTAX, "");  //TODO - add err msg

            return false;
        }

        //TODO - add to derivation tree

        last++;
        tree[last] = token;


        if(!functions())
            return false;
    }


    if(!mainBody())
        return false;


    return true;
}


bool functionFirst(int token) {
    if (token == DECLARE || token == FUNCTION)
        return true;

    return false;
}


bool functions() {

    int token = tree[last]; // TODO get last added token from tree and compare it

    if(token != DECLARE && token != FUNCTION)
        token = getNextToken();


    if(token != DECLARE) {


        if(token != FUNCTION) {
            printErrMsg(ERROR_SYNTAX, "DECLARE or FUNCTION was expected");
            return false;
        }


        if(!functionHeader())
            return false;

        if(!statement())
            return false;

        if(!functionEnd())
            return false;

        if(!functionNext())
            return false;

        return true;
    }

    //TODO - add token to tree

    //last++;
    //tree[last] = token;

    if(!functionHeader())
        return false;

    if(!functionNext())
        return false;

    return true;


}


bool functionHeader() {
    int token;

    if(tree[last] != FUNCTION) {
        token = getNextToken();

        if (token != FUNCTION) { // TODO skip this condition if token is in the tree
            printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
            return false;
        }

        //TODO add token into the tree
        last++;
        tree[last] = token;

    }


    token = getNextToken();

    if(token != ID) {
        printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    //TODO add token into the tree
    last++;
    tree[last] = token;

    token = getNextToken();

    if(!functionAsFirst(token)) {
        if (!functionItFirst(token)) {
            printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
            return false;
        }

        // TODO add token into tree
        last++;
        tree[last] = token;

        if(!functionIt())
            return false;

    } else {

        last++;
        tree[last] = token;
    }

    if(!functionAs())
        return false;


    token = getNextToken();


    if(token != EOL) {
        printErrMsg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

bool functionIt() {
    //TODO - is this necessary?
    // if(getNextToken() == BRACKET_LEFT); // TODO - if token is in tree, skip this condition


    if(!declareParams()) {
        return false;
    }

    if(tree[last] == BRACKET_RIGHT)
        return true;

    int token = getNextToken();

    if(token != BRACKET_RIGHT){
        printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

bool functionItFirst(int token) {
    if(token == BRACKET_LEFT)
        return true;

    return false;
}

bool functionAsFirst(int token) {
    if(token == AS)
        return true;

    return false;
}


bool functionAs() {

    if(tree[last] != AS) {
        int token = getNextToken();

        if (token != AS) {
            printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
            return false;
        } //TODO - if token is already in tree, skip this condition

        last++;
        tree[last] = token;

    }

    if(!dataType())
        return false;

    return true;
}

bool functionNext() {


    int token = getNextToken();

    if(functionFollow(token)) {
        //TODO - add into tree
        last++;
        tree[last] = token;

        return true;
    }


    if(!functionFirst(token)) {
        printErrMsg(ERROR_SYNTAX, "");  //TODO - add err msg

        return false;
    }

    //TODO - add to derivation tree
    last++;
    tree[last] = token;

    if(!functions())
        return false;


    return true;
}

bool functionEnd() {
    int token = getNextToken();

    if(token != END) {
        printErrMsg(ERROR_SYNTAX, "");
        return false;
    }

    //TODO - add to derivation tree
    last++;
    tree[last] = token;

    token = getNextToken();

    if(token != FUNCTION) {
        printErrMsg(ERROR_SYNTAX, "");
        return false;
    }

    //TODO - add to derivation tree
    last++;
    tree[last] = token;

    token = getNextToken();

    if(token != EOL) {
        printErrMsg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}


bool functionFollow(int token) {
    if(token == SCOPE)
        return true;

    return false;
}


bool declareParams() {
    int token = getNextToken();

    //TODO - if last added is comma, skip this condition
    if(tree[last] != COMMA) {
        if (declareParamsFollow(token)) {
            //TODO - add to derivation tree

            last++;
            tree[last] = token;
            return true;
        }
    }

    if(token != ID) {
        printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    last++;
    tree[last] = token;

    if(!functionAs()) {
        return false;
    }

    if(!declareParamsNext()) {
        return false;
    }

    return true;
}

bool declareParamsNext() {
    int token = getNextToken();

    if(declareParamsFollow(token)) {
        //TODO - add to derivation tree

        last++;
        tree[last] = token;
        return true;
    }

    if(token != COMMA) {
        printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    last++;
    tree[last] = token;

    if(!declareParams())
        return false;

    return true;
}

bool declareParamsFollow(int token) {
    if(token == BRACKET_RIGHT)
        return true;

    return false;
}


bool dataType() {
    int token = getNextToken();

    if(token != INTEGER && token != DOUBLE && token != STRING) {
        printErrMsg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    //TODO - add into the tree
    last++;
    tree[last] = token;

    return true;
}

bool statement() {
    return true;
}

bool end() {
    int token = getNextToken();


    if(token != END) {
        printErrMsg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

bool mainBody() { //TODO - find if token is already in derivation tree, then skip conditions


    if(!mainBodyIt())
        return false;


    if(!statement()) {
        return false;
    }

    if(!end()) {
        return false;
    }

    if(!mainBodyIt())
        return false;
/*
    if(mainBodyIt());
    if(statement());
    if(end());
    if(mainBodyIt());
*/
    return true;
}



bool mainBodyIt() {
    int token = tree[last];

    if(token != SCOPE) {
        token = getNextToken();

        if(token != SCOPE) {
            printErrMsg(ERROR_SYNTAX, "");
            return false;
        }

        last++;
        tree[last] = token;
    }


    token = getNextToken();

    if(token != EOL){
        printErrMsg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

/*
bool statement() {
    return true;
}

bool end_token() {
    if (getNextToken() == END) {
        // TODO insert into derivation tree
        return true;
    }

    return false;
}

//bool eol_token() {
    //if (getNextToken() == EOL) TODO
  //  return true;
//}
*/