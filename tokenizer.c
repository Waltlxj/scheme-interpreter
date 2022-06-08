#include "tokenizer.h"
#include <stdio.h>
#include "linkedlist.h"
#include "talloc.h"
#include <ctype.h>
#include <string.h>

// helper function for identifying initial characters of symbols
int isInitial(char c) {
    if (isalpha(c) || c == '!' ||
        c == '$' || c == '%' || c == '&' ||
        c == '*' || c == '/' || c == ':' ||
        c == '<' || c == '=' || c == '>' ||
        c == '?' || c == '~' || c == '_' ||
        c == '^') {
            return 1;
        } else {
            return 0;
        }
}

// helper function for identifying subsequent characters of symbols
int isSubsequent(char c) {
    if (isInitial(c) || isdigit(c) || c == '.' || c == '+' || c == '-') {
        return 1;
    } else {
        return 0;
    }
}

// helper function for identifying valid symbol
int isValid(char c) {
    if (isSubsequent(c) || c == ' ' || c == '\n') {
        return 1;
    } else {
        return 0;
    }
}

// Read all of the input from stdin, and return a linked list consisting of the
// tokens.
Value *tokenize() {
    char charRead;
    Value *list = makeNull();
    charRead = (char)fgetc(stdin);

    while (charRead != EOF) {

        
        if (charRead == ';') { // anything after a ; on a line is ignored
            while (charRead != '\n') {
                charRead = (char)fgetc(stdin);
            }

        } else if (charRead == '(') { //OPEN_TYPE
            Value *token = talloc(sizeof(Value));
            token->type = OPEN_TYPE;
            token->s = "("; //???? 
            list = cons(token, list);

        } else if (charRead == ')') { //CLOSE_TYPE
            Value *token = talloc(sizeof(Value));
            token->type = CLOSE_TYPE;
            token->s = ")"; //????
            list = cons(token, list);


        // take cares of numbers (integers and doubles) and plus/minus symbols
        } else if (isdigit(charRead) || charRead == '.' || 
        charRead == '-' || charRead == '+') { 
            char buffer[301];
            int isDouble = 0, index = 0;

            // put all related chars into a string
            while (isdigit(charRead) || charRead == '.' || 
            charRead == '-' || charRead == '+') {
                if (charRead == '.') {
                    isDouble = 1;
                }
                buffer[index] = charRead;
                index++;
                charRead = (char)fgetc(stdin);
            }
            buffer[index] = '\0';

            // create the token
            Value *token = talloc(sizeof(Value));
            char *ptr;
            if (!strcmp(buffer, "+") || !strcmp(buffer, "-")) { //plus/minus symbols
                token->type = SYMBOL_TYPE;
                token->s = talloc(2);
                strcpy(token->s, buffer);
            } else if (isDouble == 0) { // int
                token->type = INT_TYPE;
                token->i = strtol(buffer, &ptr, 10);
            } else { //double
                token->type = DOUBLE_TYPE;
                token->d = strtod(buffer, &ptr);
            }
            // insert
            list = cons(token, list);
            // step back one char
            ungetc(charRead, stdin);

        
        // takes care of string
        } else if (charRead == '\"') { // strings
            char currString[301];
            int index = 0;
            currString[index] = charRead; // put the first " in
            charRead = (char)fgetc(stdin);
            index++;
            // put the entire string in
            while (charRead != '\"') {
                currString[index] = charRead;
                index++;
                charRead = (char)fgetc(stdin);
            }
            // set up the tail
            currString[index] = '\"';
            currString[index+1] = '\0';

            Value *token = talloc(sizeof(Value));
            token->type = STR_TYPE;
            token->s = talloc(301);
            strcpy(token->s, currString);
            list = cons(token, list);
        

        // takes care of boolean
        } else if (charRead == '#') {
            Value *token = talloc(sizeof(Value));
            token->type = BOOL_TYPE;
            charRead = (char)fgetc(stdin);
            if (charRead == 't') {
                token->s = "#t";
            } else if (charRead == 'f') {
                token->s = "#f";
            } else {
                printf("Syntax error (readBoolean): boolean was not #t or #f\n");
                texit(1);
            }
            list = cons(token, list);


        // takes care of symbols other than +/-
        } else if (isInitial(charRead)) { 
            Value *token = talloc(sizeof(Value));
            token->type = SYMBOL_TYPE;
            token->s = talloc(301);
            // read symbol into buffer
            char buffer[301];
            int index = 0;
            while (isSubsequent(charRead)) {
                buffer[index] = charRead;
                index++;
                charRead = (char)fgetc(stdin);
            }
            buffer[index] = '\0';
            // copy buffer string to token
            strcpy(token->s, buffer);
            list = cons(token, list);
            // step back one char
            ungetc(charRead, stdin);
        
        // invalid symbols
        } else if (!isValid(charRead)) {
            printf("Syntax error: invalid symbol\n");
            texit(1);
        }

        // next char in file
        charRead = (char)fgetc(stdin);
    }

    Value *revList = reverse(list);
    return revList;
};

// Displays the contents of the linked list as tokens, with type information
// Types are: boolean, integer!, double!, string, symbol, open!, close!
void displayTokens(Value *list) {
    //printf("Linked List: \n");
    while (list->type != NULL_TYPE) {
        switch (list->c.car->type) {
            case INT_TYPE:
                printf("%i:integer\n", list->c.car->i);
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", list->c.car->s);
                break;
            case DOUBLE_TYPE:
                printf("%f:double\n", list->c.car->d);
                break;
            case STR_TYPE:
                printf("%s:string\n", list->c.car->s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", list->c.car->s);
                break;
            case OPEN_TYPE:
                printf("%s:open\n", list->c.car->s);
                break;
            case CLOSE_TYPE:
                printf("%s:close\n", list->c.car->s);
                break;
            default:
                printf("walt li");
                break;
        }
        list = list->c.cdr;
    }
};