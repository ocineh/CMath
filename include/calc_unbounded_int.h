#ifndef PROJET_CALC_UNBOUNDED_INT_H
#define PROJET_CALC_UNBOUNDED_INT_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "unbounded_int.h"

/**
 * Interprets instructions from one file and writes the result to another file.
 * If the input file is NULL then stdin is used.
 * If the output file is NULL then stdout is used.
 *
 * @param input the file with the instructions to execute
 * @param output The file to which the result is redirected
 */
extern void interpret(FILE *input, FILE *output);

/**
 * Remove spaces at start and end of the string
 *
 * @param c a string
 */
extern char* strip(char *c);

/**
 * Split the elements of the string with the separator
 *
 * @param line a string
 * @param separator a separator
 * @return an array of strings
 */
extern char **split(char *line, char separator);

/**
 * Checks if the variable name is valid.
 * The equivalent in regex is `[a-zA-Z_][a-zA-Z_à-9]*`
 *
 * @param name the name of a variable
 * @return if the name is valid
 */
extern bool valid_variable_name(char *name);

/**
 * Assign the value with the name of the variable.
 * If the name is already used or the variable name is not valid the operation will fail.
 *
 * @param name the name of the variable
 * @param u the value to assign
 * @return if the operation fails return NULL else a pointer
 */
extern unbounded_int *assign(char *name, unbounded_int *u);

/**
 * Get the value of a variable from its name.
 *
 * @param name the name of a variable
 * @return if the variable exists return its value else NULL
 */
extern unbounded_int *value_of(char *name);

/**
 * Evaluates a string and returns the result of the operations.
 *
 * @param line the operation to be evaluated
 * @return the result of the operation
 */
extern unbounded_int *eval(char *line);

/**
 * Print the value of a variable.
 *
 * @param name the name of a variable
 */
extern void print(char *name, FILE *output);

#endif //PROJET_CALC_UNBOUNDED_INT_H
