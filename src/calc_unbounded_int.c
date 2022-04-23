#include <calc_unbounded_int.h>
#include <stdlib.h>
#include <ctype.h>

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

char *strip(char *c) {
	size_t begin = 0;
	while(isspace(c[begin])) begin++;
	
	size_t end = strlen(c) - 1;
	while(end > 0 && isspace(c[end])) end--;
	
	if(begin >= end) return "";
	size_t len = end - begin + 1;
	char *new = malloc(len);
	memmove(new, c + begin, len);
	return new;
}

static char *extract(char *s, size_t begin, size_t end) {
	size_t len = end - begin + 1;
	char *new = malloc(len);
	memmove(new, s + begin, len);
	return new;
}

size_t split(char *line, char separator, char ***tokens) {
	size_t count = 0, len = strlen(line);
	for(size_t i = 0; i < len; ++i) {
		if(line[i] == separator) {
			size_t j = i + 1;
			while(j < len && line[j] != separator) ++j;
			if(i != j - 1) ++count;
			i = j - 1;
		}
	}
	
	*tokens = malloc(sizeof(char *) * count);
	for(size_t i = 0, j = i, k = 0; i < len && k < count; i = j + 1, j = i) {
		while(line[j] != separator && j < len) j++;
		if(i != j)(*tokens)[k++] = extract(line, i, j - 1);
	}
	return count;
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
	return NULL;
}
