#ifndef BIG_INT_H
#define BIG_INT_H

#define NaN ((big_int){ .sign='*', .len=0, .head=NULL, .tail=NULL })
#define ZERO (string_to_big_int("0"))
#define isZERO(x) ((x).len == 1 && (x).head->c == '0')
#define isONE(x) ((x).len == 1 && (x).head->c == '1')
#define isNaN(a) ((a).sign == '*' || (a).len == 0 || (a).head == NULL || (a).tail == NULL)

/**
 * @brief a structure representing an digit.
 * @details The node of a linked list of digits.
 * @see big_int
 */
typedef struct digit {
	struct digit* next;
	char c;
	struct digit* prev;
} digit;

/**
 * @brief a structure representing an signed big_int.
 * @details The structure representing an signed big_int in the form of a linked list of digits.
 * @see digit
 */
typedef struct big_int {
	char sign;
	size_t len;
	digit* head;
	digit* tail;
} big_int;

/**
 * Free the memory allocated for the big_int.
 *
 * @param a an big_int
 */
extern void free_big_int(big_int* a);

/**
 * Create an big_int instance from a string.
 * /!\ The string need to correspond to the regex "^[+\-0-9][0-9]*$",
 * return an instance of big_int with the field sign == '*'.
 *
 * @param str a string of characters of a number
 * @return an instance of big_int
 */
extern big_int string_to_big_int(const char* str);

/**
 * Create an big_int instance from a `long long int`.
 *
 * @param i a number
 * @return an instance of big_int
 */
extern big_int ll_to_big_int(long long i);

/**
 * Create a string from an instance of `big_int`.
 * The inverse of the `string_to_big_int` function.
 *
 * @param i an instance of big_int
 * @return the string of a number
 */
extern char* big_int_to_string(big_int i);

/**
 * Compares two big_int.
 *
 * @param a an instance of big_int
 * @param b an instance of big_int
 * @return -1 if a is less than b.<br>
 *          1 if a is greater than b.<br>
 *          0 if a is equal to b.
 */
extern int big_int_cmp_big_int(big_int a, big_int b);

/**
 * Compare an big_int and `long long int`.
 *
 * @param a an instance of big_int
 * @param b an number
 * @return -1 if a is less than b.<br>
 *          1 if a is greater than b.<br>
 *          0 if a is equal to b.
 *          -2 if a or b is not a number.
 */
extern int big_int_cmp_ll(big_int a, long long b);

/**
 * Calculates the sum of two big_int.
 *
 * @param a an instance of big_int
 * @param b an instance of big_int
 * @return the sum of a and b
 */
extern big_int unit_sum(big_int a, big_int b);

/**
 * Calculates the difference between two big_int.
 *
 * @param a an instance of big_int
 * @param b an instance of big_int
 * @return the difference between a and b
 */
extern big_int big_int_diff(big_int a, big_int b);

/**
 * Calculates the product of two big_int.
 *
 * @param a an instance of big_int
 * @param b an instance of big_int
 * @return the product of a and b
 */
extern big_int big_int_product(big_int a, big_int b);

/**
 * Copy an big_int.
 *
 * @param a an instance of big_int
 * @return a copy of a
 */
extern big_int copy_big_int(big_int* a);

/**
 * The exponentiation of an big_int by an unsigned number.
 *
 * @param u an instance of big_int
 * @param n the exponent
 * @return the result of the exponentiation
 */
extern big_int big_int_pow(big_int u, big_int n);

/**
 * The quotient of the division of two big_int.
 * @param a an instance of big_int (dividend)
 * @param b an instance of big_int (divisor)
 * @return the quotient of a and b
 */
extern big_int big_int_quotient(big_int a, big_int b);

/**
 * The remainder of the division of two big_int.
 *
 * @param a an instance of big_int (dividend)
 * @param b an instance of big_int (divisor)
 * @return the remainder of a and b
 */
extern big_int big_int_modulo(big_int a, big_int b);

#endif //BIG_INT_H
