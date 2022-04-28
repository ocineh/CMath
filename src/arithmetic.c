#include <malloc.h>
#include "arithmetic.h"
#include "strings.h"

typedef enum operator {
	ADD, SUB, MUL, NONE
} operator;

typedef struct node {
	operator operator;
	union {
		unbounded_int operand;
		struct {
			struct node *left;
			struct node *right;
		};
	};
} node;

struct tree {
	node *root;
};

static node *operator_to_node(operator op, node *left, node *right) {
	if(left == NULL || right == NULL) return NULL;
	node *n = malloc(sizeof(node));
	if(n == NULL) return NULL;
	n->operator = op;
	n->left = left;
	n->right = right;
	return n;
}

static node *value_to_node(unbounded_int value) {
	if(isNaN(value)) return NULL;
	node *n = malloc(sizeof(node));
	if(n == NULL) return NULL;
	n->operator = NONE;
	n->operand = value;
	return n;
}

static void free_node(node *n) {
	if(n == NULL) return;
	if(n->operator == NONE)
		free_unbounded_int(&n->operand);
	else {
		free_node(n->left);
		free_node(n->right);
	}
	free(n);
}

void free_tree(tree *t) {
	if(t != NULL) {
		if(t->root != NULL) free_node(t->root);
		free(t);
	}
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

	if(pos <= 0) {
		char *tmp = str;
		++tmp;
		while(is_digit(*tmp)) ++tmp;

		int tmp_pos = index_of(tmp, '+');
		if(tmp_pos == -1) tmp_pos = index_of(tmp, '-');
		if(tmp_pos == -1) tmp_pos = index_of(tmp, '*');

		if(tmp_pos == -1) {
			unbounded_int value = string2unbounded_int(str);
			free(str);
			return value_to_node(value);
		}

		char *n = substring(str, 0, (tmp - str) + tmp_pos);
		unbounded_int value = string2unbounded_int(n);
		node *left = value_to_node(value);
		operator op = char_to_operator(tmp[tmp_pos]);
		node *node = operator_to_node(op, left, string_to_node(tmp + tmp_pos + 1));
		free(n);
		free(str);
		return node;
	}

	char *left = strip_free(substring(str, 0, pos));
	char *right = strip_free(substring(str, pos + 1, len));

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
	if(t == NULL) return NULL;
	t->root = string_to_node(str);
	if(t->root == NULL) {
		free(t);
		free(str);
		return NULL;
	}
	free(str);
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
	if(n->operator == NONE) {
		char *str = unbounded_int2string(n->operand);
		char *res = concat("(", str, ")");
		free(str);
		return res;
	}

	char *left = node_to_string(n->left);
	char *right = node_to_string(n->right);
	char *res = concat(left, " ", operator_to_char(n->operator), " ", right);
	free(left);
	free(right);
	return res;
}

char *tree_to_string(tree *t) {
	if(t == NULL) return "NULL";
	return node_to_string(t->root);
}

static node *simplify(node *n) {
	if(n == NULL) return NULL;
	if(n->operator == NONE) return n;

	n->left = simplify(n->left);
	n->right = simplify(n->right);

	if(n->operator == ADD) {
		if(n->left->operator == NONE && isZERO(n->left->operand)) {
			free_node(n->left);
			return n->right;
		}
		if(n->right->operator == NONE && isZERO(n->right->operand)) {
			free_node(n->right);
			return n->left;
		}
	} else if(n->operator == SUB) {
		if(n->right->operator == NONE && isZERO(n->right->operand)) {
			free_node(n->right);
			return n->left;
		}
		if(n->left->operator == NONE && isZERO(n->left->operand)) {
			free_node(n->left);
			n->right->operand.signe == (n->right->operand.signe == '-') ? '+' : '-';
			return n->right;
		}
	} else if(n->operator == MUL){
		if(n->left->operator == NONE && isONE(n->left->operand)) {
			free_node(n->left);
			return n->right;
		}
		if(n->right->operator == NONE && isONE(n->right->operand)) {
			free_node(n->right);
			return n->left;
		}
		if(n->left->operator == NONE && isZERO(n->left->operand)) {
			free_node(n->left);
			free_node(n->right);
			return value_to_node(ZERO);
		}
		if(n->right->operator == NONE && isZERO(n->right->operand)) {
			free_node(n->right);
			free_node(n->left);
			return value_to_node(ZERO);
		}
	}
	return n;
}

static node *evaluate_mul_node(node *n) {
	if(n == NULL) return NULL;

	if(n->operator != NONE) {
		n->left = evaluate_mul_node(n->left);
		n->right = evaluate_mul_node(n->right);
	}

	if(n->operator == MUL) {
		unbounded_int res = unbounded_int_produit(n->left->operand, n->right->operand);
		free_node(n);
		return value_to_node(res);
	}
	return n;
}

static node *evaluate_add_and_sub_node(node *n) {
	if(n == NULL) return NULL;
	if(n->operator == NONE) return n;

	n->left = evaluate_add_and_sub_node(n->left);
	n->right = evaluate_add_and_sub_node(n->right);

	if(n->operator == ADD) {
		unbounded_int res = unbounded_int_somme(n->left->operand, n->right->operand);
		free_node(n);
		return value_to_node(res);
	}
	if(n->operator == SUB) {
		n->right->operand.signe = (n->right->operand.signe == '+') ? '-' : '+';
		unbounded_int res = unbounded_int_somme(n->left->operand, n->right->operand);
		free_node(n);
		return value_to_node(res);
	}
	return n;
}

void evaluate(tree *t) {
	if(t != NULL && t->root != NULL) {
		t->root = simplify(t->root);
		t->root = evaluate_mul_node(t->root);
		t->root = evaluate_add_and_sub_node(t->root);
	}
}

static size_t node_size(node *n) {
	if(n == NULL) return 0;
	if(n->operator == NONE) return 1;
	return 1 + node_size(n->left) + node_size(n->right);
}

size_t tree_size(tree *t) {
	if(t == NULL) return 0;
	return node_size(t->root);
}

static size_t node_depth(node *n) {
	if(n == NULL) return 0;
	if(n->operator == NONE) return 1;
	size_t left = node_depth(n->left);
	size_t right = node_depth(n->right);
	return 1 + ((left > right) ? left : right);
}

size_t tree_depth(tree *t) {
	if(t == NULL) return 0;
	return node_depth(t->root);
}
