#ifndef PROJET_ARITHMETIC_H
#define PROJET_ARITHMETIC_H

#include "unbounded_int.h"

typedef enum operator { ADD, SUB, MUL, NONE } operator;

typedef struct node {
	operator operator;
	unbounded_int operand;
	struct node *left;
	struct node *right;
} node;

typedef struct tree {
	node *root;
	unbounded_int *result;
} tree;

node *operator_to_node(operator op, node *left, node *right);
node *value_to_node(unbounded_int value);

tree *string_to_tree(char *str);

void evaluate(tree *t);

char *tree_to_string(tree *t);

void free_node(node *n);
void free_tree(tree *t);

#endif //PROJET_ARITHMETIC_H
