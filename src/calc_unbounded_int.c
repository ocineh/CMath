#include <calc_unbounded_int.h>
#include <stdlib.h>
#include <ctype.h>

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
