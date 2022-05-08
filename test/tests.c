#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_unbounded_int.h"
#include "unbounded_int.h"

#define NUMBER_OF_TESTS 43
static int test_count = 0;

static void test(bool (*f)(void), const char *msg) {
	if(f()) printf("(%2d/%d) \033[32m%-42s: OK\n\033[0m", ++test_count, NUMBER_OF_TESTS, msg);
	else printf("(%2d/%d) \033[31m%-42s: FAIL\n\033[0m", ++test_count, NUMBER_OF_TESTS, msg);
}

int main(void) {
	srand((unsigned) time(NULL));
	printf("========================= Testing unbounded int =========================\n\n");

	printf("Test the creation of an unbounded int\n");
	test(test_create_uint_from_string_1, "Test with a string of a positive number");
	test(test_create_uint_from_string_2, "Test with a string of a negative number");
	test(test_create_uint_from_string_with_invalid_string, "Test with an invalid string");
	test(test_create_uint_from_string_with_invalid_string_2, "Test with an empty string");
	test(test_create_uint_from_ll_1, "Test with a long long (positive)");
	test(test_create_uint_from_ll_2, "Test with a long long (negative)");

	printf("\nTest the transformation of an unbounded int into a string\n");
	test(test_create_string_from_uint_1, "Test with positive unbounded int");
	test(test_create_string_from_uint_2, "Test with negative unbounded int");

	printf("\nTest the comparison of two unbounded int\n");
	test(test_uint_cmp_with_uint_1, "Test with two positive numbers");
	test(test_uint_cmp_with_uint_2, "Test with two negative numbers");
	test(test_uint_cmp_with_uint_3, "Test with a positive and a negative number");

	printf("\nTest the comparison of an unbounded int with a long long int\n");
	test(test_uint_cmp_with_ll_1, "Test with two positive numbers");
	test(test_uint_cmp_with_ll_2, "Test with a positive and a negative number");
	test(test_uint_cmp_with_ll_3, "Test with two negative numbers");

	printf("\nTest the addition of two unbounded int\n");
	test(test_uint_sum_1, "Test with a positive number and zero");
	test(test_uint_sum_2, "Test with a negative number and zero");
	test(test_uint_sum_3, "Test with two positive numbers");
	test(test_uint_sum_4, "Test with two negative numbers");
	test(test_uint_sum_5, "Test with a positive and a negative number");

	printf("\nTest the product of two unbounded int\n");
	test(test_uint_produit_1, "Test with a number and zero");
	test(test_uint_produit_2, "Test with a number and one");
	test(test_uint_produit_3, "Test with two positive numbers");
	test(test_uint_produit_4, "Test with two negative numbers");
	test(test_uint_produit_5, "Test with a positive and a negative number");

	printf("\nTest the power of an unbounded int with another unbounded int\n");
	test(test_uint_pow_1, "Test with a number and zero");
	test(test_uint_pow_2, "Test with two positive numbers");
	test(test_uint_pow_3, "Test with a positive and a negative number");
	test(test_uint_pow_4, "Test with two negative numbers");
	test(test_uint_pow_5, "Test with a negative and a positive number");

	printf("\nTest the quotient of an unbounded int with another unbounded int\n");
	test(test_uint_quotient_1, "Test with a number and zero");
	test(test_uint_quotient_2, "Test with two positive numbers");
	test(test_uint_quotient_3, "Test with two negative numbers");
	test(test_uint_quotient_4, "Test with a negative and a positive number");
	test(test_uint_quotient_5, "Test with a positive and a negative number");

	printf("\nTest the modulo of an unbounded int with another unbounded int\n");
	test(test_uint_modulo_1, "Test with a number and zero");
	test(test_uint_modulo_2, "Test with two positive numbers");
	test(test_uint_modulo_3, "Test with two negative numbers");
	test(test_uint_modulo_4, "Test with a negative and a positive number");
	test(test_uint_modulo_5, "Test with a positive and a negative number");

	printf("\nTest the difference of two unbounded int\n");
	test(test_uint_difference_1, "Test with a number and zero");
	test(test_uint_difference_2, "Test with two positive numbers");
	test(test_uint_difference_3, "Test with two negative numbers");
	test(test_uint_difference_4, "Test with a negative and a positive number");
	return 0;
}