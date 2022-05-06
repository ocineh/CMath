#include <unbounded_int.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

static void push_back(unbounded_int *u, char n) {
	chiffre *p_chiffre = malloc(sizeof(chiffre));
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
}

static short ctoi(const char c) {
	return c - '0';
}

static char itoc(short i) {
	i = abs(i);
	if(i < 0 || i > 9) return '\0';
	return '0' + i;
}

static int cpm(char a, char b) {
	short i = ctoi(a), j = ctoi(b);
	if(i < j) return -1;
	if(i > j) return 1;
	return 0;
}

static void push_front(unbounded_int *u, short n) {
	chiffre *p_chiffre = malloc(sizeof(chiffre));
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

unbounded_int string2unbounded_int(const char *e) {
	unbounded_int result = { .signe='*', .len = 0 };
	size_t len = strlen(e);
	if(len > 0) {
		size_t i = 0;
		while(i < len && isspace(e[i])) ++i;

		if(e[i] == '-' || e[i] == '+') {
			result.signe = e[i];
			++i;
		} else if(isdigit(e[i])) result.signe = '+';
		else return result;

		while(i < len) {
			if(isdigit(e[i])) push_back(&result, e[i++]);
			else {
				free_unbounded_int(&result);
				return NaN;
			}
		}
	}
	return strip_unbounded_int(result);
}

unbounded_int ll2unbounded_int(long long int i) {
	if(i == 0) return string2unbounded_int("0");
	long long n = i < 0 ? -i : i;
	unsigned count = i < 0 ? 1 : 0;
	while(n > 0) {
		++count;
		n /= 10;
	}

	char tmp[count + 1];
	tmp[count] = '\0';
	sprintf(tmp, "%lli", i);
	return string2unbounded_int(tmp);
}

char *unbounded_int2string(unbounded_int i) {
	if(isNaN(i)) return concat("NaN");
	size_t len = i.len + (i.signe == '-') + 1;
	char *res = malloc(sizeof(char) * len);
	res[len - 1] = '\0';
	int j = 0;
	if(i.signe == '-') res[j++] = '-';
	chiffre *p_chiffre = i.premier;
	while(j <= i.len && p_chiffre != NULL) {
		res[j++] = p_chiffre->c;
		p_chiffre = p_chiffre->suivant;
	}
	return res;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
	if(a.signe == '-' && b.signe == '+') return -1;
	if(a.signe == '+' && b.signe == '-') return 1;

	if(a.len < b.len) return a.signe == '-' ? 1 : -1;
	if(a.len > b.len) return a.signe == '-' ? -1 : 1;

	chiffre *p_a = a.premier, *p_b = b.premier;
	while(p_a != NULL && p_b != NULL) {
		int tmp = cpm(p_a->c, p_b->c);
		if(tmp != 0)
			return a.signe == '-' ? -tmp : tmp;

		p_a = p_a->suivant;
		p_b = p_b->suivant;
	}
	return 0;
}

static int cmp_abs(unbounded_int a, unbounded_int b) {
	a.signe = b.signe = '+';
	int tmp = unbounded_int_cmp_unbounded_int(a, b);
	return tmp;
}

int unbounded_int_cmp_ll(unbounded_int a, long long int b) {
	unbounded_int tmp = ll2unbounded_int(b);
	return unbounded_int_cmp_unbounded_int(a, tmp);
}

static unbounded_int add_same_sign(unbounded_int *a, unbounded_int *b) {
	if(a->signe == b->signe) {
		unbounded_int r = { .signe=a->signe, .len=0, .premier=NULL, .dernier=NULL };
		short hold = 0, tmp;

		chiffre *p_a = a->dernier, *p_b = b->dernier;
		while(p_a != NULL && p_b != NULL) {
			tmp = ctoi(p_a->c) + ctoi(p_b->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p_a = p_a->precedent;
			p_b = p_b->precedent;
		}

		chiffre *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			tmp = ctoi(p->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p = p->precedent;
		}

		if(hold != 0) push_front(&r, hold);
		return r;
	}
	return NaN;
}

static unbounded_int add_diff_sign(unbounded_int *a, unbounded_int *b) {
	if(a->signe != b->signe) {
		unbounded_int r = {
				.signe= (char) (cmp_abs(*a, *b) == 1 ? a->signe : b->signe),
				.len=0,
				.premier=NULL,
				.dernier=NULL
		};
		short hold = 0, tmp;

		chiffre *p_a = a->dernier, *p_b = b->dernier;
		while(p_a != NULL && p_b != NULL) {
			short i = ctoi(p_a->c), j = ctoi(p_b->c);
			if(i == j) {
				if(hold < 0) tmp = -10 - (10 - ((j + abs(hold)) - i));
				else tmp = hold;
			} else if(i < j) tmp = -10 - (10 - ((j + abs(hold)) - i));
			else tmp = i - j + hold;

			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p_a = p_a->precedent;
			p_b = p_b->precedent;
		}

		chiffre *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			short i = ctoi(p->c);
			if(hold < 0 && -hold > i) tmp = -10 - (10 - (abs(hold) - i));
			else tmp = i + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p = p->precedent;
		}

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
	if(a.signe != b.signe && cmp == 0) return ZERO;

	if(a.signe == b.signe) return strip_unbounded_int(add_same_sign(&a, &b));
	if(a.signe != b.signe) {
		if(cmp == 0) return ZERO;
		if(cmp == -1) return strip_unbounded_int(add_diff_sign(&b, &a));
		if(cmp == 1) return strip_unbounded_int(add_diff_sign(&a, &b));
	}
	return NaN;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b) {
	a.signe = '+';
	b.signe = '-';
	unbounded_int r = unbounded_int_somme(a, b);
	r.signe = '+';
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
	if(!isZERO((*u)))
		while(n-- > 0)
			push_back(u, '0');
}

static void right_shift(unbounded_int *u, long long unsigned n) {
	if(!isZERO((*u))) {
		chiffre *actual = u->dernier;
		while(n-- > 0 && actual != NULL) {
			actual = actual->precedent;
			--u->len;
		}
		free_chiffre(actual->suivant);
		actual->suivant = NULL;
		u->dernier = actual;
	}
}

static unbounded_int unbounded_int_fois_chiffre(unbounded_int *a, chiffre *c) {
	if(a == NULL || isNaN((*a))) return NaN;
	if(c->c == '0' || isZERO((*a))) return ZERO;
	if(c->c == '1') return copy_unbounded_int(a);

	const short coef = ctoi(c->c);
	unbounded_int result = { .signe=a->signe, .len=0, .premier=NULL, .dernier=NULL };

	chiffre *c_a = a->dernier;
	short res, hold = 0;
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

	unbounded_int result = ZERO;
	result.signe = (a.signe != b.signe && (a.signe == '-' || b.signe == '-')) ? '-' : '+';

	chiffre *c_b = b.dernier;
	long long unsigned rand = 0;
	while(c_b != NULL) {
		unbounded_int inter = unbounded_int_fois_chiffre(&a, c_b);
		inter.signe = result.signe;
		left_shift(&inter, rand);

		unbounded_int tmp_bis = unbounded_int_somme(result, inter);
		free_unbounded_int(&inter);
		free_unbounded_int(&result);
		result = tmp_bis;

		++rand;
		c_b = c_b->precedent;
	}
	return result;
}

unbounded_int unbounded_int_pow(unbounded_int u, unbounded_int n) {
	if(isZERO(n)) return string2unbounded_int("1");
	if(n.signe == '-') return NaN;
	n = copy_unbounded_int(&n);

	unbounded_int result = copy_unbounded_int(&u);
	unbounded_int one = string2unbounded_int("1"), minus_one = string2unbounded_int("-1");
	while(unbounded_int_cmp_unbounded_int(n, one) == 1) {
		unbounded_int tmp = unbounded_int_produit(result, u);
		free_unbounded_int(&result);
		result = tmp;

		tmp = unbounded_int_somme(n, minus_one);
		free_unbounded_int(&n);
		n = tmp;
	}
	free_unbounded_int(&n);
	free_unbounded_int(&one);
	free_unbounded_int(&minus_one);
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
	remain.signe = b.signe = '+';

	unbounded_int count = string2unbounded_int("1");
	left_shift(&count, remain.len - pas.len);
	left_shift(&pas, remain.len - pas.len);

	unbounded_int quotient = string2unbounded_int("0"), tmp;
	while(unbounded_int_cmp_unbounded_int(remain, b) >= 0) {
		pas.signe = '+';
		while(unbounded_int_cmp_unbounded_int(remain, pas) == -1) {
			right_shift(&count, 1);
			right_shift(&pas, 1);
		}
		pas.signe = '-';

		tmp = unbounded_int_somme(remain, pas);
		free_unbounded_int(&remain);
		remain = tmp;

		tmp = unbounded_int_somme(quotient, count);
		free_unbounded_int(&quotient);
		quotient = tmp;
	}

	quotient.signe = minus ? '-' : '+';
	if(isZERO(remain)) remain.signe = '+';
	else remain.signe = a.signe;
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
