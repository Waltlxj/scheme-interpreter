#include "linkedlist.h"
#include "talloc.h"
#include "interpreter.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>

// take in a value that is not a cons cell and print it
void printValue(Value *value) {
    switch (value->type) {
        case INT_TYPE:
            printf("%i\n", value->i);
            break;
        case BOOL_TYPE:
            printf("%s\n", value->s);
            break;
        case DOUBLE_TYPE:
            printf("%f\n", value->d);
            break;
        case STR_TYPE:
            printf("%s\n", value->s);
            break;
        case SYMBOL_TYPE:
            printf("%s\n", value->s);
            break;
        case CONS_TYPE:
            printf("(");
            printTree(value);
            printf(")\n");
            break;
        case NULL_TYPE:
            printf("(");
            printf(")\n");
            break;
        case CLOSURE_TYPE:
            printf("#<procedure>\n");
            break;
        case VOID_TYPE:
            break;
        default:
            printf("Oops not printable!\n");
            break;
    }
    return;
}

// PRIMITIVES

// primitive function for +
Value *primitiveAdd(Value *args) {
    
    int isDouble = 0;
    float sum = 0;

    // loop throgh args
    while (!isNull(args)) {
        Value *cur = car(args);
        if (cur->type == DOUBLE_TYPE) {
            isDouble = 1;
            sum += cur->d;
        } else if (cur->type == INT_TYPE) {
            sum += cur->i;
        } else {
            printf("Evaluation error: '+' has invalid argument(s).\n");
            texit(1);
        }
        args = cdr(args);
    }
    
    // make result Value
    Value *result = talloc(sizeof(Value));
    if (isDouble) {
        result->type = DOUBLE_TYPE;
        result->d = sum;
    } else {
        result->type = INT_TYPE;
        result->i = (int) sum;
    }
    return result;
}

// primitive function for null?
Value *primitiveNull(Value *args) {

    if (isNull(args)) {
        printf("Evaluation error: no argument supplied to 'null?'\n");
        texit(1);
    }

    if (!isNull(cdr(args))){
        printf("Evaluation error: too many arguments supplied to 'null?'\n");
        texit(1);
    }

    Value* result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (isNull(car(args))) {
        result->s = "#t";
    } else {
        result->s = "#f";
    }
    return result;
}

// primitive function for cons
Value *primitiveCons(Value *args){

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to cons\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to cons\n");
        texit(1);
    }

    return cons(car(args), car(cdr(args)));
    
}

// primitive function for cdr
Value *primitiveCdr(Value *args){

    if (isNull(args) || isNull(car(args))) {
        printf("Evaluation error: no argument supplied to cdr\n");
        texit(1);
    }

    return cdr(car(args));

}

// primitive function for car
Value *primitiveCar(Value *args){
    
    if (isNull(args) || isNull(car(args))){
        printf("Evaluation error: no argument supplied to car\n");
        texit(1);
    }
    if(car(args)->type != CONS_TYPE){
        printf("Evaluation error: incorrect argument type supplied to car\n");
        texit(1);
    }
    if(!isNull(cdr(args))){
         printf("Evaluation error: too many arguments supplied to car\n");
         texit(1);
    }
    
    return (car(car(args)));
}

// primitive function for -
Value *primitiveMinus(Value *args) {

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to '-'\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to '-'\n");
        texit(1);
    }

    Value* first = car(args);
    Value* second = car(cdr(args));

    // make result Value
    Value *result = talloc(sizeof(Value));

    if (first->type == DOUBLE_TYPE && second->type == DOUBLE_TYPE) {
        result->type = DOUBLE_TYPE;
        result->d = first->d - second->d;
    } else if (first->type == INT_TYPE && second->type == DOUBLE_TYPE) {
        result->type = DOUBLE_TYPE;
        result->d = first->i - second->d;
    } else if (first->type == DOUBLE_TYPE && second->type == INT_TYPE) {
        result->type = DOUBLE_TYPE;
        result->d = first->d - second->i;
    } else if (first->type == INT_TYPE && second->type == INT_TYPE) {
        result->type = INT_TYPE;
        result->i = first->i - second->i;
    } else {
        printf("Evaluation error: '-' has invalid argument(s).\n");
        texit(1);
    }

    return result;
}

