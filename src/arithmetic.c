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

void free_node(node *n) {
	if(n == NULL) return;
	free_unbounded_int(&n->operand);
	free_node(n->left);
	free_node(n->right);
	free(n);
}

void free_tree(tree *t) {
	free_node(t->root);
	free_unbounded_int(t->result);
	free(t);
}
