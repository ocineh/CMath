#ifndef PROJET_UNBOUNDED_INT_H
#define PROJET_UNBOUNDED_INT_H

#include <stddef.h>

#define NaN ((unbounded_int){ .signe='*', .len=0, .premier=NULL, .dernier=NULL })
#define ZERO (ll2unbounded_int(0))
#define isZERO(x) (x.len == 1 && x.premier->c == '0')
#define isNaN(a) (a.signe == '*')

typedef struct chiffre {
	struct chiffre *suivant;
	char c;
	struct chiffre *precedent;
} chiffre;

typedef struct {
	char signe;
	size_t len;
	chiffre *premier;
	chiffre *dernier;
} unbounded_int;

void free_unbounded_int(unbounded_int *a);

/**
 * Print the digits composing an unbounded_int.
 *
 * @param u an instance of unbounded_int
 */
void print_unbounded_int(unbounded_int *u);

/**
 * Create an unbounded_int instance from a string.
 * /!\ If the string does not correspond to a number,
 * return an instance of unbounded_int with the field sign == '*'.
 *
 * @param e a string of characters of a number
 * @return an instance of unbounded_int
 */
unbounded_int string2unbounded_int(const char *e);

/**
 * Create an unbounded_int instance from a `long long int`.
 *
 * @param i a number
 * @return an instance of unbounded_int
 */
unbounded_int ll2unbounded_int(long long i);

/**
 * Create a string from an instance of `unbounded_int`.
 * The inverse of the `string2unbounded_int` function.
 *
 * @param i an instance of unbounded_int
 * @return the string of a number
 */
char *unbounded_int2string(unbounded_int i);

/**
 * Compares two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return -1 if a is less than b.<br>
 *          1 if a is greater than b.<br>
 *          0 if a is equal to b.
 */
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

/**
 * Compare an unbounded_int and `long long int`.
 *
 * @param a an instance of unbounded_int
 * @param b an number
 * @return -1 if a is less than b.<br>
 *          1 if a is greater than b.<br>
 *          0 if a is equal to b.
 */
int unbounded_int_cmp_ll(unbounded_int a, long long b);

/**
 * Calculates the sum of two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return the sum of a and b
 */
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

/**
 * Calculates the difference between two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return the difference between a and b
 */
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

/**
 * Calculates the product of two unbounded_int.
 *
 * @param a an instance of unbounded_int
 * @param b an instance of unbounded_int
 * @return the product of a and b
 */
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

#endif //PROJET_UNBOUNDED_INT_H