// primitive function for <
Value *primitiveSmaller(Value *args) {

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to '<'\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to '<'\n");
        texit(1);
    }

    Value* first = car(args);
    Value* second = car(cdr(args));
    float firstnumber, secondnumber;

    // make result Value
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;

    if (first->type == DOUBLE_TYPE && second->type == DOUBLE_TYPE) {
        firstnumber = first->d;
        secondnumber = second->d;
    } else if (first->type == INT_TYPE && second->type == DOUBLE_TYPE) {
        firstnumber = (float) first->i;
        secondnumber = second->d;
    } else if (first->type == DOUBLE_TYPE && second->type == INT_TYPE) {
        firstnumber = first->d;
        secondnumber = (float) second->i;
    } else if (first->type == INT_TYPE && second->type == INT_TYPE) {
        firstnumber = (float) first->i;
        secondnumber = (float) second->i;
    } else {
        printf("Evaluation error: '<' has invalid argument(s).\n");
        texit(1);
    }

    if (firstnumber < secondnumber) {
        result->s = "#t";
    } else {
        result->s = "#f";
    }
    return result;
}

// primitive function for >
Value *primitiveLarger(Value *args) {

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to '<'\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to '<'\n");
        texit(1);
    }

    Value* first = car(args);
    Value* second = car(cdr(args));
    float firstnumber, secondnumber;

    // make result Value
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;

    if (first->type == DOUBLE_TYPE && second->type == DOUBLE_TYPE) {
        firstnumber = first->d;
        secondnumber = second->d;
    } else if (first->type == INT_TYPE && second->type == DOUBLE_TYPE) {
        firstnumber = (float) first->i;
        secondnumber = second->d;
    } else if (first->type == DOUBLE_TYPE && second->type == INT_TYPE) {
        firstnumber = first->d;
        secondnumber = (float) second->i;
    } else if (first->type == INT_TYPE && second->type == INT_TYPE) {
        firstnumber = (float) first->i;
        secondnumber = (float) second->i;
    } else {
        printf("Evaluation error: '<' has invalid argument(s).\n");
        texit(1);
    }

    if (firstnumber > secondnumber) {
        result->s = "#t";
    } else {
        result->s = "#f";
    }
    return result;
}

// primitive function for =
Value *primitiveEqual(Value *args) {

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to '<'\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to '<'\n");
        texit(1);
    }

    Value* first = car(args);
    Value* second = car(cdr(args));
    float firstnumber, secondnumber;

    // make result Value
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;

    if (first->type == DOUBLE_TYPE && second->type == DOUBLE_TYPE) {
        firstnumber = first->d;
        secondnumber = second->d;
    } else if (first->type == INT_TYPE && second->type == DOUBLE_TYPE) {
        firstnumber = (float) first->i;
        secondnumber = second->d;
    } else if (first->type == DOUBLE_TYPE && second->type == INT_TYPE) {
        firstnumber = first->d;
        secondnumber = (float) second->i;
    } else if (first->type == INT_TYPE && second->type == INT_TYPE) {
        firstnumber = (float) first->i;
        secondnumber = (float) second->i;
    } else {
        printf("Evaluation error: '<' has invalid argument(s).\n");
        texit(1);
    }

    if (firstnumber == secondnumber) {
        result->s = "#t";
    } else {
        result->s = "#f";
    }
    return result;
}

// primitive function for *
Value *primitiveMultiply(Value *args) {
    
    int isDouble = 0;
    float product = 1;

    // loop throgh args
    while (!isNull(args)) {
        Value *cur = car(args);
        if (cur->type == DOUBLE_TYPE) {
            isDouble = 1;
            product *= cur->d;
        } else if (cur->type == INT_TYPE) {
            product *= cur->i;
        } else {
            printf("Evaluation error: '+' has invalid argument(s).\n");
            texit(1);
        }
        args = cdr(args);
    }
    
    // make result Value
    Value *result = talloc(sizeof(Value));
    if (isDouble) {
        result->type = DOUBLE_TYPE;
        result->d = product;
    } else {
        result->type = INT_TYPE;
        result->i = (int) product;
    }
    return result;
}

