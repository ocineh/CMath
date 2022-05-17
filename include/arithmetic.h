#ifndef PROJET_ARITHMETIC_H
#define PROJET_ARITHMETIC_H

#include "unbounded_int.h"

/**
 * Represents a arithmetic expression in the form of a list.
 */
typedef struct arithmetic arithmetic;

/**
 * Create a new tree from a string.
 *
 * @param str a string representing an arithmetic expression
 * @return a new tree or NULL if it failed
 */
extern arithmetic *string_to_arithmetic(char *str);

/**
 * Transform a tree into an arithmetic expression.
 *
 * @param t an arithmetic list
 * @return a string
 */
extern char *arithmetic_to_string(arithmetic *a);

/**
 * Evaluate all the nodes of a tree until it remains only one with a value.
 *
 * @param a an arithmetic list
 */
extern unbounded_int evaluate(arithmetic *a);

/**
 * Free a tree and all its nodes recursively.
 *
 * @param t an arithmetic list
 */
extern void free_arithmetic(arithmetic *a);

#endif //PROJET_ARITHMETIC_H
