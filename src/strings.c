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
		if(i != j) (*tokens)[k++] = extract(line, i, j - 1);
	}
	return count;
}

int index_of(const char *s, char c) {
	size_t len = strlen(s);
	for(int i = 0; i < len; ++i)
		if(s[i] == c) return i;
	return -1;
}

char *substring(const char *s, size_t begin, size_t end) {
	size_t len = strlen(s);
	char *new = malloc(end - begin + 1);
	memmove(new, s + begin, end - begin + 1);
	new[end - begin] = '\0';
	return new;
}

bool is_number(const char *s) {
	size_t len = strlen(s);
	if(len == 0) return false;
	if(s[0] == '-' || s[0] == '+') {
		if(len == 1) return false;
		++s;
		--len;
	}
	while(len-- > 0)
		if(!isdigit(*(s++)))
			return false;
	return true;
}

int last_index_of(const char *s, char c) {
	for(int i = (int) strlen(s) - 1; i >= 0; --i)
		if(s[i] == c) return i;
	return -1;
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