// primitive function for /
Value *primitiveDivide(Value *args) {

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to '/'\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to '/'\n");
        texit(1);
    }

    Value* first = car(args);
    Value* second = car(cdr(args));

    // make result Value
    Value *result = talloc(sizeof(Value));

    if (first->type == DOUBLE_TYPE && second->type == DOUBLE_TYPE) {
        result->type = DOUBLE_TYPE;
        result->d = first->d / second->d;
    } else if (first->type == INT_TYPE && second->type == DOUBLE_TYPE) {
        result->type = DOUBLE_TYPE;
        result->d = first->i / second->d;
    } else if (first->type == DOUBLE_TYPE && second->type == INT_TYPE) {
        result->type = DOUBLE_TYPE;
        result->d = first->d / second->i;
    } else if (first->type == INT_TYPE && second->type == INT_TYPE) {
        if (first->i % second->i == 0) {
            result->type = INT_TYPE;
            result->i = first->i / second->i;
        } else {
            result->type = DOUBLE_TYPE;
            result->d = (float) first->i / second->i;
        }
    } else {
        printf("Evaluation error: '/' has invalid argument(s).\n");
        texit(1);
    }

    return result;
}

// primitive function for modulo
Value *primitiveModulo(Value *args) {

    if (isNull(args) || isNull(car(args)) || isNull(cdr(args))){
        printf("Evaluation error: insufficient amount of arguments supplied to 'modulo'\n");
        texit(1);
    }

    if (!isNull(cdr(cdr(args)))){
        printf("Evaluation error: too many arguments supplied to 'modulo'\n");
        texit(1);
    }

    Value* first = car(args);
    Value* second = car(cdr(args));

    // make result Value
    Value *result = talloc(sizeof(Value));

    if (first->type == INT_TYPE && second->type == INT_TYPE) {
        result->type = INT_TYPE;
        result->i = first->i % second->i;
    } else {
        printf("Evaluation error: '/' has invalid argument(s).\n");
        texit(1);
    }

    return result;
}

// add the symbol-primitive binding to frame
void bind(char *name, Value *(*function)(struct Value *), Frame *frame) {
    // Add primitive functions to top-level bindings list
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;

    Value *symbol = talloc(sizeof(Value));
    symbol->type = SYMBOL_TYPE;
    symbol->s = talloc(strlen(name) + 1);
    strcpy(symbol->s, name);

    Value* binding = cons(symbol, value);

    frame->bindings = cons(binding, frame->bindings);

    return;
}


// SPECIAL FORMS

// eval quote
Value *evalQuote(Value *args, Frame *frame) {
    
    if (isNull(args)) {
        printf("Evaluation error: 'quote' has no argument.\n");
        texit(1);
    }
    
    if (!isNull(cdr(args))) {
        printf("Evaluation error: 'quote' has more than 1 argument.\n");
        texit(1);
    }

    return car(args);
}

// eval define: modifies the current environment frame.
Value *evalDefine(Value *args, Frame *frame) {

    // error checking: # of args
    Value *curr = args;
    int count = 0;
    while (!isNull(curr)){
        count++;
        if (count > 2) {
            printf("Evaluation error: 'define' has more than 2 arguments.\n");
            texit(1);
        }
        curr = cdr(curr);
    }
    if (count < 2) {
        printf("Evaluation error: 'define' has less than 2 arguments.\n");
        texit(1);
    }

    // error checking: argument format
    Value *variable = car(args);
    Value *value = car(cdr(args));
    if (variable->type != SYMBOL_TYPE) {
        printf("Evaluation error: invalid type in 'define' arguments.\n");
        texit(1);
    }

    // make binding
    Value *binding = cons(variable, eval(value, frame));

    // insert binding
    frame->bindings = cons(binding, frame->bindings);

    Value *result = talloc(sizeof(Value));
    result->type = VOID_TYPE;
    return result;
}

