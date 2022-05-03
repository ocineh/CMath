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
