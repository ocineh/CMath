#include <calc_unbounded_int.h>
#include <stdlib.h>
#include <ctype.h>
#include "strings.h"
#include "arithmetic.h"

typedef struct node {
	char *name;
	unbounded_int value;
	struct node *next;
	struct node *prev;
} node;

typedef struct memory {
	size_t size;
	node *head;
	node *tail;
} memory;

typedef struct interpreter {
	memory *memory;
	FILE *input;
	FILE *output;
} interpreter;

static node *create_node(char *name, unbounded_int value) {
	node *n = malloc(sizeof(node));
	n->name = name;
	n->value = value;
	n->next = n->prev = NULL;
	return n;
}

static memory *create_memory() {
	memory *mem = malloc(sizeof(memory));
	mem->size = 0;
	mem->head = mem->tail = NULL;
	return mem;
}

interpreter *create_interpreter(FILE *input, FILE *output) {
	if(input == NULL) input = stdin;
	if(output == NULL) output = stdout;

	interpreter *interp = malloc(sizeof(interpreter));
	interp->input = input;
	interp->output = output;
	interp->memory = create_memory();
	return interp;
}

static void destroy_node(node *n) {
	free(n->name);
	free_unbounded_int(&n->value);
	free(n);
}

static void destroy_memory(memory *mem) {
	node *curr = mem->head;
	while(curr != NULL) {
		node *next = curr->next;
		destroy_node(curr);
		curr = next;
	}
	free(mem);
}

void destroy_interpreter(interpreter *interpreter) {
	if(interpreter == NULL) return;
	destroy_memory(interpreter->memory);
	fclose(interpreter->input);
	fclose(interpreter->output);
	free(interpreter);
}

bool valid_variable_name(char *name) {
	if(name == NULL) return false;
	size_t len = strlen(name);
	if(len == 0) return false;

	if(!isalpha(name[0])) return false;
	for(size_t i = 0; i < len; ++i) if(!isalnum(name[i]) && name[i] != '_') return false;
	return true;
}

bool is_assigned(memory *mem, char *name) {
	node *curr = mem->head;
	while(curr != NULL) {
		if(strcmp(curr->name, name) == 0) return true;
		curr = curr->next;
	}
	return false;
}

unbounded_int *assign(memory *mem, char *name, unbounded_int u) {
	if(!valid_variable_name(name)) return NULL;
	if(is_assigned(mem, name)) return NULL;

	node *n = create_node(name, u);
	if(mem->head == NULL)
		mem->head = mem->tail = n;
	else {
		mem->tail->next = n;
		n->prev = mem->tail;
		mem->tail = n;
	}
	return &n->value;
}

unbounded_int *value_of(memory *mem, char *name) {
	node *curr = mem->head;
	while(curr != NULL) {
		if(strcmp(curr->name, name) == 0) return &curr->value;
		curr = curr->next;
	}
	return NULL;
}

void print(interpreter *interpreter, char *name) {
	unbounded_int *u = value_of(interpreter->memory, name);
	if(u == NULL) fprintf(interpreter->output, "Variable %s not found.\n", name);
	else fprintf(interpreter->output, "%s = %s\n", name, unbounded_int2string(*u));
}

unbounded_int eval(char *line) {
	if(!is_arithmetic_expression(line)) return NaN;
	tree *t = string_to_tree(line);
	unbounded_int value = evaluate(t);
	free_tree(t);
	return value;
}
