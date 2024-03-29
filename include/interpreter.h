#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "big_int.h"

typedef struct memory memory;

typedef struct interpreter interpreter;

/**
 * Create a new memory (hash table).
 *
 * @return a pointer to the new memory.
 */
extern memory *create_memory(void);

/**
 * Create a new interpreter with the given file.
 *
 * @param input an input file
 * @param output an output file
 * @param error an error file
 * @return a new interpreter
 */
extern interpreter *create_interpreter(FILE *input, FILE *output, FILE *error);

/**
 * Destroy the given memory and free all the variables.
 *
 * @param mem the memory to destroy.
 */
extern void destroy_memory(memory *mem);

/**
 * Destroy the interpreter and free all the memory used.
 *
 * @param inter the interpreter to destroy (free)
 */
extern void destroy_interpreter(interpreter *inter);

/**
 * Interprets instructions from one file and writes the result to another file.
 * If the input file is NULL then stdin is used.
 * If the output file is NULL then stdout is used.
 *
 * @param input the file with the instructions to execute
 * @param output The file to which the result is redirected
 */
extern void interpret(interpreter *inter);

/**
 * Assign the value with the name of the variable.
 * If the name is already used or the variable name is not valid the operation will fail.
 *
 * @param mem the memory where the variable is stored
 * @param name the name of the variable
 * @param u the value to assign
 * @return if the operation fails return NULL else a pointer
 */
extern big_int *assign(memory *mem, char *name, big_int u);

/**
 * Un-assign the value with the name of the variable.
 * If the name is not used or the variable name is not valid the operation will fail.
 *
 * @param mem the memory where the variable is stored
 * @param name the name of the variable
 * @return true if the operation succeeds otherwise false
 */
extern bool un_assign(memory *mem, char *name);

/**
 * Get the value of a variable from its name.
 *
 * @param mem the memory where the variable is stored
 * @param name the name of a variable
 * @return if the variable exists return its value else NULL
 */
extern big_int *value_of(memory *mem, char *name);

/**
 * Evaluates a string and returns the result of the operations.
 *
 * @param line the operation to be evaluated
 * @return the result of the operation
 */
extern big_int eval(interpreter *inter, char *line);

/**
 * Print the value of a variable.
 *
 * @param inter the interpreter
 * @param name the name of a variable
 */
extern void print(interpreter *inter, char *name);

#endif //INTERPRETER_H
