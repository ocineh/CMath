#include <ctype.h>
#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>
#include "strings.h"

size_t strlen(const char *s) {
	size_t len = 0;
	while((*s++) != '\0')
		len++;
	return len;
}

char *strip(char *c) {
	if(!c) return NULL;
	size_t begin = 0;
	while(isspace(c[begin])) begin++;

	size_t end = strlen(c) - 1;
	while(end > 0 && isspace(c[end])) end--;

	if(begin > end) return "";
	size_t len = end - begin + 1;
	char *new = calloc(len + 1, sizeof(char));
	if(new == NULL) return NULL;
	memmove(new, c + begin, len);
	return new;
}

bool index_of(const char *s, char c, size_t *index) {
	size_t len = strlen(s);
	for(size_t i = 0; i < len; ++i)
		if(s[i] == c) {
			if(index) *index = i;
			return true;
		}
	return false;
}

char *substring(const char *s, size_t begin, size_t end) {
	if(begin > end) return NULL;
	char *new = calloc(end - begin + 1, sizeof(char));
	if(new == NULL) return NULL;
	memmove(new, s + begin, end - begin + 1);
	new[end - begin] = '\0';
	return new;
}

char *__concat__(char *s, ...) {
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

bool is_arithmetic(char c) {
	return isdigit(c) || (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%') || c == ' ';
}

bool is_arithmetic_expression(const char *s) {
	size_t len = strlen(s);
	if(len == 0) return false;
	for(size_t i = 0; i < len; ++i)
		if(!is_arithmetic(s[i]))
			return false;
	return true;
}

bool is_digit(char c) {
	return '0' <= c && c <= '9';
}

char *remove_spaces(char *s) {
	size_t len = 0;
	char *tmp = s;
	while(*tmp != '\0') if(!isspace(*(tmp++))) ++len;

	char *res = calloc(len + 1, sizeof(char));
	if(res == NULL) return NULL;
	for(size_t j = 0, i = 0; j < len; ++j) {
		while(isspace(s[i])) ++i;
		res[j] = s[i++];
	}
	return res;
}

bool is_empty(const char *s) {
	if(s == NULL) return true;
	if(s[0] == '\0') return true;
	while(*s == ' ') ++s;
	return *s == '\0';
}
