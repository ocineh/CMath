#include <malloc.h>
#include <string.h>
#include "digits.h"

#define DEFAULT_CAPACITY 10

struct digits {
	char* digits;    // array of digits
	size_t begin;    // index of the first digit
	size_t end;      // index of the last digit + 1
	size_t length;   // number of digits
	size_t capacity; // size of the array
};

#define is_full(d) ((d)->begin == (d)->end && (d)->length > 0)
#define is_digits(c) ((c) >= '0' && (c) <= '9')

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

static void change_capacity(digits* d, size_t s) {
	if(d == NULL || d->digits == NULL || s < d->length) return;
	char* result = realloc(d->digits, s * sizeof(char));
	if(result == NULL) return;

	if(d->begin <= d->end)
		memmove(result, d->digits + d->begin, d->length);
	else {
		memmove(result, d->digits + d->begin, d->capacity - d->begin);
		memmove(result + d->capacity - d->begin, d->digits, d->end);
	}

	free(d->digits);
	d->digits = result;
	d->begin = 0, d->end = d->length + 1;
	d->capacity = s;
}

void digits_push_back(digits* d, char c) {
	if(d != NULL && d->digits != NULL && is_digits(c)) {
		if(is_full(d)) change_capacity(d, d->capacity * 2);
		d->digits[d->end++] = c;
		if(d->end == d->capacity) d->end = 0;
		d->length++;
	}
}

void digits_push_front(digits* d, char c) {
	if(d != NULL && d->digits != NULL && is_digits(c)) {
		if(is_full(d)) change_capacity(d, d->capacity * 2);
		d->begin = (d->begin == 0) ? d->capacity : d->begin - 1;
		d->digits[d->begin] = c;
		d->length++;
	}
}

char digits_at(digits const* d, size_t i) {
	if(d == NULL || d->digits == NULL || i >= d->length) return '\0';
	return d->digits[(d->begin + i) % d->capacity];
}

void digits_pop_front(digits* d) {
	if(d != NULL && d->digits != NULL && d->length > 0) {
		d->begin = (d->begin + 1) % d->capacity;
		d->length--;
	}
}