// create a closure and return it
Value *evalLambda(Value *args, Frame *frame) {

    // error checking: # of args
    Value *curr = args;
    int count = 0;
    while (!isNull(curr)){
        count++;
        if (count > 2) {
            printf("Evaluation error: 'lambda' has more than 2 arguments.\n");
            texit(1);
        }
        curr = cdr(curr);
    }
    if (count < 2) {
        printf("Evaluation error: 'lambda' has less than 2 arguments.\n");
        texit(1);
    }

    // checking params validity
    Value *params = car(args);
    curr = params;
    while (!isNull(curr)) {
        if (car(curr)->type != SYMBOL_TYPE) {
            printf("Evaluation error: 'lambda' has invalid params.\n");
            texit(1);
        }
        Value* next = cdr(curr);
        while (!isNull(next)) {
            if (!strcmp(car(curr)->s, car(next)->s)) {
                printf("Evaluation error: 'lambda' has duplicate params.\n");
                texit(1);
            }
            next = cdr(next);
        }
        curr = cdr(curr);
    }

    // create closure
    Value *closure = talloc(sizeof(Value));
    closure->type = CLOSURE_TYPE;
    closure->cl.paramNames = params;
    closure->cl.functionCode = car(cdr(args));
    closure->cl.frame = frame;

    return closure;
}

// eval all arguments for special form functions
Value *evalEach(Value *args, Frame *frame) {
    
    Value *evaluated = makeNull();
    while (!isNull(args)) {
        evaluated = cons(eval(car(args), frame), evaluated);
        args = cdr(args);
    }
    evaluated = reverse(evaluated);
    return evaluated;
}

// apply the special form funtion to the evaluated args
Value *apply(Value *function, Value *args){

    if (function->type == PRIMITIVE_TYPE) { 
        return function->pf(args);
    }
    
    if (function->type != CLOSURE_TYPE) {
        printf("Evaluation error: first expression in a parens is not a function.\n");
        texit(1);
    }

    Frame *frame = talloc(sizeof(Frame));
    frame->parent = function->cl.frame;
    frame->bindings = makeNull();

    Value *func_args = function->cl.paramNames;

    // match arguments and put into frame
    while (!isNull(func_args) && !isNull(args)) {
        Value *binding = cons(car(func_args), car(args));
        frame->bindings = cons(binding, frame->bindings);
        func_args = cdr(func_args);
        args = cdr(args);
    }

    // error checking: # of args
    if (!isNull(func_args) || !isNull(args)) {
        printf("Evaluation error: wrong number of arguments for function.\n");
        texit(1);
    };

    Value *result = eval(function->cl.functionCode, frame);
    return result;
}

// evaluate if function
Value *evalIf(Value *args, Frame *frame) {
    // check # of args
    Value *checkif = args;
    for (int i = 0; i < 3; i++) {
        if (isNull(checkif)){
            printf("Evaluation error: 'if' passed fewer than 3 arguments.\n");
            texit(1);
        }
        checkif = cdr(checkif);
    }

    Value *evalValue = eval(car(args), frame); // first argument
    if (!strcmp(evalValue->s, "#t")) {
        return eval(car(cdr(args)), frame); // second argument
    }
    return eval(car(cdr(cdr(args))), frame); // third argument
}

// find Value of the symol in all the frames, return most recent match
Value *lookUpSymbol(Value *tree, Frame *frame) {
    Frame *currFrame = frame;
    while(currFrame != NULL) { // check all layers of frame
        Value *currBindings = currFrame->bindings;
        while(!isNull(currBindings)) { /// check all bindings in a given frame
            if(!strcmp(car(car(currBindings))->s, tree->s)){
                return cdr(car(currBindings));
            }
            currBindings = cdr(currBindings);
        }
        currFrame = currFrame->parent;
    }
    printf("Evaluation error: symbol '%s' unbound.\n", tree->s);
    texit(1); 
    return NULL;
}

