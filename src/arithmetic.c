#include <stdlib.h>
#include "arithmetic.h"
#include "strings.h"

typedef enum operator {
	ADD, SUB, MUL, POW, DIV, NONE
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
		case '/': return DIV;
		case '^': return POW;
		default: return NONE;
	}
}

#define is_operator(c) ((c) == '+' || (c) == '-' || (c) == '*' || (c) == '/' || (c) == '^')

static bool is_unary_operator(char *s, size_t i) {
	return (s[i] == '+' || s[i] == '-') && (i == 0 || is_operator(s[i - 1])) && is_digit(s[i + 1]);
}

static bool next_operator(char *s, char c, size_t *index) {
	size_t len = strlen(s);
	for(size_t i = 0; i < len; ++i)
		if(s[i] == c && !(is_unary_operator(s, i))) {
			if(index != NULL) *index = i;
			return true;
		}
	return false;
}

static node *string_to_node(char *str) {
	str = strip(str);
	size_t len = strlen(str);

	size_t pos;
	if(!next_operator(str, '+', &pos) && !next_operator(str, '-', &pos) &&
	   !next_operator(str, '*', &pos) && !next_operator(str, '/', &pos) &&
	   !next_operator(str, '^', &pos)) {
		unbounded_int value = string2unbounded_int(str);
		free(str);
		return value_to_node(value);
	}

	char *left = strip_free(substring(str, 0, (size_t) pos));
	char *right = strip_free(substring(str, (size_t) pos + 1, (size_t) len));

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
	char *tmp = remove_spaces(str);
	free(str);
	str = tmp;
	if(*str == '\0') return NULL;

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
		case DIV: return "/";
		case POW: return "^";
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

static node *evaluate_pow(node *n) {
	if(n->operator == POW) {
		if(n->right->operator == NONE) {
			if(isZERO(n->right->operand) || n->right->operand.signe == '-') {
				free_node(n);
				return value_to_node(string2unbounded_int("1"));
			} else if(isONE(n->right->operand)) {
				node *tmp = n->left;
				n->left = NULL;
				free_node(n);
				return tmp;
			} else if(n->left->operator == NONE) {
				unbounded_int res = unbounded_int_pow(n->left->operand, n->right->operand);
				free_node(n);
				return value_to_node(res);
			}
		}
	}
	return n;
}

static node *evaluate_mul(node *n) {
	if(n->operator == MUL) {
		if(n->left->operator == NONE) {
			if(isONE(n->left->operand)) {
				node *tmp = n->right;
				n->right = NULL;
				free_node(n);
				return tmp;
			} else if(isZERO(n->left->operand)) {
				free_node(n);
				return value_to_node(ZERO);
			}
		}
		if(n->right->operator == NONE) {
			if(isONE(n->right->operand)) {
				node *tmp = n->left;
				n->left = NULL;
				free_node(n);
				return tmp;
			} else if(isZERO(n->right->operand)) {
				free_node(n);
				return value_to_node(ZERO);
			}
		}
		if(n->right->operator == NONE && n->left->operator == NONE) {
			unbounded_int res = unbounded_int_produit(n->left->operand, n->right->operand);
			free_node(n);
			return value_to_node(res);
		}
	}
	return n;
}

static node *evaluate_div(node *n) {
	if(n->operator == DIV) {
		if(n->right->operator == NONE) {
			if(isONE(n->right->operand)) {
				node *tmp = n->left;
				n->left = NULL;
				free_node(n);
				return tmp;
			} else if(isZERO(n->right->operand)) {
				free_node(n);
				return value_to_node(NaN);
			}
		}
		if(n->left->operator == NONE && n->right->operator == NONE) {
			unbounded_int res = unbounded_int_quotient(n->left->operand, n->right->operand);
			free_node(n);
			return value_to_node(res);
		}
	}
	return n;
}

static node *evaluate_add(node *n) {
	if(n->operator == ADD) {
		if(n->left->operator == NONE && n->right->operator == NONE) {
			unbounded_int res = unbounded_int_somme(n->left->operand, n->right->operand);
			free_node(n);
			return value_to_node(res);
		}
	}
	return n;
}

static node *evaluate_sub(node *n) {
	if(n->operator == SUB) {
		if(n->left->operator == NONE && n->right->operator == NONE) {
			n->right->operand.signe = (n->right->operand.signe == '+') ? '-' : '+';
			n->operator = ADD;
			return evaluate_add(n);
		}
	}
	return n;
}

static node *evaluate_node(node *n) {
	if(n == NULL) return NULL;
	if(n->operator != NONE) {
		n->left = evaluate_node(n->left);
		n->right = evaluate_node(n->right);
	}

	switch(n->operator) {
		case ADD: return evaluate_add(n);
		case SUB: return evaluate_sub(n);
		case MUL: return evaluate_mul(n);
		case POW: return evaluate_pow(n);
		case DIV: return evaluate_div(n);
		default: return n;
	}
}

unbounded_int evaluate(tree *t) {
	if(t != NULL && t->root != NULL) {
		t->root = evaluate_node(t->root);
		if(t->root->operator == NONE)
			return copy_unbounded_int(&t->root->operand);
	}
	return NaN;
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

static char *etiquete(node *n) {
	if(n == NULL) return concat("NULL");
	if(n->operator == NONE) return unbounded_int2string(n->operand);
	if(n->operator == ADD) return concat("ADD");
	if(n->operator == SUB) return concat("SUB");
	if(n->operator == MUL) return concat("MUL");
	if(n->operator == DIV) return concat("DIV");
	if(n->operator == POW) return concat("POW");
	return concat("NULL");
}

static size_t find_index(node *tab[], size_t s, node *n) {
	for(size_t i = 0; i < s; ++i)
		if(tab[i] == n) return i;
	return 0; // Not reachable in theory
}

static void node_to_dot(node *n, FILE *f, size_t i, node *tab[], size_t size) {
	if(n == NULL) return;
	if(n->operator == NONE) {
		char *a = etiquete(n), *b = unbounded_int2string(n->operand);
		fprintf(f, "\"%zu-%s\" [label=\"%s\"];\n\n", i, a, b);
		free(a);
		free(b);
	} else {
		char *e_n = etiquete(n), *e_l = etiquete(n->left), *e_r = etiquete(n->right);
		size_t l = find_index(tab, size, n->left), r = find_index(tab, size, n->right);

		fprintf(f, "\"%zu-%s\" [label=\"%s\"];\n", i, e_n, e_n);
		fprintf(f, "\"%zu-%s\" -> \"%zu-%s\";\n", i, e_n, l, e_l);
		fprintf(f, "\"%zu-%s\" -> \"%zu-%s\";\n\n", i, e_n, r, e_r);

		free(e_n);
		free(e_l);
		free(e_r);
	}
}

static void fill_array(node *n, node **tab, size_t *i) {
	if(n == NULL) return;
	tab[*i] = n;
	(*i)++;
	if(n->operator != NONE) {
		fill_array(n->left, tab, i);
		fill_array(n->right, tab, i);
	}
}

void tree_to_dot(tree *t, char *filename) {
	if(t != NULL && t->root != NULL) {
		FILE *f = fopen(filename, "w");
		fprintf(f, "digraph tree {\n");
		fprintf(f, "\tnode [shape=box];\n");

		size_t size = tree_size(t), i = 0;
		node *tab[size];
		fill_array(t->root, tab, &i);
		for(size_t j = 0; j < size; ++j)
			node_to_dot(tab[j], f, j, tab, size);

		fprintf(f, "}\n");
		fclose(f);
	}
}
