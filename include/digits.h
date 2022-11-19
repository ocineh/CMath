#ifndef DIGITS_H
#define DIGITS_H

typedef struct digits digits;

digits* new_digits(void);

void free_digits(digits* d);

size_t digits_length(const digits* d);

#endif //DIGITS_H