// binding variables and put the binding into the frame
Value *evalLet(Value *args, Frame *frame){
    
    Frame *e = frame;
    Frame *f = talloc(sizeof(Frame));
    f->parent = e;
    f->bindings = makeNull();
    Value *pairs = car(args);
    
    // make bindings
    while(!isNull(pairs)) {

        if(pairs->type != CONS_TYPE) {
            printf("Evaluation error: let second arg has no parens.\n");
            texit(1);
        }

        if(car(pairs)->type != CONS_TYPE){
            printf("Evaluation error: let variables binding not in pairs.\n");
            texit(1);
        }

        Value *pair = car(pairs); // symbol-val pair
        Value *symbol = car(pair);
        Value *expression = car(cdr(pair));

        if(symbol->type != SYMBOL_TYPE){
            printf("Evaluation error: in binding, first token is not symbol.\n");
            texit(1);
        }
        if(isNull(expression)){
            printf("Evaluation error: in binding, second token is of NULL_TYPE.\n");
            texit(1);
        }

        // check duplicate symbol in current frame
        Value *currentBindings = f->bindings;
        while (!isNull(currentBindings)) {
            if(!strcmp(symbol->s, car(car(currentBindings))->s)) {
                printf("Evaluation error: attempt to bind symbol twice.\n");
                texit(1);
            }
            currentBindings = cdr(currentBindings);
        }

        // make binding
        Value *binding = cons(symbol, eval(expression, e));

        // insert binding
        f->bindings = cons(binding, f->bindings);

        pairs = cdr(pairs);
    }

    
    Value *lastExpression = cdr(args);
    if (isNull(lastExpression)) { // if there is no body
        printf("Evaluation error: no body arg in let expression.\n");
        texit(1);
    }
    // locate the last expression
    while(!isNull(cdr(lastExpression)) && car(cdr(lastExpression))->type != NULL_TYPE){
        eval(car(lastExpression), f);
        lastExpression = cdr(lastExpression);
    } 

    return eval(car(lastExpression), f);

}

// eval let* : creating a new frame for each binding.
Value *evalLetstar(Value *args, Frame *frame) {
    Value *pairs = car(args);
    Frame *parent = frame;

    while (!isNull(pairs)) {

        if(pairs->type != CONS_TYPE) {
            printf("Evaluation error: let* second arg has no parens.\n");
            texit(1);
        }

        if(car(pairs)->type != CONS_TYPE){
            printf("Evaluation error: let* variables binding not in pairs.\n");
            texit(1);
        }

        Value *pair = car(pairs); // symbol-val pair
        Value *symbol = car(pair);
        Value *expression = car(cdr(pair));
        
        if(symbol->type != SYMBOL_TYPE){
            printf("Evaluation error: in binding, first token is not symbol.\n");
            texit(1);
        }

        if(isNull(expression)){
            printf("Evaluation error: in binding, second token is of NULL_TYPE.\n");
            texit(1);
        }

        Frame *current = talloc(sizeof(Frame));
        current->parent = parent;
        current->bindings = makeNull();

        // make binding
        Value *binding = cons(symbol, eval(expression, parent));

        // insert binding
        current->bindings = cons(binding, current->bindings);

        parent = current;
        pairs = cdr(pairs);
    }

    Value *lastExpression = cdr(args);
    if (isNull(lastExpression)) { // if there is no body
        printf("Evaluation error: no body arg in let expression.\n");
        texit(1);
    }
    // locate the last expression
    while(!isNull(cdr(lastExpression)) && car(cdr(lastExpression))->type != NULL_TYPE){
        eval(car(lastExpression), parent);
        lastExpression = cdr(lastExpression);
    } 

    return eval(car(lastExpression), parent);
}

