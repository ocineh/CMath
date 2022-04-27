#include <malloc.h>
#include "arithmetic.h"

node *operator_to_node(operator op, node *left, node *right) {
	node *n = malloc(sizeof(node));
	n->operator = op;
	n->left = left;
	n->right = right;
	return n;
}

node *value_to_node(unbounded_int value) {
	node *n = malloc(sizeof(node));
	n->operator = NONE;
	n->operand = value;
	n->left = n->right = NULL;
	return n;
}
