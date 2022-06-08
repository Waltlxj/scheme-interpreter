#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "talloc.h"

// Create a new NULL_TYPE value node.
Value *makeNull(){
    Value *newNode = talloc(sizeof(Value));
    newNode->type = NULL_TYPE;
    return newNode;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr) {
    Value *newNode = talloc(sizeof(Value));
    newNode->type = CONS_TYPE;
    newNode->c.car = newCar;
    newNode->c.cdr = newCdr;
    return newNode;
};

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list) { // do we need a temp pointer?
    printf("Linked List: \n");
    while (list->type != NULL_TYPE) {
        switch (list->c.car->type) {
            case INT_TYPE:
                printf("%i\n", list->c.car->i);
                break;
            case DOUBLE_TYPE:
                printf("%f\n", list->c.car->d);
                break;
            case STR_TYPE:
                printf("%s\n", list->c.car->s);
                break;
            default:
                break;
        }
        list = list->c.cdr;
    }
};

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list) {
    Value *reversed = makeNull();
    while(list->type != NULL_TYPE){
        reversed = cons(list->c.car, reversed);
        list = list->c.cdr;
    }
    return reversed;
};


// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list) {
    assert(list != NULL);
    assert(list->type == CONS_TYPE);
    return list->c.car;
};

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list) {
    assert(list != NULL);
    assert(list->type == CONS_TYPE);
    return list->c.cdr;
};

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value) {
    assert(value != NULL);
    return value->type == NULL_TYPE;
};

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value) {
    assert(value != NULL);
    if (value->type != CONS_TYPE) {
        return 0;
    }
    
    int c = 0;
    
    while (value->type != NULL_TYPE) {
        c += 1;
        value = value->c.cdr;
    }
    return c;
};
