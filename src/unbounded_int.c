#include <unbounded_int.h>
#include <utils.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

static void push_back(unbounded_int *u, char n) {
	chiffre *p_chiffre = malloc(sizeof(chiffre));
	if(p_chiffre != NULL) {
		p_chiffre->c = n;
		p_chiffre->precedent = NULL;
		p_chiffre->suivant = NULL;

		++u->len;
		if(u->premier == NULL) u->premier = u->dernier = p_chiffre;
		else {
			p_chiffre->precedent = u->dernier;
			u->dernier->suivant = p_chiffre;
			u->dernier = p_chiffre;
		}
	} else abort();
}

static int ctoi(const char c) {
	return c - '0';
}

static char itoc(int i) {
	return (char) (abs(i) + '0');
}

static int cpm(char a, char b) {
	int i = ctoi(a), j = ctoi(b);
	if(i == -1 || j == -1) return -2;
	return i < j ? -1 : i > j ? 1 : 0;
}

static void push_front(unbounded_int *u, int n) {
	chiffre *p_chiffre = malloc(sizeof(chiffre));
	if(p_chiffre != NULL) {
		p_chiffre->c = itoc(n);
		p_chiffre->suivant = p_chiffre->precedent = NULL;

		if(u->premier == NULL) {
			u->premier = u->dernier = p_chiffre;
		} else {
			p_chiffre->suivant = u->premier;
			u->premier->precedent = p_chiffre;
			u->premier = p_chiffre;
		}
		++u->len;
	} else abort();
}

static unbounded_int strip_unbounded_int(unbounded_int u) {
	if(u.len > 1 && u.premier->c == '0') {
		chiffre *actual = u.premier;
		while(actual != NULL && actual->c == '0') {
			actual = actual->suivant;
			--u.len;
		}
		if(actual == NULL) return ZERO;

		chiffre *precedent = actual->precedent;
		while(precedent != NULL) {
			chiffre *tmp = precedent;
			precedent = precedent->precedent;
			free(tmp);
		}

		actual->precedent = NULL;
		u.premier = actual;
	}
	return u;
}

unbounded_int string2unbounded_int(const char *str) {
	unbounded_int result = NaN;
	char *tmp = strip(str);
	size_t len = strlen(tmp), i = 0;
	if(len > 0) {
		if(strchr("+-", tmp[i])) result.signe = tmp[i++];
		else if(isdigit(tmp[i])) result.signe = '+';
		else {
			free(tmp);
			return result; // NaN
		}

		while(tmp[i] != '\0') {
			if(isdigit(tmp[i])) push_back(&result, tmp[i++]);
			else {
				free_unbounded_int(&result);
				break;
			}
		}
	}
	free(tmp);
	return strip_unbounded_int(result);
}

unbounded_int ll2unbounded_int(long long int i) {
	if(i == 0) return ZERO;
	long long n = i < 0 ? -i : i;
	unsigned len = i < 0;
	while(n > 0) ++len, n /= 10;

	char tmp[len + 1];
	sprintf(tmp, "%lli", i);
	return string2unbounded_int(tmp);
}

char *unbounded_int2string(unbounded_int i) {
	if(isNaN(i)) return copy("NaN");
	size_t len = i.len + (i.signe == '-') + 1, j = 0;
	char *res = calloc(len, sizeof(char));
	if(i.signe == '-') res[j++] = '-';

	for(chiffre *p = i.premier; p != NULL; p = p->suivant)
		res[j++] = p->c;
	return res;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
	if(isNaN(a) || isNaN(b)) return -2; // when NaN == ? && ? == NaN && NaN == NaN

	// if the signs are different
	if(a.signe != b.signe) {
		if(a.signe == '-') return -1; // then b.signe == '+'
		if(a.signe == '+') return 1; // then b.signe == '-'
	}

	// If the lengths are different, it depends on the signe
	if(a.len != b.len) {
		if(a.len < b.len) return a.signe == '-' ? 1 : -1;
		return a.signe == '-' ? -1 : 1;
	}

	// If the length and the signe are the same, it depends on the digits.
	chiffre *p_a = a.premier, *p_b = b.premier;
	while(p_a != NULL && p_b != NULL) {
		int tmp = cpm(p_a->c, p_b->c);
		if(tmp != 0) // if the digits are different, it depends on the signe
			return a.signe == '-' ? -tmp : tmp;

		p_a = p_a->suivant;
		p_b = p_b->suivant;
	}
	return 0;
}

static int cmp_abs(unbounded_int a, unbounded_int b) {
	a.signe = b.signe = '+';
	return unbounded_int_cmp_unbounded_int(a, b);
}

