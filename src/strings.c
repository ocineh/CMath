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
	size_t begin = 0;
	while(isspace(c[begin])) begin++;

	size_t end = strlen(c) - 1;
	while(end > 0 && isspace(c[end])) end--;

	if(begin > end) return "";
	size_t len = end - begin + 1;
	char *new = malloc(len + 1);
	memmove(new, c + begin, len);
	new[len] = '\0';
	return new;
}

static char *extract(char *s, size_t begin, size_t end) {
	size_t len = end - begin + 1;
	char *new = malloc(len);
	memmove(new, s + begin, len);
	return new;
}

int index_of(const char *s, char c) {
	size_t len = strlen(s);
	for(int i = 0; i < len; ++i)
		if(s[i] == c) return i;
	return -1;
}

char *substring(const char *s, size_t begin, size_t end) {
	char *new = malloc(end - begin + 1);
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
	char *new = malloc(len + 1);
	memmove(new, s, strlen(s) + 1);
	size_t j = strlen(s);
	while((tmp = va_arg(args, char *)) != NULL) {
		size_t l = strlen(tmp);
		memmove(new + j, tmp, l + 1);
		j += l;
	}
	va_end(args);
	return new;
}

bool is_arithmetic(char c) {
	if(isdigit(c)) return true;
	if(c == '+' || c == '-' || c == '*') return true;
	return c == ' ';
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
	if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' ||
	   c == '9')
		return true;
	return false;
}

char *remove_spaces(char *s) {
	size_t len = 0;
	char *tmp = s;
	while(*tmp != '\0') if(!isspace(*(tmp++))) ++len;

	char *res = malloc(len + 1);
	for(int j = 0, i = 0; j < len; ++j) {
		while(isspace(s[i])) ++i;
		res[j] = s[i++];
	}
	res[len] = '\0';
	return res;
}

bool is_empty(const char *s) {
	if(s == NULL) return true;
	if(s[0] == '\0') return true;
	while(*s == ' ') ++s;
	return *s == '\0';
}
