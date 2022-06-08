#include "parser.h"
#include "linkedlist.h"
#include "talloc.h"
#include <stdio.h>
#include <assert.h>


// function takes in a pre-existing tree, a token to add to it, 
// and a pointer to an integer depth. depth is updated to represent 
//the number of unclosed open parentheses in the parse tree. 
Value *addToParseTree(Value *tree, int *depth, Value *token) {
    if (token->type == OPEN_TYPE) { // open 
        tree = cons(token, tree);
        *depth += 1;
    }

    else if (token->type == CLOSE_TYPE) { // close
        Value *subtree = makeNull();
        while (tree->type != NULL_TYPE && car(tree)->type != OPEN_TYPE) { // pop until reach open or end
            subtree = cons(car(tree), subtree);
            tree = cdr(tree);
        }
        
        if (tree->type == NULL_TYPE) { // no open found in main list
            printf("Syntax error: too many closed parentheses.\n");
            texit(1);
        } 

        tree = cdr(tree); // pop open paren off
        tree = cons(subtree, tree); // add sublist to main list if reached open
        
        *depth -= 1;
        
    }
    else { // others
        tree = cons(token, tree);
    }
    return tree;
}


// Takes a list of tokens from a Scheme program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens) {
    Value *tree = makeNull();
    int depth = 0;

    Value *current = tokens;
    assert(current != NULL && "Error (parse): null pointer");

    while (current->type != NULL_TYPE) {
        Value *token = car(current);
        //printf("Added: %s\n", token->s);
        tree = addToParseTree(tree, &depth, token);
        
        // printTree(tree);
        // printf("\n\n");
        
        current = cdr(current);
    }
    if (depth != 0) {
        printf("Syntax error: too many open parentheses.\n");
        texit(1);
    }
    return reverse(tree);
};


// Prints the tree to the screen in a readable fashion. It should look just like
// Scheme code; use parentheses to indicate subtrees.
void printTree(Value *tree) {
    // printing everything in a linkedlist
    while (tree->type != NULL_TYPE) {
        if (tree->type != CONS_TYPE) {
            printf(". ");
            switch (tree->type) {
                case INT_TYPE:
                    printf("%i ", tree->i);
                    break;
                case DOUBLE_TYPE:
                    printf("%f ", tree->d);
                    break;
                case CONS_TYPE:
                    printf("(");
                    printTree(tree);
                    printf(") ");
                    break;
                case NULL_TYPE:
                    printf("()");
                    break;
                default:
                    printf("%s ", tree->s);
                    break;
            }
            return;
        } else {
            switch (car(tree)->type) {
                case INT_TYPE:
                    printf("%i ", car(tree)->i);
                    break;
                case DOUBLE_TYPE:
                    printf("%f ", car(tree)->d);
                    break;
                case CONS_TYPE:
                    printf("(");
                    printTree(car(tree));
                    printf(") ");
                    break;
                case NULL_TYPE:
                    printf("()");
                    break;
                default:
                    printf("%s ", car(tree)->s);
                    break;
            }
            tree = cdr(tree);
        }
    }
};