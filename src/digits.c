#include <malloc.h>
#include "digits.h"

#define DEFAULT_CAPACITY 10

struct digits {
	char* digits;
	size_t begin, end;
	size_t length;
	size_t capacity;
};

digits* new_digits(void) {
	digits* d = malloc(sizeof(digits));
	if(d == NULL) return NULL;

	d->digits = calloc(DEFAULT_CAPACITY, sizeof(char));
	if(d->digits == NULL) {
		free(d);
		return NULL;
	}

	d->begin = d->end = 0;
	d->capacity = DEFAULT_CAPACITY;
	return d;
}

void free_digits(digits* d) {
	if(d == NULL) return;
	free(d->digits);
	free(d);
}

size_t digits_length(const digits* d) {
	if(d == NULL) return 0;
	return d->length;
}
