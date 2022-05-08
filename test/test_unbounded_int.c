#include <stdlib.h>
#include "test_unbounded_int.h"
#include "unbounded_int.h"
#include "strings.h"

static bool uint_cmp_char(unbounded_int u, char *s) {
	if(*s == '-') ++s;
	chiffre *p = u.premier;
	while(p != NULL && *s != '\0') {
		if(p->c != *s) return false;
		p = p->suivant;
		s++;
	}
	return true;
}

static long long random_ll(long long min, long long max) {
	return (min + (rand() % (max - min + 1)));
}

#define POSITIVE (random_ll(0, 10000))
#define NEGATIVE (random_ll(-10000, 0))

static size_t len(long long n) {
	if(n == 0) return 1;
	size_t len = 0;
	while(n != 0)
		n /= 10, ++len;
	return len + 1;
}

static bool test_create_uint_from_string(long long n) {
	char *s = malloc(len(n) + 1);
	if(s == NULL) return NULL;
	sprintf(s, "%lld", n);
	char signe = (*s) == '-' ? '-' : '+';

	unbounded_int u = string2unbounded_int(s);
	bool result = !isNaN(u) && u.signe == signe && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_create_uint_from_string_1(void) {
	return test_create_uint_from_string(POSITIVE);
}

bool test_create_uint_from_string_2(void) {
	return test_create_uint_from_string(NEGATIVE);
}

bool test_create_uint_from_string_with_invalid_string(void) {
	unbounded_int u = string2unbounded_int("-9a24");
	return isNaN(u);
}

bool test_create_uint_from_string_with_invalid_string_2(void) {
	unbounded_int u = string2unbounded_int("");
	return isNaN(u);
}

bool test_create_uint_from_ll_1(void) {
	long long n = POSITIVE;
	char *s = malloc(len(n) + 1);
	sprintf(s, "%lld", n);

	unbounded_int u = ll2unbounded_int(n);
	bool result = !isNaN(u) && u.signe == '+' && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_create_uint_from_ll_2(void) {
	long long n = NEGATIVE;
	char *s = malloc(len(n) + 1);
	if(s == NULL) return NULL;
	sprintf(s, "%lld", n);

	unbounded_int u = ll2unbounded_int(n);
	bool result = !isNaN(u) && u.signe == '-' && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	free(s);
	return result;
}

static bool test_create_string_from_uint(long long n) {
	char *s = malloc(len(n) + 1);
	if(s == NULL) return NULL;
	sprintf(s, "%lld", n);

	unbounded_int u = ll2unbounded_int(n);
	bool result = !isNaN(u) && u.signe == ((n < 0) ? '-' : '+') && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_create_string_from_uint_1(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_create_string_from_uint(POSITIVE))
			return false;
	return true;
}

bool test_create_string_from_uint_2(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_create_string_from_uint(NEGATIVE))
			return false;
	return true;
}

#define cmp(a, b) (((a) < (b)) ? -1 : ((a) > (b)))

static bool test_uint_cmp_with_uint(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);

	bool result = unbounded_int_cmp_unbounded_int(u, v) == cmp(a, b);
	if(result && unbounded_int_cmp_unbounded_int(v, u) != cmp(b, a)) result = false;
	if(result && unbounded_int_cmp_unbounded_int(u, u) != 0) result = false;
	if(result && unbounded_int_cmp_unbounded_int(v, v) != 0) result = false;

	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_cmp_with_uint_1(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_uint(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_uint_2(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_uint(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_uint_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_uint(POSITIVE, NEGATIVE))
			return false;
	return true;
}

static bool test_uint_cmp_with_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	bool result = unbounded_int_cmp_ll(u, b) == cmp(a, b);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_cmp_with_ll_1(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_ll_2(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(POSITIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_ll_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

static bool test_uint_sum(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = ll2unbounded_int(a + b);

	unbounded_int res1 = unbounded_int_somme(u, v);
	unbounded_int res2 = unbounded_int_somme(u, v);
	bool result = unbounded_int_cmp_unbounded_int(res1, w) == 0 && unbounded_int_cmp_unbounded_int(res2, w) == 0;

	free_unbounded_int(&res1);
	free_unbounded_int(&res2);
	free_unbounded_int(&w);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_sum_1(void) {
	return test_uint_sum(POSITIVE, 0);
}

bool test_uint_sum_2(void) {
	return test_uint_sum(NEGATIVE, 0);
}

bool test_uint_sum_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_sum(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_sum_4(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_sum(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_sum_5(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_sum(NEGATIVE, POSITIVE))
			return false;
	return true;
}

static bool test_uint_produit(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = ll2unbounded_int(a * b);

	unbounded_int res1 = unbounded_int_produit(u, v);
	unbounded_int res2 = unbounded_int_produit(v, u);
	bool result = unbounded_int_cmp_unbounded_int(res1, w) == 0 && unbounded_int_cmp_unbounded_int(res2, w) == 0;

	free_unbounded_int(&res1);
	free_unbounded_int(&res2);
	free_unbounded_int(&w);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_produit_1(void) {
	return test_uint_produit(POSITIVE, 0) && test_uint_produit(NEGATIVE, 0);
}

bool test_uint_produit_2(void) {
	return test_uint_produit(POSITIVE, 1) && test_uint_produit(NEGATIVE, 1);
}

bool test_uint_produit_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_produit(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_produit_4(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_produit(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_produit_5(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_produit(NEGATIVE, POSITIVE))
			return false;
	return true;
}

static long long powll(long long a, long long b) {
	long long res = 1;
	while(b-- > 0) res *= a;
	return res;
}

static bool test_uint_pow(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);

	unbounded_int res1 = unbounded_int_pow(u, v);
	bool result;
	if(b >= 0) {
		unbounded_int w = ll2unbounded_int(powll(a, b));
		result = unbounded_int_cmp_unbounded_int(res1, w) == 0;
		free_unbounded_int(&w);
	} else result = isNaN(res1);

	free_unbounded_int(&res1);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_pow_1(void) {
	return test_uint_pow(random_ll(0, 1000), 0) && test_uint_pow(0, random_ll(0, 1000));
}

bool test_uint_pow_2(void) {
	for(int i = 1; i < 16; ++i)
		for(int j = 1; j < 16; ++j)
			if(!test_uint_pow(i, j))
				return false;
	return true;
}

bool test_uint_pow_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_pow(POSITIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_pow_4(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_pow(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_pow_5(void) {
	for(int i = -15; i < 0; ++i)
		for(int j = 1; j < 16; ++j)
			if(!test_uint_pow(i, j))
				return false;
	return true;
}

static bool test_uint_quotient(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);

	unbounded_int modulo = unbounded_int_quotient(u, v);
	bool result;
	if(b != 0) {
		unbounded_int w = ll2unbounded_int(a / b);
		result = unbounded_int_cmp_unbounded_int(modulo, w) == 0;
		free_unbounded_int(&w);
	} else result = isNaN(modulo);

	free_unbounded_int(&modulo);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_quotient_1(void) {
	return test_uint_quotient(POSITIVE, 0) && test_uint_quotient(NEGATIVE, 0);
}

bool test_uint_quotient_2(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_quotient_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_quotient_4(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_quotient_5(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient(POSITIVE, NEGATIVE))
			return false;
	return true;
}

static bool test_uint_modulo(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);

	unbounded_int modulo = unbounded_int_modulo(u, v);
	bool result;
	if(b != 0) {
		unbounded_int w = ll2unbounded_int(a % b);
		result = unbounded_int_cmp_unbounded_int(modulo, w) == 0;
		free_unbounded_int(&w);
	} else result = isNaN(modulo);

	free_unbounded_int(&modulo);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_modulo_1(void) {
	return test_uint_modulo(POSITIVE, 0) && test_uint_modulo(NEGATIVE, 0);
}

bool test_uint_modulo_2(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_modulo_3(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_modulo_4(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_modulo_5(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo(POSITIVE, NEGATIVE))
			return false;
	return true;
}