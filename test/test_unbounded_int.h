#ifndef PROJET_TEST_UNBOUNDED_INT_H
#define PROJET_TEST_UNBOUNDED_INT_H

#include <stdbool.h>

// Test the creation of an unbounded int from a string

extern bool test_create_uint_from_string(void);

extern bool test_create_uint_from_string_2(void);

extern bool test_create_uint_from_string_with_invalid_string(void);

extern bool test_create_uint_from_string_with_invalid_string_2(void);

// Test the creation of an unbounded int from a long long int

extern bool test_create_uint_from_ll(void);

extern bool test_create_uint_from_ll_2(void);

// Test the creation of a string from an unbounded int

extern bool test_create_string_from_uint(void);

extern bool test_create_string_from_uint_2(void);

// Test the comparison of an unbounded int with another unbounded int or a long long int

extern bool test_uint_cmp_with_uint(void);

extern bool test_uint_cmp_with_uint_2(void);

extern bool test_uint_cmp_with_uint_3(void);

extern bool test_uint_cmp_with_ll(void);

extern bool test_uint_cmp_with_ll_2(void);

// Test the sum of two unbounded int

extern bool test_uint_sum(void);

extern bool test_uint_sum_2(void);

extern bool test_uint_sum_3(void);

extern bool test_uint_sum_4(void);

extern bool test_uint_sum_5(void);

// Test the product of two unbounded int

extern bool test_uint_produit(void);

extern bool test_uint_produit_2(void);

extern bool test_uint_produit_3(void);

extern bool test_uint_produit_4(void);

#endif //PROJET_TEST_UNBOUNDED_INT_H
