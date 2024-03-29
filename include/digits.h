#ifndef DIGITS_H
#define DIGITS_H

typedef struct digits digits;

digits* new_digits(void);

void free_digits(digits* d);

size_t digits_length(const digits* d);

void digits_push_back(digits* d, char c);

void digits_push_front(digits* d, char c);

char digits_at(const digits* d, size_t i);

void digits_pop_front(digits* d);

void digits_pop_back(digits* d);

digits* digits_copy(const digits* d);

#endif //DIGITS_H