// eval letrec
Value *evalLetrec(Value *args, Frame *frame) {
    // Create a new frame env’ with parent env.
    Frame *env = talloc(sizeof(Frame));
    env->parent = frame;
    env->bindings = makeNull();
    
    // Create each of the bindings, and set them to UNSPECIFIED_TYPE (this is in value.h).
    Value *pairs = car(args);
    // make bindings
    while(!isNull(pairs)) {

        if(pairs->type != CONS_TYPE) {
            printf("Evaluation error: let second arg has no parens.\n");
            texit(1);
        }

        if(car(pairs)->type != CONS_TYPE){
            printf("Evaluation error: let variables binding not in pairs.\n");
            texit(1);
        }

        Value *pair = car(pairs); // symbol-val pair
        Value *symbol = car(pair);
        Value *expression = car(cdr(pair));

        if(symbol->type != SYMBOL_TYPE){
            printf("Evaluation error: in binding, first token is not symbol.\n");
            texit(1);
        }
        if(isNull(expression)){
            printf("Evaluation error: in binding, second token is of NULL_TYPE.\n");
            texit(1);
        }

        // check duplicate symbol in current frame
        Value *currentBindings = env->bindings;
        while (!isNull(currentBindings)) {
            if(!strcmp(symbol->s, car(car(currentBindings))->s)) {
                printf("Evaluation error: attempt to bind symbol twice.\n");
                texit(1);
            }
            currentBindings = cdr(currentBindings);
        }

        // make binding
        Value* unevaled = talloc(sizeof(Value));
        unevaled->type = UNSPECIFIED_TYPE;
        unevaled->p = expression;
        Value *binding = cons(symbol, unevaled);

        // insert binding
        env->bindings = cons(binding, env->bindings);

        pairs = cdr(pairs);
    }
    
    
    // Evaluate each of e1, …, en in environment env’. If any of those evaluations use anything with UNSPECIFIED_TYPE, that should result in an error.
    Value* bindings = env->bindings;
    Value* newbindings = makeNull();
    while (!isNull(bindings)) {
        Value *expression = cdr(car(bindings))->p;
        Value *evaluated = eval(expression, env);
        if(evaluated->type == UNSPECIFIED_TYPE) {
            printf("Evaluation error: 'letrec' unspecified args.\n");
            texit(1);
        }
        // make new binding
        Value* binding = cons(car(car(bindings)), evaluated);
        newbindings = cons(binding, newbindings);

        bindings = cdr(bindings);
    }
    
    // After all of these evaluations are complete, replace bindings for each xi with the evaluated result of ei (from step 2) in environment env’.
    env->bindings = newbindings;
    
    //Evaluate body1, …, bodyn sequentially in env’, returning the result of evaluating bodyn.
    Value *lastExpression = cdr(args);
    if (isNull(lastExpression)) { // if there is no body
        printf("Evaluation error: no body arg in let expression.\n");
        texit(1);
    }
    // locate the last expression
    while(!isNull(cdr(lastExpression)) && car(cdr(lastExpression))->type != NULL_TYPE){
        eval(car(lastExpression), env);
        lastExpression = cdr(lastExpression);
    } 

    return eval(car(lastExpression), env);
}

// eval set!
Value *setBang(Value *args, Frame *f){
    
    Frame *currFrame = f;
    while(currFrame != NULL) { // check all layers of frame
        Value *currBindings = currFrame->bindings;
        while(!isNull(currBindings)) { /// check all bindings in a given frame
            if(!strcmp(car(car(currBindings))->s, car(args)->s)){
                Value* binding = car(currBindings);
                binding->c.cdr = eval(car(cdr(args)), f);
                //printValue(binding);

                Value* result = talloc(sizeof(Value));
                result->type = VOID_TYPE;
                return result;
            }
            currBindings = cdr(currBindings);
        }
        currFrame = currFrame->parent;
    }

    printf("Evaluation error: symbol not found. \n");
    texit(1);
    return NULL;
}

// eval begin
Value *evalBegin(Value *args, Frame *f){

    Value *lastExpression = args;
    if (isNull(lastExpression)) { // if there is no body
        Value* result = talloc(sizeof(Value));
        result->type = VOID_TYPE;
        return result;
    }
    
    // locate the last expression
    while(!isNull(cdr(lastExpression)) && car(cdr(lastExpression))->type != NULL_TYPE){
        eval(car(lastExpression), f);
        lastExpression = cdr(lastExpression);
    }
    return eval(car(lastExpression), f);

}

// eval and
Value *evalAnd(Value *args, Frame *f){

    Value *boole = talloc(sizeof(Value));
    boole->type = BOOL_TYPE;
    boole->s = "#t";

    while(!isNull(args) && !isNull(car(args))){
        Value *evaled = eval(car(args), f);
        if(evaled->type == BOOL_TYPE && strcmp(evaled->s, "#t")){
            boole->s = "#f";
            return boole;
        }
        args = cdr(args);
    }
    
    return boole;
}

// eval or
Value *evalOr(Value *args, Frame *f){

    Value *boole = talloc(sizeof(Value));
    boole->type = BOOL_TYPE;
    boole->s = "#f";

    while(!isNull(args) && !isNull(car(args))){
        Value *evaled = eval(car(args), f);
        if(evaled->type == BOOL_TYPE && !strcmp(evaled->s, "#t")){
            boole->s = "#t";
            return boole;
        }
        args = cdr(args);
    }
    
    return boole;
}