int unbounded_int_cmp_ll(unbounded_int a, long long int b) {
	unbounded_int tmp = ll2unbounded_int(b);
	int result = unbounded_int_cmp_unbounded_int(a, tmp);
	free_unbounded_int(&tmp);
	return result;
}

static unbounded_int add_same_sign(unbounded_int *a, unbounded_int *b) {
	if(a->signe == b->signe) {
		// if the signs are the same, we just need to sum the digits and take care of the hold.
		unbounded_int r = { .signe=a->signe, .len=0, .premier=NULL, .dernier=NULL };
		int hold = 0, tmp;

		chiffre *p_a = a->dernier, *p_b = b->dernier;
		while(p_a != NULL && p_b != NULL) {
			tmp = ctoi(p_a->c) + ctoi(p_b->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p_a = p_a->precedent;
			p_b = p_b->precedent;
		}

		// In the case an unbounded_int is longer than the other.
		chiffre *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			tmp = ctoi(p->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p = p->precedent;
		}

		// In the case the hold is not 0.
		if(hold != 0) push_front(&r, hold);
		return r;
	}
	return NaN;
}

static unbounded_int add_diff_sign(unbounded_int *a, unbounded_int *b) {
	if(a->signe != b->signe) {
		// If the signs are different, we need to calculate the difference between the two.
		unbounded_int r = NaN;
		// The signe of the result is the signe of the bigger unbounded_int.
		r.signe = (cmp_abs(*a, *b) == 1 ? a : b)->signe;
		int hold = 0, tmp;

		chiffre *p_a = a->dernier, *p_b = b->dernier;
		while(p_a != NULL && p_b != NULL) {
			int i = ctoi(p_a->c), j = ctoi(p_b->c);
			if(i == j) tmp = hold < 0 ? -(20 + hold) : hold;
			else if(i < j) tmp = -10 - (10 - (j - i + -hold));
			else tmp = i - j + hold;

			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p_a = p_a->precedent;
			p_b = p_b->precedent;
		}

		// In the case an unbounded_int is longer than the other.
		chiffre *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			int i = ctoi(p->c);
			if(hold < 0 && -hold > i) tmp = -10 - (10 - (i + -hold));
			else tmp = i + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p = p->precedent;
		}

		// In the case the hold is not 0.
		if(hold != 0) push_front(&r, hold);
		return r;
	}
	return NaN;
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
	if(a.signe == '*' || b.signe == '*') return NaN;
	if(isZERO(a)) return copy_unbounded_int(&b);
	if(isZERO(b)) return copy_unbounded_int(&a);

	int cmp = cmp_abs(a, b);
	// If the signs are the same, we just need to sum the digits and take care of the hold.
	if(a.signe == b.signe) return strip_unbounded_int(add_same_sign(&a, &b));
	if(a.signe != b.signe) { // if the signs are different,
		// we can return 0 if |a| == |b|
		if(cmp == 0) return ZERO;
		// or we need to calculate the difference between the two.
		if(cmp == -1) return strip_unbounded_int(add_diff_sign(&b, &a));
		if(cmp == 1) return strip_unbounded_int(add_diff_sign(&a, &b));
	}
	return NaN;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
	// a - b = a + (-b) or a - (-b) = a + b
	b.signe = b.signe == '-' ? '+' : '-';
	unbounded_int r = unbounded_int_somme(a, b);
	return r;
}

static void free_chiffre(chiffre *c) {
	chiffre *actual = c;
	while(actual != NULL) {
		chiffre *tmp = actual;
		actual = actual->suivant;
		free(tmp);
	}
}

void free_unbounded_int(unbounded_int *a) {
	free_chiffre(a->premier);
	a->premier = NULL;
	a->dernier = NULL;
	a->signe = '*';
	a->len = 0;
}

unbounded_int copy_unbounded_int(unbounded_int *a) {
	if(a == NULL) return NaN;
	unbounded_int result = { .signe=a->signe, .len=0, .premier=NULL, .dernier=NULL };
	for(chiffre *actual = a->premier; actual != NULL; actual = actual->suivant)
		push_back(&result, actual->c);
	return result;
}

static void left_shift(unbounded_int *u, long long unsigned n) {
	if(!isZERO(*u))
		while(n-- > 0)
			push_back(u, '0');
}

static void pop_back(unbounded_int *u) {
	if(u->dernier != NULL) {
		if(u->len == 1) {
			u->premier->c = '0';
			return;
		} else {
			u->dernier = u->dernier->precedent;
			free_chiffre(u->dernier->suivant);
			u->dernier->suivant = NULL;
			--u->len;
		}
	}
}

static void right_shift(unbounded_int *u, long long unsigned n) {
	if(!isZERO(*u))
		while(n-- > 0 && u->len > 0)
			pop_back(u);
}

