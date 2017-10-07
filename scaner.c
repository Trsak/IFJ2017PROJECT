//
// Created by Bartosek on 05.10.2017.
//

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scaner.h"
#include "string.h"

string attr; // Globální proměnná pro posílání attributu

FILE *source;
void setSourceFile(FILE *f){
    source = f;
    strInit(&attr);
}

int getNextToken(){
    int state = 0;
    int c;
    strClear(&attr);

    while(1){
        c = getc(source);

        switch(state){
            case 0: // Tady začínám
                if (isspace(c)); // Je to bílý znak - ignoruji
                else if (c == '{'){ // Je to komentář
                    state = 1;
                }
                else if (isalpha(c)){ // Je to identifikátor, nebo klíčové slovo
                    strAddChar(&attr, c);
                    state = 2;
                }
                else if (isdigit(c)) { // Je to číslo
                    strAddChar(&attr, c);
                    state = 3;
                }
                else if (c == ':'){ // Je to přiřazení
                    state = 4;
                }
                else if (c == '+'){ // Je to operátor + nebo ++
                    state = 5;
                }
                else if(c == '-'){ // Je to operátor - nebo --
                    state = 6;
                }
                else if (c == '<'){
                    state = 7;
                }
                else if (c == '*'){
                    return MUL;
                }
                else if (c == '/'){
                    return DIV;
                }
                else if (c == EOF){
                    return END_OF_FILE;
                }
                else{
                    return LEX_ERROR;
                }
                break;

            case 1: // Komentář
                if (c == '}'){
                    state = 0;
                }
                else if (c == EOF){ // Neukončený komentář
                    return LEX_ERROR;
                }
                break;

            case 2: // Identifikátor / Klíčové slovo
                if (isalnum(c)){
                    strAddChar(&attr, c);
                }
                else {
                    ungetc(c, source);
                }
                for (int i = 0; i < sizeof(keyWords); i++){
                    if (strcmp(&attr.str, keyWords[i])){
                        return 20 + i;
                    }
                }
                return ID;

            case 3: // Číslo
                if (isdigit(c)){
                    strAddChar(&attr, c);
                }
                else if (isalpha(c)){
                    return LEX_ERROR;
                }
                else{
                    ungetc(c, source);
                    return NUM;
                }
                break;

            case 4: // Přiřazení
                if (c == '='){
                    return ASSIGNMENT;
                }
                return LEX_ERROR;

            case 5: // + nebo ++
                if (c == '+'){
                    return INC;
                }
                else{
                    ungetc(c, source);
                    return PLUS;
                }

            case 6: // - nebo --
                if (c == '-'){
                    return DEC;
                }
                else{
                    ungetc(c, source);
                    return MINUS;
                }

            //TODO co je to za operátory?Je to správně?
            case 7: // <= nebo <> nebo <
                if (c == '='){
                    return LEQ;
                }
                else if (c == '>'){
                    return NEQ;
                }
                else{
                    ungetc(c, source);
                    return LTN;
                }
        }
    }
}