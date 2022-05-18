#include <stdlib.h>
#include "arithmetic.h"
#include "strings.h"

typedef enum operator {
	ADD = '+', SUB = '-', MUL = '*', POW = '^', DIV = '/', MOD = '%', NONE = '\0'
} operator;

typedef struct cell {
	operator operator;
	struct cell *next;
	struct cell *prev;
	unbounded_int value;
} cell;

struct arithmetic {
	cell *first;
	cell *last;
};

static cell *operator_to_node(operator op) {
	cell *n = calloc(1, sizeof(cell));
	if(n == NULL) return NULL;
	n->operator = op;
	return n;
}

static cell *value_to_node(unbounded_int value) {
	cell *n = calloc(1, sizeof(cell));
	if(n == NULL) return NULL;
	n->operator = NONE;
	n->value = value;
	return n;
}

static void free_node(cell *n) {
	if(n == NULL) return;
	if(n->operator == NONE)
		free_unbounded_int(&n->value);
	free(n);
}

void free_arithmetic(arithmetic *a) {
	if(a != NULL) {
		while(a->first != NULL) {
			cell *tmp = a->first;
			a->first = a->first->next;
			free_node(tmp);
		}
		free(a);
		a = NULL;
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

#define is_operator(c) (strchr((char[]){ADD, SUB, MUL, DIV, POW, MOD, '\0'}, (c)) != NULL)

static bool is_unary_operator(const char *s, size_t i) {
	return strchr("+-", *s) && (i == 0 || is_operator(s[i - 1])) && isdigit(s[i + 1]);
}

static char *next_operator(char *s) {
	// We need this to handle unary operators.
	for(size_t i = 0, len = strlen(s); i < len; ++i)
		if(is_operator(s[i]) && !(is_unary_operator(s, i)))
			return s + i;
	return NULL;
}

static void add(arithmetic *a, cell *n) {
	if(a->first == NULL) a->first = a->last = n;
	else {
		a->last->next = n;
		n->prev = a->last;
		a->last = n;
	}
}

static void string_to_node(arithmetic *a, char *str) {
	char *p = str = copy(str);
	char *pos;
	while((pos = next_operator(str)) != NULL) {
		operator op = char_to_operator(*pos);

		*pos = '\0';
		unbounded_int u = string2unbounded_int(str);
		cell *n = value_to_node(u);
		add(a, n);
		str = pos + 1;

		n = operator_to_node(op);
		add(a, n);
	}

	if(*str != '\0') {
		unbounded_int u = string2unbounded_int(str);
		cell *n = value_to_node(u);
		add(a, n);
	}
	free(p);
}

arithmetic *string_to_arithmetic(char *str) {
	if(str == NULL) return NULL;
	str = remove_spaces(str);

	arithmetic *a = calloc(1, sizeof(arithmetic));
	if(a == NULL) return NULL;

	string_to_node(a, str);
	free(str);
	return a;
}

static char *operator_to_string(operator op) {
	switch(op) {
		case ADD: return "+";
		case SUB: return "-";
		case MUL: return "*";
		case DIV: return "//";
		case MOD: return "%";
		case POW: return "**";
		default: return "\0";
	}
}

char *arithmetic_to_string(arithmetic *a) {
	if(a == NULL) return "NULL";

	cell *actual = a->first;
	char *tmp, *res = copy(""), *node_str;
	while(actual != NULL) {
		if(actual->operator != NONE) {
			node_str = operator_to_string(actual->operator);
			tmp = concat(res, node_str, " ");
			free(res);
			res = tmp;
		} else {
			node_str = unbounded_int2string(actual->value);
			tmp = concat(res, node_str, " ");
			free(res);
			free(node_str);
			res = tmp;
		}
		actual = actual->next;
	}

	return res;
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

static void evaluate_(arithmetic *a, char *match) {
	cell *actual = a->first;
	while(actual != NULL) {
		if(actual->operator != NONE && strchr(match, actual->operator) != NULL) {
			unbounded_int (*f)(unbounded_int, unbounded_int) = operator_to_function(actual->operator);
			unbounded_int res = f(actual->prev->value, actual->next->value);

			// Collapse the three nodes into one.
			cell *n = value_to_node(res);
			n->prev = actual->prev->prev;
			n->next = actual->next->next;
			if(actual->prev->prev != NULL) actual->prev->prev->next = n;
			else a->first = n;
			if(actual->next->next != NULL) actual->next->next->prev = n;
			else a->last = n;

			// Free the memory of the nodes
			free_node(actual->prev), actual->prev = NULL;
			free_node(actual->next), actual->next = NULL;
			free_node(actual), actual = NULL;

			actual = n;
		}
		actual = actual->next;
	}
}

unbounded_int evaluate(arithmetic *a) {
	if(a != NULL && a->first != NULL) {
		evaluate_(a, (char[]) { POW, '\0' });
		evaluate_(a, (char[]) { MUL, DIV, MOD, '\0' });
		evaluate_(a, (char[]) { ADD, SUB, '\0' });
		if(a->first == a->last)
			return copy_unbounded_int(&a->first->value);
	}
	return NaN;
}
