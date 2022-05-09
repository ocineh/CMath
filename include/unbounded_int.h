#ifndef PROJET_UNBOUNDED_INT_H
#define PROJET_UNBOUNDED_INT_H

#include <stddef.h>

#define NaN ((unbounded_int){ .signe='*', .len=0, .premier=NULL, .dernier=NULL })
#define ZERO (ll2unbounded_int(0))
#define isZERO(x) ((x).len == 1 && (x).premier->c == '0')
#define isONE(x) ((x).len == 1 && (x).premier->c == '1')
#define isNaN(a) ((a).signe == '*' || (a).len == 0 || (a).premier == NULL || (a).dernier == NULL)

/**
 * @brief a structure representing an digit.
 * @details The node of a linked list of digits.
 * @see unbounded_int
 */
typedef struct chiffre {
	struct chiffre *suivant;
	char c;
	struct chiffre *precedent;
} chiffre;

/**
 * @brief a structure representing an unbounded signed integer.
 * @details The structure of an unbounded signed integer.
 * @see chiffre
 */
typedef struct unbounded_int {
	char signe;
	size_t len;
	chiffre *premier;
	chiffre *dernier;
} unbounded_int;

/**
 * Free the memory allocated for the unbounded_int.
 *
 * @param a an unbounded_int
 */
extern void free_unbounded_int(unbounded_int *a);

/**
 * Create an unbounded_int instance from a string.
 * /!\ If the string does not correspond to a number,
 * return an instance of unbounded_int with the field sign == '*'.
 *
 * @param str a string of characters of a number
 * @return an instance of unbounded_int
 */
extern unbounded_int string2unbounded_int(char *str);

/**
 * Create an unbounded_int instance from a `long long int`.
 *
 * @param i a number
 * @return an instance of unbounded_int
 */
extern unbounded_int ll2unbounded_int(long long i);

/**
 * Create a string from an instance of `unbounded_int`.
 * The inverse of the `string2unbounded_int` function.
 *
 * @param i an instance of unbounded_int
 * @return the string of a number
 */
extern char *unbounded_int2string(unbounded_int i);

/**
 * Compares two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return -1 if a is less than b.<br>
 *          1 if a is greater than b.<br>
 *          0 if a is equal to b.
 */
extern int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

/**
 * Compare an unbounded_int and `long long int`.
 *
 * @param a an instance of unbounded_int
 * @param b an number
 * @return -1 if a is less than b.<br>
 *          1 if a is greater than b.<br>
 *          0 if a is equal to b.
 */
extern int unbounded_int_cmp_ll(unbounded_int a, long long b);

/**
 * Calculates the sum of two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return the sum of a and b
 */
extern unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

/**
 * Calculates the difference between two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return the difference between a and b
 */
extern unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

/**
 * Calculates the product of two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return the product of a and b
 */
extern unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

/**
 * Copy an unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @return a copy of a
 */
extern unbounded_int copy_unbounded_int(unbounded_int *a);

/**
 * The exponentiation of an unbounded_int by an unsigned number.
 *
 * @param u an instance of unbounded_int
 * @param n the exponent
 * @return the result of the exponentiation
 */
extern unbounded_int unbounded_int_pow(unbounded_int u, unbounded_int n);

extern unbounded_int unbounded_int_quotient(unbounded_int a, unbounded_int b);

extern unbounded_int unbounded_int_modulo(unbounded_int a, unbounded_int b);

#endif //PROJET_UNBOUNDED_INT_H
