#include <calc_unbounded_int.h>
#include <stdlib.h>
#include <ctype.h>
#include "strings.h"
#include "arithmetic.h"

#define MAX_LOAD_FACTOR 0.75
#define INITIAL_CAPACITY 8

#define isMarked(mem, i) ((mem)->vars[i] != NULL && (mem)->vars[i]->name == NULL)
#define isUsed(mem, i) ((mem)->vars[i] != NULL && (mem)->vars[i]->name != NULL)
#define isFree(mem, i) ((mem)->vars[i] == NULL)

typedef struct variable {
	char *name;
	unbounded_int value;
} variable;

struct memory {
	size_t size;
	size_t used;
	size_t marked;
	variable **vars;
};

typedef struct interpreter {
	memory *memory;
	FILE *input;
	FILE *output;
	FILE *error;
} interpreter;

static memory *create_memory() {
	memory *mem = malloc(sizeof(memory));
	if(mem == NULL) return NULL;
	mem->size = INITIAL_CAPACITY, mem->used = 0, mem->marked = 0;
	mem->vars = calloc(mem->size, sizeof(variable *));
	return mem;
}

static size_t hash(const char *name, size_t size) {
	if(name == NULL) return 0;
	// djb2 hash function (http://www.cse.yorku.ca/~oz/hash.html)
	size_t hash = 5381, len = strlen(name);
	for(size_t i = 0; i < len; ++i)
		hash = ((hash << 5) + hash) + (size_t) name[i];
	return hash % size;
}

static variable *create_variable(const char *name, unbounded_int value) {
	variable *v = calloc(1, sizeof(variable));
	if(v == NULL) return NULL;
	v->name = copy(name);
	v->value = value;
	return v;
}

static void resize_memory(memory *mem, size_t new_size) {
	if(mem->used > new_size) return;
	variable **vars = calloc(new_size, sizeof(variable *));
	if(vars == NULL) return;

	for(size_t i = 0; i < mem->size; ++i) {
		if(isUsed(mem, i)) {
			size_t h = hash(mem->vars[i]->name, new_size);
			while(vars[h] != NULL) h = (h + 1) % new_size;
			vars[h] = mem->vars[i];
		}
	}

	free(mem->vars);
	mem->vars = vars, mem->size = new_size, mem->marked = 0;
}

interpreter *create_interpreter(FILE *input, FILE *output, FILE *error) {
	if(input == NULL) input = stdin;
	if(output == NULL) output = stdout;
	if(error == NULL) error = stderr;

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

static void destroy_memory(memory *mem) {
	if(mem == NULL) return;
	for(size_t i = 0; i < mem->size; ++i) {
		if(isUsed(mem, i)) {
			free(mem->vars[i]->name);
			free_unbounded_int(&mem->vars[i]->value);
			free(mem->vars[i]);
		}
	}
	free(mem->vars);
	free(mem);
}

void destroy_interpreter(interpreter *inter) {
	if(inter == NULL) return;
	destroy_memory(inter->memory);
	fclose(inter->input);
	fclose(inter->output);
	fclose(inter->error);
	free(inter);
}

bool valid_variable_name(char *name) {
	if(name == NULL) return false;
	size_t len = strlen(name);
	if(len == 0) return false;

	if(!isalpha(name[0])) return false;
	for(size_t i = 1; i < len; ++i)
		if(!isalnum(name[i]) && name[i] != '_')
			return false;
	return true;
}

unbounded_int *assign(memory *mem, char *name, unbounded_int u) {
	if(mem == NULL || name == NULL || isNaN(u))
		return NULL;
	if(!valid_variable_name(name))
		return NULL;

	long long load = (long long) ((long double) mem->size * MAX_LOAD_FACTOR);
	if(mem->used >= load)
		resize_memory(mem, mem->size * 2);
	else if(mem->used + mem->marked >= load)
		resize_memory(mem, mem->size);

	size_t h = hash(name, mem->size);
	while(isUsed(mem, h)) {
		if(strcmp(name, mem->vars[h]->name) == 0) {
			free_unbounded_int(&mem->vars[h]->value);
			mem->vars[h]->value = u;
			return &mem->vars[h]->value;
		}
		h = (h + 1) % mem->size;
	}

	if(isFree(mem, h) || isMarked(mem, h)) {
		mem->vars[h] = create_variable(name, u);
		++mem->used;
		return &mem->vars[h]->value;
	}
	return NULL;
}

unbounded_int *value_of(memory *mem, char *name) {
	size_t h = hash(name, mem->size);
	while(!isFree(mem, h)) {
		if(strcmp(name, mem->vars[h]->name) == 0)
			return &mem->vars[h]->value;
		h = (h + 1) % mem->size;
	}
	return NULL;
}

void print(interpreter *inter, char *name) {
	unbounded_int *u = value_of(inter->memory, name);
	if(u == NULL) fprintf(inter->error, "Variable %s not found.\n", name);
	else {
		char *str = unbounded_int2string(*u);
		fprintf(inter->output, "%s = %s\n", name, str);
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
	arithmetic *a = string_to_arithmetic(str);
	unbounded_int value = evaluate(a);
	free_arithmetic(a);
	free(str);
	return value;
}

void interpret(interpreter *inter) {
	char *buffer = calloc(1024, sizeof(char));
	do {
		if(fgets(buffer, 1024, inter->input) == NULL) break;
		if(isspace(*buffer) || *buffer == '#') continue;

		char *pos = strchr(buffer, '=');
		if(pos != NULL) {
			*pos = '\0';
			char *value = strip(pos + 1);
			char *name = strip(buffer);

			if(!valid_variable_name(name)) {
				fprintf(inter->error, "Invalid variable name.\n");
				continue;
			}

			unbounded_int u = eval(inter, value);
			if(isNaN(u)) {
				fprintf(inter->error, "Invalid expression: %s = %s\n", name, value);
				continue;
			}

			unbounded_int *v = assign(inter->memory, name, u);
			if(v == NULL) fprintf(inter->error, "Failled to assign the value with the variable name");

			free(name);
			free(value);
		} else if(strstr(buffer, "exit") != NULL) break;
		else {
			if((pos = strchr(buffer, ' ')) != NULL) {
				*pos = '\0';
				char *command = strip(buffer);
				if(strcmp(command, "print") == 0) {
					char *name = strip(pos + 1);
					print(inter, name);
					free(name);
				} else fprintf(inter->error, "Unknown command: %s\n", command);
				free(command);
			}
		}
	} while(*buffer != EOF && *buffer != '\0' && strcmp(buffer, "exit") != 0);
	free(buffer);
}
