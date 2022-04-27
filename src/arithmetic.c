#include <malloc.h>
#include "arithmetic.h"
#include "strings.h"

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
	if(t->result != NULL) free_unbounded_int(t->result);
	free(t);
}

static char *strip_free(char *s) {
	char *res = strip(s);
	free(s);
	return res;
}

static operator char_to_operator(char c) {
	switch(c) {
		case '+': return ADD;
		case '-': return SUB;
		case '*': return MUL;
		default: return NONE;
	}
}

static node *string_to_node(char *str) {
	str = strip(str);
	size_t len = strlen(str);
	
	int pos = index_of(str, '+');
	if(pos == -1) pos = index_of(str, '-');
	if(pos == -1) pos = index_of(str, '*');
	
	if(pos == -1) {
		unbounded_int value = string2unbounded_int(str);
		free(str);
		return value_to_node(value);
	}
	
	char *left = strip_free(substring(str, 0, pos));
	char *right = strip_free(substring(str, pos + 1, len));
	if(strlen(left) == 0) {
		free(left);
		free(str);
		unbounded_int value = string2unbounded_int(right);
		free(right);
		return value_to_node(value);
	}
	
	node *left_node = string_to_node(left);
	node *right_node = string_to_node(right);
	
	operator op = char_to_operator(str[pos]);
	free(str);
	free(left);
	free(right);
	return operator_to_node(op, left_node, right_node);
}

tree *string_to_tree(char *str) {
	if(str == NULL) return NULL;
	str = strip(str);
	size_t len = strlen(str);
	if(len == 0) return NULL;
	
	tree *t = malloc(sizeof(tree));
	t->root = string_to_node(str);
	return t;
}

static char *operator_to_char(operator op) {
	switch(op) {
		case ADD: return "+";
		case SUB: return "-";
		case MUL: return "*";
		default: return "\0";
	}
}

static char *node_to_string(node *n) {
	if(n == NULL) return NULL;
	if(n->operator == NONE)
		return concat("(", unbounded_int2string(n->operand), ")");
	
	char *left = node_to_string(n->left);
	char *right = node_to_string(n->right);
	char *res = concat(left, " ", operator_to_char(n->operator), " ", right);
	free(left);
	free(right);
	return res;
}

char *tree_to_string(tree *t) {
	return node_to_string(t->root);
}
