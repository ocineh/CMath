#include <stdlib.h>
#include "arithmetic.h"
#include "strings.h"

typedef enum operator {
	ADD, SUB, MUL, POW, DIV, MOD, NONE
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
	node *n = calloc(1, sizeof(node));
	if(n == NULL) return NULL;
	n->operator = op;
	n->left = left;
	n->right = right;
	return n;
}

static node *value_to_node(unbounded_int value) {
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

static operator char_to_operator(char c) {
	switch(c) {
		case '+': return ADD;
		case '-': return SUB;
		case '*': return MUL;
		case '/': return DIV;
		case '^': return POW;
		case '%': return MOD;
		default: return NONE;
	}
}

#define is_operator(c) ((c) == '+' || (c) == '-' || (c) == '*' || (c) == '/' || (c) == '^' || (c) == '%')

static bool is_unary_operator(const char *s, size_t i) {
	return strchr("+-", *s) && (i == 0 || is_operator(s[i - 1])) && isdigit(s[i + 1]);
}

static char *next_operator(char *s, char c) {
	// We need this to handle unary operators.
	for(size_t i = 0, len = strlen(s); i < len; ++i)
		if(s[i] == c && !(is_unary_operator(s, i)))
			return s + i;
	return NULL;
}

#define max(a, b) ((a) > (b) ? (a) : (b))

static node *string_to_node(char *str) {
	str = strip(str);
	// The more deep is a node, the higher is its priority.

	char *pos;
	if((pos = next_operator(str, '+')) == NULL && (pos = next_operator(str, '-')) == NULL) {
		// The *, / and % operators have the same priority then we need to search for the far near one.
		char *pos_mul = next_operator(str, '*');
		char *pos_div = next_operator(str, '/');
		char *pos_mod = next_operator(str, '%');
		bool mul = pos_mul != NULL, div = pos_div != NULL, mod = pos_mod != NULL;

		// If only one of these operators is found, we can just use it.
		if((mul && !div && !mod) || (!mul && div && !mod) || (!mul && !div && mod))
			pos = mul ? pos_mul : (div ? pos_div : pos_mod);
			// If both of these operators are found, we need to find the one the more far away.
		else if(mul && div && !mod)
			pos = max(pos_mul, pos_div);
		else if(mul && !div && mod)
			pos = max(pos_mul, pos_mod);
		else if(!mul && div && mod)
			pos = max(pos_div, pos_mod);
			// If the three operators are found, we need to find the one the more far away.
		else if(mul && div && mod)
			pos = max(pos_mul, max(pos_div, pos_mod));
			// If none of these operators are found, we search for the ^ operator.
		else if((pos = next_operator(str, '^')) == NULL) {
			// If there is no operator apart the unary one, we can just try to parse the string as a value.
			unbounded_int value = string2unbounded_int(str);
			free(str);
			return value_to_node(value);
		}
	}

	// We found an operator, we need to recursively parse the left and right subtrees.
	operator op = char_to_operator(*pos);
	*pos = '\0';
	char *left = strip(str);
	char *right = strip(pos + 1);

	node *left_node = string_to_node(left);
	node *right_node = string_to_node(right);

	free(str);
	free(left);
	free(right);
	return operator_to_node(op, left_node, right_node);
}

tree *string_to_tree(char *str) {
	if(str == NULL) return NULL;
	str = strip(str);
	// Remove the spaces for simplifying the parsing.
	char *tmp = remove_spaces(str);
	free(str);
	str = tmp;
	if(*str == '\0') return NULL;

	tree *t = malloc(sizeof(tree));
	if(t == NULL) {
		free(str);
		return NULL;
	}

	t->root = string_to_node(str);
	if(t->root == NULL) {
		free(t);
		free(str);
		return NULL;
	}
	free(str);
	return t;
}

static char *operator_to_string(operator op) {
	switch(op) {
		case ADD: return "+";
		case SUB: return "-";
		case MUL: return "*";
		case DIV: return "/";
		case MOD: return "%";
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
	char *res = concat(left, " ", operator_to_string(n->operator), " ", right);
	free(left);
	free(right);
	return res;
}

char *tree_to_string(tree *t) {
	if(t == NULL) return "NULL";
	return node_to_string(t->root);
}

static unbounded_int (*operator_to_function(operator op))(unbounded_int, unbounded_int) {
	switch(op) {
		case ADD: return unbounded_int_somme;
		case SUB: return unbounded_int_difference;
		case MUL: return unbounded_int_produit;
		case DIV: return unbounded_int_quotient;
		case MOD: return unbounded_int_modulo;
		case POW: return unbounded_int_pow;
		default: return NULL;
	}
}

static node *evaluate_leaf(node *n) {
	if(n->operator != NONE && n->left->operator == NONE && n->right->operator == NONE) {
		unbounded_int res = operator_to_function(n->operator)(n->left->operand, n->right->operand);
		free_node(n);
		return value_to_node(res);
	}
	return n;
}

static node *evaluate_complex_node(node *n) {
	if(n->operator == NONE) return n;

	if(n->right->operator != NONE) {
		node *succesor = n->right, *to_return = n->right;
		while(succesor->operator != NONE) succesor = succesor->left;

		unbounded_int res = operator_to_function(n->operator)(n->left->operand, succesor->operand);
		n->right = NULL;
		free_node(n);
		free_unbounded_int(&succesor->operand);
		succesor->operand = res;
		return to_return;
	}

	return evaluate_leaf(n);
}

static node *evaluate_pow(node *n) {
	if(n->operator != NONE) n->left = evaluate_pow(n->left);
	return n->operator == POW ? evaluate_complex_node(n) : n;
}

static node *evaluate_mul_div_mod(node *n) {
	if(n->operator != NONE) n->left = evaluate_mul_div_mod(n->left);
	if(n->operator == MUL || n->operator == DIV || n->operator == MOD)
		return evaluate_mul_div_mod(evaluate_complex_node(n));

	if(n->operator != NONE) n->right = evaluate_mul_div_mod(n->right);
	return n;
}

static node *evaluate_add_sub(node *n) {
	if(n->operator != NONE) n->left = evaluate_add_sub(n->left);
	if(n->operator == ADD || n->operator == SUB)
		return evaluate_add_sub(evaluate_complex_node(n));

	if(n->operator != NONE) n->right = evaluate_add_sub(n->right);
	return n;
}

static node *evaluate_node(node *n) {
	if(n == NULL) return NULL;
	if(n->operator == NONE) return n;
	n = evaluate_pow(n);
	n = evaluate_mul_div_mod(n);
	n = evaluate_add_sub(n);
	return n;
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
	if(n->operator == MOD) return concat("MOD");
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
