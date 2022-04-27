#ifndef PROJET_ARITHMETIC_H
#define PROJET_ARITHMETIC_H

#include "unbounded_int.h"

typedef enum operator { ADD, SUB, MUL, NONE } operator;

typedef struct node node;

typedef struct tree tree;

/**
 * Create a new node
 * @param op an operator (ADD, SUB, MUL)
 * @param left another node
 * @param right another node
 * @return a new node
 */
node *operator_to_node(operator op, node *left, node *right);
node *value_to_node(unbounded_int value);

tree *string_to_tree(char *str);
char *tree_to_string(tree *t);

void evaluate(tree *t);

void free_node(node *n);
void free_tree(tree *t);

#endif //PROJET_ARITHMETIC_H
