#ifndef TEST_INTEGER_H
#define TEST_INTEGER_H

#include <stdbool.h>

// Test the creation of an big_int from a string

extern bool test_create_big_int_from_string_random_positive(void);
extern bool test_create_big_int_from_string_random_negative(void);
extern bool test_create_big_int_from_string_big_positive(void);
extern bool test_create_big_int_from_string_big_negative(void);
extern bool test_create_big_int_from_string_with_invalid_string_with_letter(void);
extern bool test_create_big_int_from_string_with_empty_string(void);

// Test the creation of an big_int from a long long int

extern bool test_create_big_int_from_ll_random_positive(void);
extern bool test_create_big_int_from_ll_random_negative(void);

// Test the creation of a string from an big_int

extern bool test_create_string_from_big_int_random_positive(void);
extern bool test_create_string_from_big_int_random_negative(void);

// Test the comparison of an big_int with another big_int or a long long int

extern bool test_big_int_cmp_with_big_int_positive(void);
extern bool test_big_int_cmp_with_big_int_negative(void);
extern bool test_big_int_cmp_with_big_int_positive_and_negative(void);
extern bool test_big_int_cmp_with_ll_positive(void);
extern bool test_big_int_cmp_with_ll_negative(void);
extern bool test_big_int_cmp_with_ll_positive_and_negative(void);
extern bool test_big_int_cmp_with_ll_negative_and_positive(void);

// Test the sum of two big_int

extern bool test_big_int_sum_random_positive_and_zero(void);
extern bool test_big_int_sum_random_negative_and_zero(void);
extern bool test_big_int_sum_random_positive(void);
extern bool test_big_int_sum_random_negative(void);
extern bool test_big_int_sum_random_positive_and_negative(void);
extern bool test_big_int_sum_big_positive(void);
extern bool test_big_int_sum_big_negative(void);
extern bool test_big_int_sum_big_positive_and_negative(void);

// Test the product of two big_int

extern bool test_big_int_produit_random_number_and_zero(void);
extern bool test_big_int_produit_random_number_with_one(void);
extern bool test_big_int_produit_random_positive(void);
extern bool test_big_int_produit_random_negative(void);
extern bool test_big_int_produit_random_negative_and_positive(void);
extern bool test_big_int_produit_big_positive(void);
extern bool test_big_int_produit_big_negative(void);
extern bool test_big_int_produit_big_positive_and_negative(void);

// Test the power of an big_int by another big_int

extern bool test_big_int_pow_random_number_and_zero(void);
extern bool test_big_int_pow_positive(void);
extern bool test_big_int_pow_positive_and_negative(void);
extern bool test_big_int_pow_negative(void);
extern bool test_big_int_pow_negative_and_positive(void);

// Test the quotient of an big_int by another big_int

extern bool test_big_int_quotient_random_positive_or_negative_and_zero(void);
extern bool test_big_int_quotient_random_positive(void);
extern bool test_big_int_quotient_random_negative(void);
extern bool test_big_int_quotient_random_negative_and_positive(void);
extern bool test_big_int_quotient_random_positive_and_negative(void);
extern bool test_big_int_quotient_big_positive(void);
extern bool test_big_int_quotient_big_negative(void);
extern bool test_big_int_quotient_big_positive_and_negative(void);
extern bool test_big_int_quotient_big_negative_and_positive(void);

// Test the modulo of an big_int by another big_int

extern bool test_big_int_modulo_positive_or_negative_and_zero(void);
extern bool test_big_int_modulo_positive(void);
extern bool test_big_int_modulo_negative(void);
extern bool test_big_int_modulo_negative_and_positive(void);
extern bool test_big_int_modulo_positive_and_negative(void);
extern bool test_big_int_modulo_big_positive(void);
extern bool test_big_int_modulo_big_negative(void);
extern bool test_big_int_modulo_big_positive_and_negative(void);

// Test the difference of two big_int

extern bool test_big_int_difference_random_positive_or_negative_and_zero(void);
extern bool test_big_int_difference_random_positive(void);
extern bool test_big_int_difference_random_negative(void);
extern bool test_big_int_difference_random_negative_and_positive(void);
extern bool test_big_int_difference_random_positive_and_negative(void);
extern bool test_big_int_difference_big_positive(void);
extern bool test_big_int_difference_big_negative(void);
extern bool test_big_int_difference_big_positive_and_negative(void);

#endif //TEST_INTEGER_H
