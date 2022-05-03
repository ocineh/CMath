#include <string.h>
#include <malloc.h>
#include "test_unbounded_int.h"
#include "unbounded_int.h"
#include "strings.h"

static bool uint_cmp_char(unbounded_int u, char *s) {
	size_t len = strlen(s);
	if(len != u.len) return false;
	chiffre *p = u.premier;
	while(p != NULL) {
		if(p->c != *s) return false;
		p = p->suivant;
		s++;
	}
	return true;
}

bool test_create_uint_from_string(void) {
	unbounded_int u = string2unbounded_int("364");
	bool result = !isNaN(u) && u.signe == '+' && uint_cmp_char(u, "364");
	free_unbounded_int(&u);
	return result;
}

bool test_create_uint_from_string_2(void) {
	unbounded_int u = string2unbounded_int("-924");
	bool result = !isNaN(u) && u.signe == '-' && uint_cmp_char(u, "924");
	free_unbounded_int(&u);
	return result;
}

bool test_create_uint_from_string_with_invalid_string(void) {
	unbounded_int u = string2unbounded_int("-9a24");
	return isNaN(u);
}

bool test_create_uint_from_string_with_invalid_string_2(void) {
	unbounded_int u = string2unbounded_int("");
	return isNaN(u);
}

bool test_create_uint_from_ll(void) {
	unbounded_int u = ll2unbounded_int(924);
	bool result = !isNaN(u) && u.signe == '+' && uint_cmp_char(u, "924");
	free_unbounded_int(&u);
	return result;
}

bool test_create_uint_from_ll_2(void) {
	unbounded_int u = ll2unbounded_int(-12);
	bool result = !isNaN(u) && u.signe == '-' && uint_cmp_char(u, "12");
	free_unbounded_int(&u);
	return result;
}

