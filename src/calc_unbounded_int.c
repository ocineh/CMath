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
	FILE *error;
} interpreter;

static node *create_node(char *name, unbounded_int value) {
	node *n = malloc(sizeof(node));
	if(n == NULL) return NULL;
	n->name = copy(name);
	n->value = value;
	n->next = n->prev = NULL;
	return n;
}

static memory *create_memory() {
	memory *mem = malloc(sizeof(memory));
	if(mem == NULL) return NULL;
	mem->size = 0;
	mem->head = mem->tail = NULL;
	return mem;
}

interpreter *create_interpreter(FILE *input, FILE *output, FILE *error) {
	if(input == NULL) input = stdin;
	if(output == NULL) output = stdout;

	interpreter *interp = malloc(sizeof(interpreter));
	if(interp == NULL) return NULL;
	interp->memory = create_memory();
	interp->input = input;
	interp->output = output;
	interp->error = error;
	if(interp->memory == NULL) {
		destroy_interpreter(interp);
		return NULL;
	}
	return interp;
}

static void destroy_node(node *n) {
	free(n->name);
	free_unbounded_int(&n->value);
	free(n);
}

static void destroy_memory(memory *mem) {
	if(mem == NULL) return;
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
	fclose(interpreter->error);
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
	if(mem == NULL || name == NULL || isNaN(u)) return NULL;
	if(!valid_variable_name(name)) return NULL;
	if(is_assigned(mem, name)) return NULL;

	node *n = create_node(name, u);
	if(n == NULL) return NULL;
	mem->size++;
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
	else {
		char *str = unbounded_int2string(*u);
		fprintf(interpreter->output, "%s = %s\n", name, str);
		free(str);
	}
}

static void find_variable_name(char *s, size_t *begin, size_t *end) {
	*begin = 0;
	while(*s != '\0' && !isalpha(*s)) {
		++s;
		++(*begin);
	}
	*end = *begin;
	while(*s != '\0' && isalnum(*s)) {
		++s;
		++(*end);
	}
}

static char *replace_variable_name_by_value(interpreter *inter, char *str) {
	if(str == NULL || str[0] == '\0') return NULL;

	size_t pos = 0;
	char *result = concat(str);
	while(result[pos] != '\0') {
		size_t begin, end;
		find_variable_name(result, &begin, &end);
		if(result[begin] == '\0') break;
		char *name = substring(result, begin, end);

		unbounded_int *u = value_of(inter->memory, name);
		if(u == NULL) {
			free(name);
			free(result);
			return NULL;
		}
		char *value = unbounded_int2string(*u);

		char *left = substring(result, 0, begin);
		char *right = substring(result, end, strlen(result));

		char *tmp = concat(left, value, right);
		pos = strlen(left) + strlen(value);
		free(left);
		free(right);
		free(name);
		free(value);
		free(result);
		result = tmp;
	}
	return result;
}

unbounded_int eval(interpreter *inter, char *line) {
	char *str = replace_variable_name_by_value(inter, line);
	if(str == NULL || !is_arithmetic_expression(str)) return NaN;
	tree *t = string_to_tree(str);
	unbounded_int value = evaluate(t);
	value = copy_unbounded_int(&value);
	free_tree(t);
	free(str);
	return value;
}

void interpret(interpreter *inter) {
	char buffer[1024], *tmp;
	do {
		fgets(buffer, 1024, inter->input);
		if(is_empty(buffer) || buffer[0] == '#') continue;

		int pos = index_of(buffer, '=');
		if(pos != -1) { // Assignment
			// Parse
			char *name = strip(tmp = substring(buffer, 0, pos));
			free(tmp);
			char *value = strip(tmp = substring(buffer, pos + 1, strlen(buffer)));
			free(tmp);
			if(!valid_variable_name(name)) {
				fprintf(inter->output, "Invalid variable name.\n");
				continue;
			}

			// Evaluate
			unbounded_int u = eval(inter, value);
			if(isNaN(u)) {
				fprintf(inter->output, "Invalid expression.\n");
				continue;
			}

			// Assign
			unbounded_int *v = assign(inter->memory, name, u);
			if(v == NULL) fprintf(inter->output, "Variable %s already exists.\n", name);

			free(name);
			free(value);
		} else { // Print
			pos = index_of(buffer, ' ');
			if(pos != -1) {
				char *command = strip(tmp = substring(buffer, 0, pos));
				free(tmp);
				if(strcmp(command, "print") == 0) {
					char *name = strip(tmp = substring(buffer, pos + 1, strlen(buffer)));
					print(inter, name);
					free(tmp);
					free(name);
				}
				free(command);
			}
		}
	} while(*buffer != EOF && *buffer != '\0' && strcmp(buffer, "exit") != 0);
}
