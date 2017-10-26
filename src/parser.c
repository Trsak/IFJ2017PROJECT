/**
 * @file parser.c
 * @author Roman Bartl (xbartl06)
 * @brief TODO
 */


#include "parser.h"

//static int tokens[50] = {DECLARE, FUNCTION, ID, BRACKET_LEFT, ID, AS, INTEGER, BRACKET_RIGHT, AS, DOUBLE, EOL};
//static int tokens[50] = {DECLARE, FUNCTION, ID, BRACKET_LEFT, BRACKET_RIGHT, AS, DOUBLE, EOL};
//static int tokens[50] = {DECLARE, FUNCTION, ID, AS, DOUBLE, EOL};
//static int tokens[50] = {FUNCTION, ID, AS, DOUBLE, EOL, END, FUNCTION, EOL};
//static int tokens[50] = {DECLARE, FUNCTION, ID, BRACKET_LEFT, ID, AS, INTEGER, COMMA, ID, AS, DOUBLE, BRACKET_RIGHT, AS, STRING, EOL, SCOPE};

/*
static int tokens[50] = {
        FUNCTION, ID, AS, DOUBLE, EOL, END, FUNCTION, EOL,
        FUNCTION, ID, AS, STRING, EOL, END, FUNCTION, EOL,
        DECLARE, FUNCTION, ID, BRACKET_LEFT, ID, AS, INTEGER, COMMA, ID, AS, STRING, BRACKET_RIGHT, AS, DOUBLE, EOL,
        SCOPE, EOL, END, SCOPE, EOL};





int getNextToken() {
    static int id = -1;

    id++;

    return tokens[id];
}
*/
static int last = -1;

static int tree[50];

bool program() {
    int token = getNextToken();

    if(function_follow(token)) {     // block 'functions' is empty (epsilon)
        //TODO - add to derivation tree

        last++;
        tree[last] = token;


    } else {
        if(!function_first(token)) {
            print_err_msg(ERROR_SYNTAX, "");  //TODO - add err msg

            return false;
        }

        //TODO - add to derivation tree

        last++;
        tree[last] = token;


        if(!functions())
            return false;
    }


    if(!main_body())
        return false;


    return true;
}


bool function_first(int token) {
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
            print_err_msg(ERROR_SYNTAX, "DECLARE or FUNCTION was expected");
            return false;
        }


        if(!function_header())
            return false;

        if(!statement())
            return false;

        if(!function_end())
            return false;

        if(!function_next())
            return false;

        return true;
    }

    //TODO - add token to tree

    //last++;
    //tree[last] = token;

    if(!function_header())
        return false;

    if(!function_next())
        return false;

    return true;


}


bool function_header() {
    int token;

    if(tree[last] != FUNCTION) {
        token = getNextToken();

        if (token != FUNCTION) { // TODO skip this condition if token is in the tree
            print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
            return false;
        }

        //TODO add token into the tree
        last++;
        tree[last] = token;

    }


    token = getNextToken();

    if(token != ID) {
        print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    //TODO add token into the tree
    last++;
    tree[last] = token;

    token = getNextToken();

    if(!function_as_first(token)) {
        if (!function_it_first(token)) {
            print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
            return false;
        }

        // TODO add token into tree
        last++;
        tree[last] = token;

        if(!function_it())
            return false;

    } else {

        last++;
        tree[last] = token;
    }

    if(!function_as())
        return false;


    token = getNextToken();


    if(token != EOL) {
        print_err_msg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

bool function_it() {
    //TODO - is this necessary?
    // if(getNextToken() == BRACKET_LEFT); // TODO - if token is in tree, skip this condition


    if(!declare_params()) {
        return false;
    }

    if(tree[last] == BRACKET_RIGHT)
        return true;

    int token = getNextToken();

    if(token != BRACKET_RIGHT){
        print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

bool function_it_first(int token) {
    if(token == BRACKET_LEFT)
        return true;

    return false;
}

bool function_as_first(int token) {
    if(token == AS)
        return true;

    return false;
}


bool function_as() {

    if(tree[last] != AS) {
        int token = getNextToken();

        if (token != AS) {
            print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
            return false;
        } //TODO - if token is already in tree, skip this condition

        last++;
        tree[last] = token;

    }

    if(!data_type())
        return false;

    return true;
}

bool function_next() {


    int token = getNextToken();

    if(function_follow(token)) {
        //TODO - add into tree
        last++;
        tree[last] = token;

        return true;
    }


    if(!function_first(token)) {
        print_err_msg(ERROR_SYNTAX, "");  //TODO - add err msg

        return false;
    }

    //TODO - add to derivation tree
    last++;
    tree[last] = token;

    if(!functions())
        return false;


    return true;
}

bool function_end() {
    int token = getNextToken();

    if(token != END) {
        print_err_msg(ERROR_SYNTAX, "");
        return false;
    }

    //TODO - add to derivation tree
    last++;
    tree[last] = token;

    token = getNextToken();

    if(token != FUNCTION) {
        print_err_msg(ERROR_SYNTAX, "");
        return false;
    }

    //TODO - add to derivation tree
    last++;
    tree[last] = token;

    token = getNextToken();

    if(token != EOL) {
        print_err_msg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}


bool function_follow(int token) {
    if(token == SCOPE)
        return true;

    return false;
}


bool declare_params() {
    int token = getNextToken();

    //TODO - if last added is comma, skip this condition
    if(tree[last] != COMMA) {
        if (declare_params_follow(token)) {
            //TODO - add to derivation tree

            last++;
            tree[last] = token;
            return true;
        }
    }

    if(token != ID) {
        print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    last++;
    tree[last] = token;

    if(!function_as()) {
        return false;
    }

    if(!declare_params_next()) {
        return false;
    }

    return true;
}

bool declare_params_next() {
    int token = getNextToken();

    if(declare_params_follow(token)) {
        //TODO - add to derivation tree

        last++;
        tree[last] = token;
        return true;
    }

    if(token != COMMA) {
        print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
        return false;
    }

    last++;
    tree[last] = token;

    if(!declare_params())
        return false;

    return true;
}

bool declare_params_follow(int token) {
    if(token == BRACKET_RIGHT)
        return true;

    return false;
}


bool data_type() {
    int token = getNextToken();

    if(token != INTEGER && token != DOUBLE && token != STRING) {
        print_err_msg(ERROR_SYNTAX, ""); //TODO - err msg
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
        print_err_msg(ERROR_SYNTAX, "");
        return false;
    }

    last++;
    tree[last] = token;

    return true;
}

bool main_body() { //TODO - find if token is already in derivation tree, then skip conditions


    if(!main_body_it())
        return false;


    if(!statement()) {
        return false;
    }

    if(!end()) {
        return false;
    }

    if(!main_body_it())
        return false;
/*
    if(main_body_it());
    if(statement());
    if(end());
    if(main_body_it());
*/
    return true;
}



bool main_body_it() {
    int token = tree[last];

    if(token != SCOPE) {
        token = getNextToken();

        if(token != SCOPE) {
            print_err_msg(ERROR_SYNTAX, "");
            return false;
        }

        last++;
        tree[last] = token;
    }


    token = getNextToken();

    if(token != EOL){
        print_err_msg(ERROR_SYNTAX, "");
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