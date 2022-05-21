#include "arithmetic.h"
#include <ctype.h>
#include <calc_unbounded_int.h>
#include <stdlib.h>
#include <stdarg.h>
#include "strings.h"

char *strip(const char *c) {
	if(!c) return NULL;
	size_t begin = 0;
	while(isspace(c[begin])) begin++;

	size_t end = strlen(c) - 1;
	while(end > 0 && isspace(c[end])) end--;

	size_t len = begin <= end ? end - begin + 1 : 0;
	char *new = calloc(len + 1, sizeof(char));
	if(new == NULL) return NULL;
	if(len > 0) memmove(new, c + begin, len);
	return new;
}

char *substring(const char *s, size_t begin, size_t end) {
	if(begin > end) return NULL;
	char *new = calloc(end - begin + 1, sizeof(char));
	if(new == NULL) return NULL;
	memmove(new, s + begin, end - begin + 1);
	new[end - begin] = '\0';
	return new;
}

char *__concat__(const char *s, ...) {
	va_list args;
	va_start(args, s);
	size_t len = strlen(s);
	char *tmp;
	while((tmp = va_arg(args, char *)) != NULL)
		len += strlen(tmp);
	va_end(args);

	va_start(args, s);
	char *new = calloc(len + 1, sizeof(char));
	if(new == NULL) return NULL;
	if(memmove(new, s, strlen(s) + 1) == NULL) {
		free(new);
		return NULL;
	}
	size_t j = strlen(s);
	while((tmp = va_arg(args, char *)) != NULL) {
		size_t l = strlen(tmp);
		if(memmove(new + j, tmp, l + 1) == NULL) {
			free(new);
			return NULL;
		}
		j += l;
	}
	va_end(args);
	return new;
}

#define is_operator(c) (strchr("+-*/^%", c) != NULL)
#define is_unary(c) (strchr("+-", (c)) != NULL)

bool is_arithmetic_expression(const char *s) {
	char *tmp = remove_spaces(s);
	size_t len = strlen(tmp);
	if(len == 0) return false;
	if(!isdigit(tmp[0]) && !is_unary(tmp[0])) return false;

	for(size_t i = 1; i < len - 1; ++i) {
		if(is_operator(tmp[i])) {
			if(is_operator(tmp[i - 1]) && !is_unary(tmp[i]))
				return false;
			if(is_operator(tmp[i + 1]) && !is_unary(tmp[i]))
				return false;
		} else if(!isdigit(tmp[i])) return false;
	}

	if(!isdigit(tmp[len - 1]) && !is_unary(tmp[len - 1])) return false;
	free(tmp);
	return true;
}

char *remove_spaces(const char *s) {
	size_t len = 0;
	const char *tmp = s;
	while(*tmp != '\0') if(!isspace(*(tmp++))) ++len;

	char *res = calloc(len + 1, sizeof(char));
	if(res == NULL) return NULL;
	for(size_t j = 0, i = 0; j < len; ++j) {
		while(isspace(s[i])) ++i;
		res[j] = s[i++];
	}
	return res;
}

bool is_empty(char *s) {
	if(s == NULL) return true;
	while(*s != '\0')
		if(!isspace(*(s++)))
			return false;
	return true;
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