bool test_create_string_from_uint(void) {
	unbounded_int u = string2unbounded_int("924");
	char *s = unbounded_int2string(u);
	bool result = strcmp(s, "924") == 0;
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_create_string_from_uint_2(void) {
	unbounded_int u = string2unbounded_int("-75486");
	char *s = unbounded_int2string(u);
	bool result = strcmp(s, "-75486") == 0;
	free_unbounded_int(&u);
	free(s);
	return result;
}

bool test_uint_cmp_with_uint(void) {
	unbounded_int u = string2unbounded_int("8908");
	unbounded_int v = string2unbounded_int("3678");

	bool result = unbounded_int_cmp_unbounded_int(u, v) == 1;
	if(result && unbounded_int_cmp_unbounded_int(v, u) != -1) result = false;
	if(result && unbounded_int_cmp_unbounded_int(u, u) != 0) result = false;
	if(result && unbounded_int_cmp_unbounded_int(v, v) != 0) result = false;

	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_cmp_with_uint_2(void) {
	unbounded_int u = string2unbounded_int("-8908");
	unbounded_int v = string2unbounded_int("-3678");

	bool result = unbounded_int_cmp_unbounded_int(u, v) == -1;
	if(result && unbounded_int_cmp_unbounded_int(v, u) != 1) result = false;
	if(result && unbounded_int_cmp_unbounded_int(u, u) != 0) result = false;
	if(result && unbounded_int_cmp_unbounded_int(v, v) != 0) result = false;

	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return 0;
}

bool test_uint_cmp_with_uint_3(void) {
	unbounded_int u = string2unbounded_int("-8908");
	unbounded_int v = string2unbounded_int("3678");

	bool result = unbounded_int_cmp_unbounded_int(u, v) == -1;
	if(result && unbounded_int_cmp_unbounded_int(v, u) != 1) result = false;
	if(result && unbounded_int_cmp_unbounded_int(u, u) != 0) result = false;
	if(result && unbounded_int_cmp_unbounded_int(v, v) != 0) result = false;

	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return 0;
}

bool test_uint_cmp_with_ll(void) {
	unbounded_int u = string2unbounded_int("6404");
	bool result = unbounded_int_cmp_ll(u, 3740) == 1;
	if(result && unbounded_int_cmp_ll(u, -3740) != 1) result = false;
	if(result && unbounded_int_cmp_ll(u, 73512) != -1) result = false;
	if(result && unbounded_int_cmp_ll(u, 6404) != 0) result = false;
	free_unbounded_int(&u);
	return result;
}

bool test_uint_cmp_with_ll_2(void) {
	unbounded_int u = string2unbounded_int("-3832");
	bool result = unbounded_int_cmp_ll(u, 657) == -1;
	if(result && unbounded_int_cmp_ll(u, -370) != -1) result = false;
	if(result && unbounded_int_cmp_ll(u, -73512) != 1) result = false;
	if(result && unbounded_int_cmp_ll(u, -3832) != 0) result = false;
	free_unbounded_int(&u);
	return result;
}

bool test_uint_sum(void) {
	unbounded_int u = string2unbounded_int("478");
	unbounded_int v = ZERO;

	unbounded_int sum = unbounded_int_somme(u, v);
	bool result = unbounded_int_cmp_ll(sum, 478) == 0;
	if(result) {
		free_unbounded_int(&sum);
		sum = unbounded_int_somme(v, u);
		result = unbounded_int_cmp_ll(sum, 478) == 0;
	}

	free_unbounded_int(&sum);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_sum_2(void) {
	unbounded_int u = ZERO;
	unbounded_int v = ZERO;

	unbounded_int sum = unbounded_int_somme(u, v);
	bool result = unbounded_int_cmp_ll(sum, 0) == 0;
	if(result) {
		free_unbounded_int(&sum);
		sum = unbounded_int_somme(v, u);
		result = unbounded_int_cmp_ll(sum, 0) == 0;
	}

	free_unbounded_int(&sum);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_sum_3(void) {
	unbounded_int u = string2unbounded_int("-478");
	unbounded_int v = ZERO;

	unbounded_int sum = unbounded_int_somme(u, v);
	bool result = unbounded_int_cmp_ll(sum, -478) == 0;
	if(result) {
		free_unbounded_int(&sum);
		sum = unbounded_int_somme(v, u);
		result = unbounded_int_cmp_ll(sum, -478) == 0;
	}

	free_unbounded_int(&sum);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_sum_4(void) {
	unbounded_int u = string2unbounded_int("-478");
	unbounded_int v = string2unbounded_int("-6632");

	unbounded_int sum = unbounded_int_somme(u, v);
	bool result = unbounded_int_cmp_ll(sum, -7110) == 0;
	if(result) {
		free_unbounded_int(&sum);
		sum = unbounded_int_somme(v, u);
		result = unbounded_int_cmp_ll(sum, -7110) == 0;
	}

	free_unbounded_int(&sum);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_sum_5(void) {
	unbounded_int u = string2unbounded_int("478");
	unbounded_int v = string2unbounded_int("-6632");

	unbounded_int sum = unbounded_int_somme(u, v);
	bool result = unbounded_int_cmp_ll(sum, -6154) == 0;
	if(result) {
		free_unbounded_int(&sum);
		sum = unbounded_int_somme(v, u);
		result = unbounded_int_cmp_ll(sum, -6154) == 0;
	}

	free_unbounded_int(&sum);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_produit(void) {
	unbounded_int u = string2unbounded_int("478");
	unbounded_int v = string2unbounded_int("6632");

	unbounded_int prod = unbounded_int_produit(u, v);
	bool result = unbounded_int_cmp_ll(prod, 3170096) == 0;
	if(result) {
		free_unbounded_int(&prod);
		prod = unbounded_int_produit(v, u);
		result = unbounded_int_cmp_ll(prod, 3170096) == 0;
	}

	free_unbounded_int(&prod);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_produit_2(void) {
	unbounded_int u = string2unbounded_int("478");
	unbounded_int v = ZERO;

	unbounded_int prod = unbounded_int_produit(u, v);
	bool result = unbounded_int_cmp_unbounded_int(prod, v) == 0;
	if(result) {
		free_unbounded_int(&prod);
		prod = unbounded_int_produit(v, u);
		result = unbounded_int_cmp_unbounded_int(prod, v) == 0;
	}

	free_unbounded_int(&prod);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_produit_3(void) {
	unbounded_int u = string2unbounded_int("-478");
	unbounded_int v = ZERO;

	unbounded_int prod = unbounded_int_produit(u, v);
	bool result = unbounded_int_cmp_unbounded_int(prod, v) == 0;
	if(result) {
		free_unbounded_int(&prod);
		prod = unbounded_int_produit(v, u);
		result = unbounded_int_cmp_unbounded_int(prod, v) == 0;
	}

	free_unbounded_int(&prod);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}

bool test_uint_produit_4(void) {
	unbounded_int u = string2unbounded_int("-478");
	unbounded_int v = string2unbounded_int("-6632");

	unbounded_int prod = unbounded_int_produit(u, v);
	bool result = unbounded_int_cmp_ll(prod, 3170096) == 0;
	if(result) {
		free_unbounded_int(&prod);
		prod = unbounded_int_produit(v, u);
		result = unbounded_int_cmp_ll(prod, 3170096) == 0;
	}

	free_unbounded_int(&prod);
	free_unbounded_int(&u);
	free_unbounded_int(&v);
	return result;
}
