#include <ctype.h>
#include <stdlib.h>
#include <memory.h>
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
		if(i != j) (*tokens)[k++] = extract(line, i, j - 1);
	}
	return count;
}

size_t index_of(const char *s, char c) {
	size_t len = strlen(s);
	for(size_t i = 0; i < len; ++i)
		if(s[i] == c) return i;
	return -1;
}

char *substring(const char *s, size_t begin, size_t end) {
	size_t len = strlen(s);
	if(begin >= end || begin >= len || end >= len) return "";
	char *new = malloc(end - begin + 1);
	memmove(new, s + begin, end - begin + 1);
	return new;
}