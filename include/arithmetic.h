#ifndef PROJET_ARITHMETIC_H
#define PROJET_ARITHMETIC_H

#include "unbounded_int.h"

/**
 * Represents a arithmetic expression in the form of a tree.
 * Each node is either an operator with two children or a number.
 */
typedef struct tree tree;

/**
 * Create a new tree from a string.
 *
 * @param str a string representing an arithmetic expression
 * @return a new tree or NULL if it failed
 */
extern tree *string_to_tree(char *str);

/**
 * Transform a tree into an arithmetic expression.
 *
 * @param t a tree
 * @return a string
 */
extern char *tree_to_string(tree *t);

/**
 * Evaluate all the nodes of a tree until it remains only one with a value.
 *
 * @param t a tree
 */
extern void evaluate(tree *t);

/**
 * Free a tree and all its nodes recursively.
 *
 * @param t a tree
 */
extern void free_tree(tree *t);

/**
 * Calculate the number of nodes in a tree.
 *
 * @param t a tree
 * @return the number of nodes
 */
extern size_t tree_size(tree *t);

/**
 * Calculate the depth of a tree.
 *
 * @param t a tree
 * @return the depth of the tree
 */
extern size_t tree_depth(tree *t);

#endif //PROJET_ARITHMETIC_H
