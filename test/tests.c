#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test_big_int.h"
#include "big_int.h"

#define NUMBER_OF_TESTS 63
static int test_count = 0;
static bool FAILED = false;

static void test(bool (*f)(void), const char *msg) {
	if(f()) printf("(%2d/%d) \033[32m%-60s: OK\n\033[0m", ++test_count, NUMBER_OF_TESTS, msg);
	else printf("(%2d/%d) \033[31m%-60s: FAIL\n\033[0m", ++test_count, NUMBER_OF_TESTS, msg), FAILED = true;
}

int main(void) {
	srandom((unsigned) time(NULL));
	printf("========================= Testing big_int =========================\n\n");

	printf("Test the creation of an big_int\n");
	test(test_create_big_int_from_string_random_positive, "Test with a string of a random positive number");
	test(test_create_big_int_from_string_random_negative, "Test with a string of a random negative number");
	test(test_create_big_int_from_string_big_positive, "Test with a string of a big number");
	test(test_create_big_int_from_string_big_negative, "Test with a string of a big negative number");
	test(test_create_big_int_from_string_with_invalid_string_with_letter, "Test with an invalid string with a letter");
	test(test_create_big_int_from_string_with_empty_string, "Test with an empty string");
	test(test_create_big_int_from_ll_random_positive, "Test with a random positive long long");
	test(test_create_big_int_from_ll_random_negative, "Test with a random negative long long");

	printf("\nTest the transformation of an big_int into a string\n");
	test(test_create_string_from_big_int_random_positive, "Test with a random positive big_int");
	test(test_create_string_from_big_int_random_negative, "Test with a random negative big_int");

	printf("\nTest the comparison of two big_int\n");
	test(test_big_int_cmp_with_big_int_positive, "Test with two positive big_int");
	test(test_big_int_cmp_with_big_int_negative, "Test with two negative big_int");
	test(test_big_int_cmp_with_big_int_positive_and_negative, "Test with a positive and a negative big_int");

	printf("\nTest the comparison of an big_int with a long long int\n");
	test(test_big_int_cmp_with_ll_positive, "Test with a positive big_int and a long long");
	test(test_big_int_cmp_with_ll_negative, "Test with a negative big_int and a long long");
	test(test_big_int_cmp_with_ll_positive_and_negative, "Test with a positive big_int and a negative long long");
	test(test_big_int_cmp_with_ll_negative_and_positive, "Test with a negative big_int and a positive long long");

	printf("\nTest the addition of two big_int\n");
	test(test_big_int_sum_random_positive_and_zero, "Test with a random positive big_int and zero");
	test(test_big_int_sum_random_negative_and_zero, "Test with a random negative big_int and zero");
	test(test_big_int_sum_random_positive, "Test with two random positive big_int");
	test(test_big_int_sum_random_negative, "Test with two random negative big_int");
	test(test_big_int_sum_random_positive_and_negative, "Test with two random positive and negative big_int");
	test(test_big_int_sum_big_positive, "Test with a big positive big_int");
	test(test_big_int_sum_big_negative, "Test with a big negative big_int");
	test(test_big_int_sum_big_positive_and_negative, "Test with a big positive and negative big_int");

	printf("\nTest the product of two big_int\n");
	test(test_big_int_produit_random_number_and_zero, "Test with a random big_int and zero");
	test(test_big_int_produit_random_number_with_one, "Test with a random big_int and one");
	test(test_big_int_produit_random_positive, "Test with two random positive big_int");
	test(test_big_int_produit_random_negative, "Test with two random negative big_int");
	test(test_big_int_produit_random_negative_and_positive, "Test with a random negative and positive big_int");
	test(test_big_int_produit_big_positive, "Test with two big positive big_int");
	test(test_big_int_produit_big_negative, "Test with two big negative big_int");
	test(test_big_int_produit_big_positive_and_negative, "Test with a big positive and negative big_int");

	printf("\nTest the power of an big_int with another big_int\n");
	test(test_big_int_pow_random_number_and_zero, "Test with a random big_int and zero");
	test(test_big_int_pow_positive, "Test with two positive big_int");
	test(test_big_int_pow_positive_and_negative, "Test with a positive and a negative big_int");
	test(test_big_int_pow_negative, "Test with two negative big_int");
	test(test_big_int_pow_negative_and_positive, "Test with a negative and a positive big_int");

	printf("\nTest the quotient of an big_int with another big_int\n");
	test(test_big_int_quotient_random_positive_or_negative_and_zero, "Test with a random big_int and zero");
	test(test_big_int_quotient_random_positive, "Test with two random positive big_int");
	test(test_big_int_quotient_random_negative, "Test with two random negative big_int");
	test(test_big_int_quotient_random_negative_and_positive, "Test with a random negative and positive big_int");
	test(test_big_int_quotient_random_positive_and_negative, "Test with a random positive and negative big_int");
	test(test_big_int_quotient_big_positive, "Test with two big positive big_int");
	test(test_big_int_quotient_big_negative, "Test with two big negative big_int");
	test(test_big_int_quotient_big_positive_and_negative, "Test with a big positive and negative big_int");
	test(test_big_int_quotient_big_negative_and_positive, "Test with a big negative and a positive big_int");

	printf("\nTest the modulo of an big_int with another big_int\n");
	test(test_big_int_modulo_positive_or_negative_and_zero, "Test with a random big_int and zero");
	test(test_big_int_modulo_positive, "Test with two random positive big_int");
	test(test_big_int_modulo_negative, "Test with two random negative big_int");
	test(test_big_int_modulo_negative_and_positive, "Test with a random negative and positive big_int");
	test(test_big_int_modulo_positive_and_negative, "Test with a random positive and negative big_int");
	test(test_big_int_modulo_big_positive, "Test with two big positive big_int");
	test(test_big_int_modulo_big_negative, "Test with two big negative big_int");
	test(test_big_int_modulo_big_positive_and_negative, "Test with a big positive and negative big_int");

	printf("\nTest the difference of two big_int\n");
	test(test_big_int_difference_random_positive_or_negative_and_zero, "Test with a random big_int and zero");
	test(test_big_int_difference_random_positive, "Test with two random positive big_int");
	test(test_big_int_difference_random_negative, "Test with two random negative big_int");
	test(test_big_int_difference_random_negative_and_positive, "Test with a random negative and positive big_int");
	test(test_big_int_difference_random_positive_and_negative, "Test with a random positive and negative big_int");
	test(test_big_int_difference_big_positive, "Test with two big positive big_int");
	test(test_big_int_difference_big_negative, "Test with two big negative big_int");
	test(test_big_int_difference_big_positive_and_negative, "Test with a big positive and negative big_int");

	return FAILED ? EXIT_FAILURE : EXIT_SUCCESS;
}
