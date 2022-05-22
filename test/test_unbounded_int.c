#include <stdlib.h>
#include <limits.h>
#include "test_unbounded_int.h"
#include "unbounded_int.h"
#include "utils.h"

static bool uint_cmp_char(unbounded_int u, const char *s) {
	if(*s == '-' || *s == '+') {
		++s;
	}
	while(*s == '0' && *(s + 1) != '\0') ++s;

	chiffre *p = u.premier;
	while(p != NULL && *s != '\0') {
		if(p->c != *s) return false;
		p = p->suivant;
		s++;
	}
	return true;
}

static long long random_ll(long long min, long long max) {
	return (min + (random() % (max - min + 1)));
}

#define POSITIVE random_ll(0, LLONG_MAX)
#define NEGATIVE random_ll(LLONG_MIN, 0)

static size_t len(long long n) {
	if(n == 0) return 1;
	size_t len = n < 0 ? 1 : 0;
	while(n != 0)
		n /= 10, ++len;
	return len + 1;
}

static char *ll_to_string(long long n) {
	size_t size = len(n);
	char *s = calloc(size + 1, sizeof(char));
	if(s == NULL) return NULL;
	sprintf(s, "%lld", n);
	return s;
}

static bool test_create_uint_from_string(const char *s) {
	char signe = (*s) == '-' ? '-' : '+';

	unbounded_int u = string2unbounded_int(s);
	bool result = !isNaN(u) && u.signe == signe && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	return result;
}

bool test_create_uint_from_string_random_positive(void) {
	for(int i = 0; i < 100; ++i) {
		char *s = ll_to_string(POSITIVE);
		if(s != NULL) {
			bool result = test_create_uint_from_string(s);
			free(s);
			if(!result) return false;
		} else return false;
	}
	return true;
}

bool test_create_uint_from_string_random_negative(void) {
	for(int i = 0; i < 100; ++i) {
		char *s = ll_to_string(NEGATIVE);
		if(s != NULL) {
			bool result = test_create_uint_from_string(s);
			free(s);
			if(!result) return false;
		} else return false;
	}
	return true;
}

bool test_create_uint_from_string_big_positive(void) {
	return test_create_uint_from_string("189764987241897649872418976498724") &&
		   test_create_uint_from_string("953757454949690413349269380650547168361096998") &&
		   test_create_uint_from_string("+953757454949690413349269380650547168361096998") &&
		   test_create_uint_from_string("00000585487897654980000000") &&
		   test_create_uint_from_string("+00000585487897654980000000");
}

bool test_create_uint_from_string_big_negative(void) {
	return test_create_uint_from_string("-189764987241897649872418976498724") &&
		   test_create_uint_from_string("-953757454949690413349269380650547168361096998") &&
		   test_create_uint_from_string("-00000585487897654980000000");
}

static bool test_create_uint_from_string_with_invalid_string(char *s) {
	s = copy(s);
	unbounded_int u = string2unbounded_int(s);
	free(s);
	bool result = isNaN(u);
	free_unbounded_int(&u);
	return result;
}

bool test_create_uint_from_string_with_invalid_string_with_letter(void) {
	return test_create_uint_from_string_with_invalid_string("-9a24") &&
		   test_create_uint_from_string_with_invalid_string("9a24") &&
		   test_create_uint_from_string_with_invalid_string("ahez") &&
		   test_create_uint_from_string_with_invalid_string("-ahez") &&
		   test_create_uint_from_string_with_invalid_string("a5648") &&
		   test_create_uint_from_string_with_invalid_string("-a5648") &&
		   test_create_uint_from_string_with_invalid_string("7894561hdfg") &&
		   test_create_uint_from_string_with_invalid_string("-7894561hdfg");
}

bool test_create_uint_from_string_with_empty_string(void) {
	return test_create_uint_from_string_with_invalid_string("") &&
		   test_create_uint_from_string_with_invalid_string("         ");
}

static bool test_create_uint_from_ll(long long n) {
	char *s = calloc(len(n) + 1, sizeof(char));
	if(s == NULL) return NULL;
	sprintf(s, "%lld", n);

	unbounded_int u = ll2unbounded_int(n);
	bool result = !isNaN(u) && u.signe == (n < 0 ? '-' : '+') && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_create_uint_from_ll_random_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_create_uint_from_ll(POSITIVE))
			return false;
	return true;
}

bool test_create_uint_from_ll_random_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_create_uint_from_ll(NEGATIVE))
			return false;
	return true;
}

