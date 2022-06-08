# Scheme Language Interpreter
Author: Walt Li, Eric Gassel  
Interpreter language: C
## Dependencies
- clang
- valgrind (optional for debugging purpose; will not work on Mac)
## How to run
- `make`
- `./interpreter < [you_scheme_filename]`

or use pre-existing tests
- `./test-m` or `./test-e`
## What are implemented?
Special forms:
- quote
- if, cond
- define, set!, let, let*, letrec
- lambda
- begin
- and, or  

Primitives functions:
- car, cdr, cons
- null?
-  +, -, *, /, <, >, =, modulo (numeric types only)

## Known issues and future improvements
- The shorthand for `quote` is not implemented.
- Boolean type data stored as string data in interpreter, could switch into int type instead.
- Symbols/variables lookup is linear O(n), could use more efficient data structure for frames and binidings.
- Garbage collection is simply clearing everything at the end.