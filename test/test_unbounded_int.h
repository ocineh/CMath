#ifndef PROJET_TEST_UNBOUNDED_INT_H
#define PROJET_TEST_UNBOUNDED_INT_H

#include <stdbool.h>

// Test the creation of an unbounded int from a string

extern bool test_create_uint_from_string_random_positive(void);
extern bool test_create_uint_from_string_random_negative(void);
extern bool test_create_uint_from_string_big_positive(void);
extern bool test_create_uint_from_string_big_negative(void);
extern bool test_create_uint_from_string_with_invalid_string_with_letter(void);
extern bool test_create_uint_from_string_with_empty_string(void);

// Test the creation of an unbounded int from a long long int

extern bool test_create_uint_from_ll_random_positive(void);
extern bool test_create_uint_from_ll_random_negative(void);

// Test the creation of a string from an unbounded int

extern bool test_create_string_from_uint_random_positive(void);
extern bool test_create_string_from_uint_random_negative(void);

// Test the comparison of an unbounded int with another unbounded int or a long long int

extern bool test_uint_cmp_with_uint_positive(void);
extern bool test_uint_cmp_with_uint_negative(void);
extern bool test_uint_cmp_with_uint_positive_and_negative(void);
extern bool test_uint_cmp_with_ll_positive(void);
extern bool test_uint_cmp_with_ll_negative(void);
extern bool test_uint_cmp_with_ll_positive_and_negative(void);
extern bool test_uint_cmp_with_ll_negative_and_positive(void);

// Test the sum of two unbounded int

extern bool test_uint_sum_random_positive_and_zero(void);
extern bool test_uint_sum_random_negative_and_zero(void);
extern bool test_uint_sum_random_positive(void);
extern bool test_uint_sum_random_negative(void);
extern bool test_uint_sum_random_positive_and_negative(void);
extern bool test_uint_sum_big_positive(void);
extern bool test_uint_sum_big_negative(void);
extern bool test_uint_sum_big_positive_and_negative(void);

// Test the product of two unbounded int

extern bool test_uint_produit_random_number_and_zero(void);
extern bool test_uint_produit_random_number_with_one(void);
extern bool test_uint_produit_random_positive(void);
extern bool test_uint_produit_random_negative(void);
extern bool test_uint_produit_random_negative_and_positive(void);
extern bool test_uint_produit_big_positive(void);
extern bool test_uint_produit_big_negative(void);
extern bool test_uint_produit_big_positive_and_negative(void);

// Test the power of an unbounded int by another unbounded int

extern bool test_uint_pow_random_number_and_zero(void);
extern bool test_uint_pow_positive(void);
extern bool test_uint_pow_positive_and_negative(void);
extern bool test_uint_pow_negative(void);
extern bool test_uint_pow_negative_and_positive(void);

// Test the quotient of an unbounded int by another unbounded int

extern bool test_uint_quotient_random_positive_or_negative_and_zero(void);
extern bool test_uint_quotient_random_positive(void);
extern bool test_uint_quotient_random_negative(void);
extern bool test_uint_quotient_random_negative_and_positive(void);
extern bool test_uint_quotient_random_positive_and_negative(void);
extern bool test_uint_quotient_big_positive(void);
extern bool test_uint_quotient_big_negative(void);
extern bool test_uint_quotient_big_positive_and_negative(void);
extern bool test_uint_quotient_big_negative_and_positive(void);

// Test the modulo of an unbounded int by another unbounded int

extern bool test_uint_modulo_positive_or_negative_and_zero(void);
extern bool test_uint_modulo_positive(void);
extern bool test_uint_modulo_negative(void);
extern bool test_uint_modulo_negative_and_positive(void);
extern bool test_uint_modulo_positive_and_negative(void);
extern bool test_uint_modulo_big_positive(void);
extern bool test_uint_modulo_big_negative(void);
extern bool test_uint_modulo_big_positive_and_negative(void);

// Test the difference of two unbounded int

extern bool test_uint_difference_random_positive_or_negative_and_zero(void);
extern bool test_uint_difference_random_positive(void);
extern bool test_uint_difference_random_negative(void);
extern bool test_uint_difference_random_negative_and_positive(void);
extern bool test_uint_difference_random_positive_and_negative(void);
extern bool test_uint_difference_big_positive(void);
extern bool test_uint_difference_big_negative(void);
extern bool test_uint_difference_big_positive_and_negative(void);

#endif //PROJET_TEST_UNBOUNDED_INT_H