static bool test_create_string_from_uint(long long n) {
	char *s = calloc(len(n) + 1, sizeof(char));
	if(s == NULL) return NULL;
	sprintf(s, "%lld", n);

	unbounded_int u = ll2unbounded_int(n);
	bool result = !isNaN(u) && u.signe == ((n < 0) ? '-' : '+') && uint_cmp_char(u, s);
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_create_string_from_uint_random_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_create_string_from_uint(POSITIVE))
			return false;
	return true;
}

bool test_create_string_from_uint_random_negative(void) {
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

bool test_uint_cmp_with_uint_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_uint(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_uint_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_uint(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_uint_positive_and_negative(void) {
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

bool test_uint_cmp_with_ll_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_ll_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_ll_positive_and_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(POSITIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_cmp_with_ll_negative_and_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_cmp_with_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

#undef POSITIVE
#undef NEGATIVE

#define POSITIVE random_ll(0, 1000000)
#define NEGATIVE random_ll(-1000000, 0)

static bool test_uint_sum(unbounded_int u, unbounded_int v, unbounded_int w) {
	unbounded_int res1 = unbounded_int_somme(u, v);
	unbounded_int res2 = unbounded_int_somme(v, u);
	bool result = unbounded_int_cmp_unbounded_int(res1, w) == 0 && unbounded_int_cmp_unbounded_int(res2, w) == 0;
	free_unbounded_int(&res1);
	free_unbounded_int(&res2);
	return result;
}

static bool test_uint_sum_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = ll2unbounded_int(a + b);
	bool result = test_uint_sum(u, v, w);
	free_unbounded_int(&w);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

static bool test_uint_sum_string(const char *a, const char *b, const char *c) {
	unbounded_int u = string2unbounded_int(a);
	unbounded_int v = string2unbounded_int(b);
	unbounded_int w = string2unbounded_int(c);
	bool result = test_uint_sum(u, v, w);
	free_unbounded_int(&w);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_sum_random_positive_and_zero(void) {
	return test_uint_sum_ll(POSITIVE, 0);
}

bool test_uint_sum_random_negative_and_zero(void) {
	return test_uint_sum_ll(NEGATIVE, 0);
}

bool test_uint_sum_random_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_sum_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_sum_random_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_sum_ll(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_sum_random_positive_and_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_sum_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_sum_big_positive(void) {
	return test_uint_sum_string(
			"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "1",
			"100000000000000000000000000000000000000000000000000000000000000000000000000000000000000") &&
		   test_uint_sum_string("32165479846987321687147458817345983467285614279574198542113",
								"5478486484861475585715187115869245",
								"32165479846987321687147464295832468328761199994761314411358");
}

bool test_uint_sum_big_negative(void) {
	return test_uint_sum_string(
			"-99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "-1",
			"-100000000000000000000000000000000000000000000000000000000000000000000000000000000000000") &&
		   test_uint_sum_string("-32165479846987321687147458817345983467285614279574198542113",
								"-5478486484861475585715187115869245",
								"-32165479846987321687147464295832468328761199994761314411358");
}

bool test_uint_sum_big_positive_and_negative(void) {
	return test_uint_sum_string(
			"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "-1",
			"99999999999999999999999999999999999999999999999999999999999999999999999999999999999998") &&
		   test_uint_sum_string("32165479846987321687147458817345983467285614279574198542113",
								"-5478486484861475585715187115869245",
								"32165479846987321687147453338859498605810028564387082672868");
}

static bool test_uint_produit(unbounded_int u, unbounded_int v, unbounded_int w) {
	unbounded_int res1 = unbounded_int_produit(u, v);
	unbounded_int res2 = unbounded_int_produit(v, u);
	bool result = unbounded_int_cmp_unbounded_int(res1, w) == 0 && unbounded_int_cmp_unbounded_int(res2, w) == 0;
	free_unbounded_int(&res1);
	free_unbounded_int(&res2);
	return result;
}

static bool test_uint_produit_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = ll2unbounded_int(a * b);

	bool result = test_uint_produit(u, v, w);

	free_unbounded_int(&w);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

static bool test_uint_produit_string(char *a, char *b, char *c) {
	a = copy(a), b = copy(b), c = copy(c);
	unbounded_int u = string2unbounded_int(a);
	unbounded_int v = string2unbounded_int(b);
	unbounded_int w = string2unbounded_int(c);
	free(a), free(b), free(c);

	bool result = test_uint_produit(u, v, w);

	free_unbounded_int(&w);
	free_unbounded_int(&v);
	free_unbounded_int(&u);
	return result;
}

bool test_uint_produit_random_number_and_zero(void) {
	return test_uint_produit_ll(POSITIVE, 0) && test_uint_produit_ll(NEGATIVE, 0);
}

bool test_uint_produit_random_number_with_one(void) {
	return test_uint_produit_ll(POSITIVE, 1) && test_uint_produit_ll(NEGATIVE, 1);
}

bool test_uint_produit_random_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_produit_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_produit_random_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_produit_ll(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_produit_random_negative_and_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_produit_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_produit_big_positive(void) {
	return test_uint_produit_string(
			"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "1",
			"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999") &&
		   test_uint_produit_string("32165479846987321687147458817345983467285614279574198542113",
									"5478486484861475585715187115869245",
									"176218146620804205573390879846112325811750651344268325624168716297297102079324476685734014685");
}

bool test_uint_produit_big_negative(void) {
	return test_uint_produit_string(
			"-99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "-1",
			"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999") &&
		   test_uint_produit_string("-32165479846987321687147458817345983467285614279574198542113",
									"-5478486484861475585715187115869245",
									"176218146620804205573390879846112325811750651344268325624168716297297102079324476685734014685");
}

bool test_uint_produit_big_positive_and_negative(void) {
	return test_uint_produit_string(
			"-99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "1",
			"-99999999999999999999999999999999999999999999999999999999999999999999999999999999999999") &&
		   test_uint_produit_string(
				   "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999", "-1",
				   "-99999999999999999999999999999999999999999999999999999999999999999999999999999999999999") &&
		   test_uint_produit_string("-32165479846987321687147458817345983467285614279574198542113",
									"5478486484861475585715187115869245",
									"-176218146620804205573390879846112325811750651344268325624168716297297102079324476685734014685") &&
		   test_uint_produit_string("32165479846987321687147458817345983467285614279574198542113",
									"-5478486484861475585715187115869245",
									"-176218146620804205573390879846112325811750651344268325624168716297297102079324476685734014685");
}

static long long powll(long long a, long long b) {
	long long res = 1;
	while(b-- > 0) res *= a;
	return res;
}

static bool test_uint_pow(unbounded_int u, unbounded_int v, unbounded_int w) {
	unbounded_int res = unbounded_int_pow(u, v);
	bool result = v.signe == '-' ? isNaN(res) : unbounded_int_cmp_unbounded_int(res, w) == 0;
	free_unbounded_int(&res);
	return result;
}

static bool test_uint_pow_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = ll2unbounded_int(powll(a, b));

	bool result = test_uint_pow(u, v, w);

	free_unbounded_int(&v);
	free_unbounded_int(&u);
	free_unbounded_int(&w);
	return result;
}

static bool test_uint_pow_string(char *a, char *b, char *c) {
	a = copy(a), b = copy(b), c = copy(c);
	unbounded_int u = string2unbounded_int(a);
	unbounded_int v = string2unbounded_int(b);
	unbounded_int w = string2unbounded_int(c);
	free(a), free(b), free(c);

	bool result = test_uint_pow(u, v, w);

	free_unbounded_int(&v);
	free_unbounded_int(&u);
	free_unbounded_int(&w);
	return result;
}

bool test_uint_pow_random_number_and_zero(void) {
	return test_uint_pow_ll(random_ll(0, 1000), 0) && test_uint_pow_ll(0, random_ll(0, 1000));
}

bool test_uint_pow_positive(void) {
	return test_uint_pow_string("2", "1000",
								"10715086071862673209484250490600018105614048117055336074437503883703510511249361224931983788156958581275946729175531468251871452856923140435984577574698574803934567774824230985421074605062371141877954182153046474983581941267398767559165543946077062914571196477686542167660429831652624386837205668069376") &&
		   test_uint_pow_string("14", "23", "229585692886981495482220544");
}

bool test_uint_pow_positive_and_negative(void) {
	return test_uint_pow_string("2", "-1000", "") && test_uint_pow_string("74", "-6", "");
}

bool test_uint_pow_negative(void) {
	return test_uint_pow_string("-2", "-1000", "") && test_uint_pow_string("-74", "-6", "");
}

bool test_uint_pow_negative_and_positive(void) {
	return test_uint_pow_string("-2", "100", "1267650600228229401496703205376") &&
		   test_uint_pow_string("-74", "6", "164206490176") && test_uint_pow_string("-74", "5", "-2219006624");
}

static bool test_uint_quotient(unbounded_int u, unbounded_int v, unbounded_int w) {
	unbounded_int res = unbounded_int_quotient(u, v);
	bool result = isZERO(v) ? isNaN(res) : unbounded_int_cmp_unbounded_int(res, w) == 0;
	free_unbounded_int(&res);
	return result;
}

static bool test_uint_quotient_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = b != 0 ? ll2unbounded_int(a / b) : NaN;

	bool result = test_uint_quotient(u, v, w);

	free_unbounded_int(&v);
	free_unbounded_int(&u);
	free_unbounded_int(&w);
	return result;
}

static bool test_uint_quotient_string(char *a, char *b, char *c) {
	a = copy(a), b = copy(b), c = copy(c);
	unbounded_int u = string2unbounded_int(a);
	unbounded_int v = string2unbounded_int(b);
	unbounded_int w = string2unbounded_int(c);
	free(a), free(b), free(c);

	bool result = test_uint_quotient(u, v, w);

	free_unbounded_int(&v);
	free_unbounded_int(&u);
	free_unbounded_int(&w);
	return result;
}

bool test_uint_quotient_random_positive_or_negative_and_zero(void) {
	return test_uint_quotient_ll(POSITIVE, 0) && test_uint_quotient_ll(NEGATIVE, 0);
}

bool test_uint_quotient_random_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_quotient_random_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient_ll(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_quotient_random_negative_and_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_quotient_random_positive_and_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_quotient_ll(POSITIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_quotient_big_positive(void) {
	return test_uint_quotient_string("26564725936465145879484854875924", "7445456451756182", "3567910994926206") &&
		   test_uint_quotient_string("45475482448", "45458712657519874621679871", "0");
}

bool test_uint_quotient_big_negative(void) {
	return test_uint_quotient_string("-26564725936465145879484854875924", "-7445456451756182", "3567910994926206") &&
		   test_uint_quotient_string("-45475482448", "-45458712657519874621679871", "0");
}

bool test_uint_quotient_big_positive_and_negative(void) {
	return test_uint_quotient_string("26564725936465145879484854875924", "-7445456451756182", "-3567910994926206") &&
		   test_uint_quotient_string("45475482448", "-45458712657519874621679871", "0");
}

bool test_uint_quotient_big_negative_and_positive(void) {
	return test_uint_quotient_string("-26564725936465145879484854875924", "7445456451756182", "-3567910994926206") &&
		   test_uint_quotient_string("-45475482448", "45458712657519874621679871", "0");
}

static bool test_uint_modulo(unbounded_int u, unbounded_int v, unbounded_int w) {
	unbounded_int res = unbounded_int_modulo(u, v);
	bool result = isZERO(v) ? isNaN(res) : unbounded_int_cmp_unbounded_int(res, w) == 0;
	free_unbounded_int(&res);
	return result;
}

static bool test_uint_modulo_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = b != 0 ? ll2unbounded_int(a % b) : NaN;

	bool result = test_uint_modulo(u, v, w);

	free_unbounded_int(&v);
	free_unbounded_int(&u);
	free_unbounded_int(&w);
	return result;
}

static bool test_uint_modulo_string(char *a, char *b, char *c) {
	a = copy(a), b = copy(b), c = copy(c);
	unbounded_int u = string2unbounded_int(a);
	unbounded_int v = string2unbounded_int(b);
	unbounded_int w = string2unbounded_int(c);
	free(a), free(b), free(c);

	bool result = test_uint_modulo(u, v, w);

	free_unbounded_int(&v);
	free_unbounded_int(&u);
	free_unbounded_int(&w);
	return result;
}

bool test_uint_modulo_positive_or_negative_and_zero(void) {
	return test_uint_modulo_ll(POSITIVE, 0) && test_uint_modulo_ll(NEGATIVE, 0);
}

bool test_uint_modulo_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_modulo_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo_ll(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_modulo_negative_and_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_modulo_positive_and_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_modulo_ll(POSITIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_modulo_big_positive(void) {
	return test_uint_modulo_string("26564725936465145879484854875924", "7445456451756182", "7075864660570432") &&
		   test_uint_modulo_string("45475482448", "45458712657519874621679871", "45475482448");
}

bool test_uint_modulo_big_negative(void) {
	return test_uint_modulo_string("-26564725936465145879484854875924", "-7445456451756182", "-7075864660570432") &&
		   test_uint_modulo_string("-45475482448", "-45458712657519874621679871", "-45475482448");
}

bool test_uint_modulo_big_positive_and_negative(void) {
	return test_uint_modulo_string("26564725936465145879484854875924", "-7445456451756182", "7075864660570432") &&
		   test_uint_modulo_string("45475482448", "-45458712657519874621679871", "45475482448");
}

static bool test_uint_difference(unbounded_int u, unbounded_int v, unbounded_int w) {
	unbounded_int res = unbounded_int_difference(u, v);
	bool result = unbounded_int_cmp_unbounded_int(res, w) == 0;
	if(!result) {
		printf("%s - %s = %s ", unbounded_int2string(u), unbounded_int2string(v), unbounded_int2string(res));
		printf("!= %s\n", unbounded_int2string(w));
	}
	free_unbounded_int(&res);
	return result;
}

static bool test_uint_difference_ll(long long a, long long b) {
	unbounded_int u = ll2unbounded_int(a);
	unbounded_int v = ll2unbounded_int(b);
	unbounded_int w = ll2unbounded_int(a - b);

	bool result = test_uint_difference(u, v, w);
	if(result) {
		free_unbounded_int(&w);
		w = ll2unbounded_int(b - a);
		result = test_uint_difference(v, u, w);
	}

	free_unbounded_int(&u);
	free_unbounded_int(&v);
	free_unbounded_int(&w);
	return result;
}

static bool test_uint_difference_string(char *a, char *b, char *c) {
	a = copy(a), b = copy(b), c = copy(c);
	unbounded_int u = string2unbounded_int(a);
	unbounded_int v = string2unbounded_int(b);
	unbounded_int w = string2unbounded_int(c);
	free(a), free(b), free(c);

	bool result = test_uint_difference(u, v, w);

	free_unbounded_int(&u);
	free_unbounded_int(&v);
	free_unbounded_int(&w);
	return result;
}

bool test_uint_difference_random_positive_or_negative_and_zero(void) {
	return test_uint_difference_ll(POSITIVE, 0) && test_uint_difference_ll(NEGATIVE, 0);
}

bool test_uint_difference_random_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_difference_ll(POSITIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_difference_random_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_difference_ll(NEGATIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_difference_random_negative_and_positive(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_difference_ll(NEGATIVE, POSITIVE))
			return false;
	return true;
}

bool test_uint_difference_random_positive_and_negative(void) {
	for(int i = 0; i < 100; ++i)
		if(!test_uint_difference_ll(POSITIVE, NEGATIVE))
			return false;
	return true;
}

bool test_uint_difference_big_positive(void) {
	return test_uint_difference_string("26564725936465145879484854875924", "7445456451756182",
									   "26564725936465138434028403119742") &&
		   test_uint_difference_string("7445456451756182", "26564725936465145879484854875924",
									   "-26564725936465138434028403119742") &&
		   test_uint_difference_string("45475482448", "45458712657519874621679871", "-45458712657519829146197423") &&
		   test_uint_difference_string("45458712657519874621679871", "45475482448", "45458712657519829146197423");
}

bool test_uint_difference_big_negative(void) {
	return test_uint_difference_string("-26564725936465145879484854875924", "-7445456451756182",
									   "-26564725936465138434028403119742") &&
		   test_uint_difference_string("-7445456451756182", "-26564725936465145879484854875924",
									   "26564725936465138434028403119742") &&
		   test_uint_difference_string("-45475482448", "-45458712657519874621679871", "45458712657519829146197423") &&
		   test_uint_difference_string("-45458712657519874621679871", "-45475482448", "-45458712657519829146197423");
}

bool test_uint_difference_big_positive_and_negative(void) {
	return test_uint_difference_string("-26564725936465145879484854875924", "7445456451756182",
									   "-26564725936465153324941306632106") &&
		   test_uint_difference_string("26564725936465145879484854875924", "-7445456451756182",
									   "26564725936465153324941306632106") &&
		   test_uint_difference_string("-7445456451756182", "26564725936465145879484854875924",
									   "-26564725936465153324941306632106") &&
		   test_uint_difference_string("7445456451756182", "-26564725936465145879484854875924",
									   "26564725936465153324941306632106") &&
		   test_uint_difference_string("-45475482448", "45458712657519874621679871", "-45458712657519920097162319") &&
		   test_uint_difference_string("45475482448", "-45458712657519874621679871", "45458712657519920097162319") &&
		   test_uint_difference_string("45458712657519874621679871", "-45475482448", "45458712657519920097162319") &&
		   test_uint_difference_string("-45458712657519874621679871", "45475482448", "-45458712657519920097162319");
}