// eval cond
Value *evalCond(Value *args, Frame *f) {
    
    while (!isNull(args)) {
        Value *condition = car(car(args));
        Value *expression = car(cdr(car(args)));

        if (condition->type == SYMBOL_TYPE && !strcmp(condition->s, "else")) {
            return eval(expression, f);
        }

        Value* evaledcond = eval(condition, f);
        if (evaledcond->type == BOOL_TYPE && !strcmp(evaledcond->s, "#t")) {
            return eval(expression, f);
        }
        args = cdr(args);
    }
    // if nothing to return
    Value* result = talloc(sizeof(Value));
    result->type = VOID_TYPE;
    return result;
}

// print error massage and exit program nicely
void evaluationError() {
    printf("Evaluation error: Unspecified. \n");
    texit(1);
    return;
}


// It is a thin wrapper that calls eval for each top-level S-expression in the program.
// It prints out any necessary results before moving on to the next S-expression.
void interpret(Value *tree) {

    // initialize frame
    Frame *f = talloc(sizeof(Frame));
    f->parent = NULL;
    f->bindings = makeNull();

    bind("+", primitiveAdd, f);
    bind("cons", primitiveCons, f);
    bind("car", primitiveCar, f);
    bind("cdr", primitiveCdr, f);
    bind("null?", primitiveNull, f);
    bind("-", primitiveMinus, f);
    bind("<", primitiveSmaller, f);
    bind(">", primitiveLarger, f);
    bind("=", primitiveEqual, f);
    bind("modulo", primitiveModulo, f);
    bind("/", primitiveDivide, f);
    bind("*", primitiveMultiply, f);


    while(!isNull(tree)){

        Value *evaluated = eval(car(tree), f);

        printValue(evaluated);

        tree = cdr(tree);
    }
}


// Given one expression tree and a frame in which to evaluate that expression, 
// eval returns the Value of the expression.
Value *eval(Value *tree, Frame *frame) {
    Value *result;

    switch (tree->type) {
        case INT_TYPE:
            result = tree;
            break;

        case DOUBLE_TYPE:
            result = tree;
            break;

        case BOOL_TYPE:
            result = tree;
            break;

        case STR_TYPE:
            result = tree;
            break;

        case SYMBOL_TYPE:
            result = lookUpSymbol(tree, frame);
            break;
        
        case CLOSURE_TYPE:
            result = tree;
        
        case UNSPECIFIED_TYPE:
            printf("Evaluation error: 'letrec' unspecified symbol.\n");
            texit(1);


        case CONS_TYPE: {
            Value *first = car(tree);
            Value *args = cdr(tree);

            // first symbol can't be null
            if (isNull(first)) {
                printf("Evaluation error: first expression can't be null.\n");
                texit(1);
            }

            if (!strcmp(first->s,"if")) { // if
                result = evalIf(args, frame); 
            }
            else if (!strcmp(first->s,"let")) { // let
                result = evalLet(args, frame);
            }
            else if (!strcmp(first->s,"quote")) { //quote
                result = evalQuote(args, frame);
            }
            else if (!strcmp(first->s, "define")) { //define
                result = evalDefine(args, frame);
            } 
            else if (!strcmp(first->s, "lambda")) { //lambda
                result = evalLambda(args, frame);
            } 
            else if (!strcmp(first->s, "let*")) {
                result = evalLetstar(args, frame);
            }
            else if (!strcmp(first->s, "letrec")) {
                result = evalLetrec(args, frame);
            }
            else if (!strcmp(first->s, "set!")) {
                result = setBang(args, frame);
            }
            else if (!strcmp(first->s, "begin")) {
                result = evalBegin(args, frame);
            }
            else if (!strcmp(first->s, "and")) {
                result = evalAnd(args, frame);
            }
            else if (!strcmp(first->s, "or")) {
                result = evalOr(args, frame);
            }
            else if (!strcmp(first->s, "cond")) {
                result = evalCond(args, frame);
            }
            else {
                // If not a special form, evaluate the first, evaluate the args, then
                // apply the first to the args.
                Value *evaledOperator = eval(first, frame);
                Value *evaledArgs = evalEach(args, frame);

                return apply(evaledOperator,evaledArgs);
            }
            break;
        }

        default:
            printf("Ooooops!");
            break;
    }

    return result;
    
}