static unbounded_int unbounded_int_fois_chiffre(unbounded_int *a, chiffre *c) {
	if(a == NULL || isNaN((*a))) return NaN;
	if(c->c == '0' || isZERO((*a))) return ZERO;
	if(c->c == '1') return copy_unbounded_int(a);

	const int coef = ctoi(c->c);
	unbounded_int result = { .signe=a->signe, .len=0, .premier=NULL, .dernier=NULL };

	chiffre *c_a = a->dernier;
	int res, hold = 0;
	while(c_a != NULL) {
		res = ctoi(c_a->c) * coef + hold;
		hold = res / 10;
		push_front(&result, res % 10);
		c_a = c_a->precedent;
	}

	if(hold != 0) push_front(&result, hold);
	return result;
}

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
	if(isNaN(a) || isNaN(b)) return NaN;
	if(isZERO(a) || isZERO(b)) return ZERO;

	unbounded_int result = ZERO, inter, tmp;
	result.signe = (a.signe != b.signe && (a.signe == '-' || b.signe == '-')) ? '-' : '+';

	chiffre *c_b = b.dernier;
	long long unsigned rand = 0;
	while(c_b != NULL) {
		if(c_b->c != '0') {
			inter = unbounded_int_fois_chiffre(&a, c_b);
			inter.signe = result.signe;
			left_shift(&inter, rand);

			tmp = unbounded_int_somme(result, inter);
			free_unbounded_int(&inter);
			free_unbounded_int(&result);
			result = tmp;
		}

		++rand;
		c_b = c_b->precedent;
	}
	return result;
}

unbounded_int unbounded_int_pow(unbounded_int u, unbounded_int n) {
	if(isNaN(u) || isNaN(n) || n.signe == '-') return NaN;
	if(isZERO(n)) return string2unbounded_int("1");
	if(isZERO(u)) return ZERO;
	n = copy_unbounded_int(&n);

	unbounded_int result = copy_unbounded_int(&u), pas = string2unbounded_int("-1"), tmp;
	while(n.len > 1 || n.premier->c > '1') {
		tmp = unbounded_int_produit(result, u); // result = result * u
		free_unbounded_int(&result);
		result = tmp;

		tmp = unbounded_int_somme(n, pas); // n = n - 1
		free_unbounded_int(&n);
		n = tmp;
	}

	free_unbounded_int(&n);
	free_unbounded_int(&pas);
	return result;
}

static unbounded_int unbounded_int_division(unbounded_int a, unbounded_int b, bool modulo) {
	if(isNaN(a) || isNaN(b) || isZERO(b)) return NaN;
	if(isZERO(a)) return ZERO;
	if(isONE(b)) {
		if(b.signe == '-') a.signe = '-';
		return modulo ? ZERO : copy_unbounded_int(&a);
	}

	bool minus = a.signe != b.signe;
	int cmp = cmp_abs(a, b);
	if(cmp == -1) return modulo ? copy_unbounded_int(&a) : ZERO;
	if(cmp == 0) return modulo ? ZERO : string2unbounded_int(minus ? "-1" : "1");

	unbounded_int remain = copy_unbounded_int(&a);
	unbounded_int pas = copy_unbounded_int(&b);
	remain.signe = b.signe = pas.signe = '+';

	unbounded_int count = string2unbounded_int("1");
	if(!modulo) left_shift(&count, remain.len - pas.len);
	left_shift(&pas, remain.len - pas.len);

	unbounded_int quotient = string2unbounded_int("0"), tmp;
	while(unbounded_int_cmp_unbounded_int(remain, b) >= 0) {
		if(unbounded_int_cmp_unbounded_int(remain, pas) == -1) {
			right_shift(&pas, 1);
			if(!modulo) right_shift(&count, 1);
			continue;
		}

		tmp = unbounded_int_difference(remain, pas);
		free_unbounded_int(&remain);
		remain = tmp;

		if(!modulo) {
			tmp = unbounded_int_somme(quotient, count);
			free_unbounded_int(&quotient);
			quotient = tmp;
		}
	}

	quotient.signe = minus ? '-' : '+';
	remain.signe = isZERO(remain) ? '+' : a.signe;
	unbounded_int result = copy_unbounded_int(modulo ? &remain : &quotient);

	free_unbounded_int(&count);
	free_unbounded_int(&quotient);
	free_unbounded_int(&remain);
	free_unbounded_int(&pas);
	return result;
}

unbounded_int unbounded_int_quotient(unbounded_int a, unbounded_int b) {
	return unbounded_int_division(a, b, false);
}

unbounded_int unbounded_int_modulo(unbounded_int a, unbounded_int b) {
	return unbounded_int_division(a, b, true);
}
