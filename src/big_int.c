#include <utils.h>
#include <malloc.h>
#include <stdlib.h>
#include "big_int.h"

static void push_back(big_int *u, char n) {
	digit *p_chiffre = malloc(sizeof(digit));
	if(p_chiffre != NULL) {
		p_chiffre->c = n;
		p_chiffre->prev = NULL;
		p_chiffre->next = NULL;

		++u->len;
		if(u->head == NULL) u->head = u->tail = p_chiffre;
		else {
			p_chiffre->prev = u->tail;
			u->tail->next = p_chiffre;
			u->tail = p_chiffre;
		}
	} else free_big_int(u);
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

static void push_front(big_int *u, int n) {
	digit *p_chiffre = malloc(sizeof(digit));
	if(p_chiffre != NULL) {
		p_chiffre->c = itoc(n);
		p_chiffre->next = p_chiffre->prev = NULL;

		if(u->head == NULL) {
			u->head = u->tail = p_chiffre;
		} else {
			p_chiffre->next = u->head;
			u->head->prev = p_chiffre;
			u->head = p_chiffre;
		}
		++u->len;
	} else free_big_int(u);
}

static big_int strip_big_int(big_int u) {
	if(u.len > 1 && u.head->c == '0') {
		digit *actual = u.head;
		while(actual != NULL && actual->c == '0') {
			actual = actual->next;
			--u.len;
		}
		if(actual == NULL) return ZERO;

		digit *precedent = actual->prev;
		while(precedent != NULL) {
			digit *tmp = precedent;
			precedent = precedent->prev;
			free(tmp);
		}

		actual->prev = NULL;
		u.head = actual;
	}
	return u;
}

big_int string_to_big_int(const char *str) {
	big_int result = NaN;
	char *tmp = strip(str);
	size_t len = strlen(tmp), i = 0;
	if(len > 0) {
		if(strchr("+-", tmp[i])) result.sign = tmp[i++];
		else if(isdigit(tmp[i])) result.sign = '+';
		else {
			free(tmp);
			return result; // NaN
		}

		while(tmp[i] != '\0') {
			if(isdigit(tmp[i])) push_back(&result, tmp[i++]);
			else {
				free_big_int(&result);
				break;
			}
		}
	}
	free(tmp);
	return strip_big_int(result);
}

big_int ll_to_big_int(long long int i) {
	if(i == 0) return ZERO;
	long long n = i < 0 ? -i : i;
	unsigned len = i < 0;
	while(n > 0) ++len, n /= 10;

	char tmp[len + 1];
	sprintf(tmp, "%lli", i);
	return string_to_big_int(tmp);
}

char *big_int_to_string(big_int i) {
	if(isNaN(i)) return copy("NaN");
	size_t len = i.len + (i.sign == '-') + 1, j = 0;
	char *res = calloc(len, sizeof(char));
	if(i.sign == '-') res[j++] = '-';

	for(digit *p = i.head; p != NULL; p = p->next)
		res[j++] = p->c;
	return res;
}

int big_int_cmp_big_int(big_int a, big_int b) {
	if(isNaN(a) || isNaN(b)) return -2; // when NaN == ? && ? == NaN && NaN == NaN

	// if the signs are different
	if(a.sign != b.sign) {
		if(a.sign == '-') return -1; // then b.sign == '+'
		if(a.sign == '+') return 1; // then b.sign == '-'
	}

	// If the lengths are different, it depends on the sign
	if(a.len != b.len) {
		if(a.len < b.len) return a.sign == '-' ? 1 : -1;
		return a.sign == '-' ? -1 : 1;
	}

	// If the length and the sign are the same, it depends on the digits.
	digit *p_a = a.head, *p_b = b.head;
	while(p_a != NULL && p_b != NULL) {
		int tmp = cpm(p_a->c, p_b->c);
		if(tmp != 0) // if the digits are different, it depends on the sign
			return a.sign == '-' ? -tmp : tmp;

		p_a = p_a->next;
		p_b = p_b->next;
	}
	return 0;
}

static int cmp_abs(big_int a, big_int b) {
	a.sign = b.sign = '+';
	return big_int_cmp_big_int(a, b);
}

int big_int_cmp_ll(big_int a, long long int b) {
	big_int tmp = ll_to_big_int(b);
	int result = big_int_cmp_big_int(a, tmp);
	free_big_int(&tmp);
	return result;
}

static big_int add_same_sign(big_int *a, big_int *b) {
	if(a->sign == b->sign) {
		// if the signs are the same, we just need to sum the digits and take care of the hold.
		big_int r = { .sign=a->sign, .len=0, .head=NULL, .tail=NULL };
		int hold = 0, tmp;

		digit *p_a = a->tail, *p_b = b->tail;
		while(p_a != NULL && p_b != NULL) {
			tmp = ctoi(p_a->c) + ctoi(p_b->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p_a = p_a->prev;
			p_b = p_b->prev;
		}

		// In the case an big_int is longer than the other.
		digit *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			tmp = ctoi(p->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p = p->prev;
		}

		// In the case the hold is not 0.
		if(hold != 0) push_front(&r, hold);
		return r;
	}
	return NaN;
}

static big_int add_diff_sign(big_int *a, big_int *b) {
	if(a->sign != b->sign) {
		// If the signs are different, we need to calculate the difference between the two.
		big_int r = NaN;
		// The sign of the result is the sign of the bigger big_int.
		r.sign = (cmp_abs(*a, *b) == 1 ? a : b)->sign;
		int hold = 0, tmp;

		digit *p_a = a->tail, *p_b = b->tail;
		while(p_a != NULL && p_b != NULL) {
			int i = ctoi(p_a->c), j = ctoi(p_b->c);
			if(i == j) tmp = hold < 0 ? -(20 + hold) : hold;
			else if(i < j) tmp = -10 - (10 - (j - i + -hold));
			else tmp = i - j + hold;

			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p_a = p_a->prev;
			p_b = p_b->prev;
		}

		// In the case an big_int is longer than the other.
		digit *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			int i = ctoi(p->c);
			if(hold < 0 && -hold > i) tmp = -10 - (10 - (i + -hold));
			else tmp = i + hold;
			hold = tmp / 10;
			tmp %= 10;
			push_front(&r, tmp);
			p = p->prev;
		}

		// In the case the hold is not 0.
		if(hold != 0) push_front(&r, hold);
		return r;
	}
	return NaN;
}

big_int big_int_sum(big_int a, big_int b) {
	if(a.sign == '*' || b.sign == '*') return NaN;
	if(isZERO(a)) return copy_big_int(&b);
	if(isZERO(b)) return copy_big_int(&a);

	int cmp = cmp_abs(a, b);
	// If the signs are the same, we just need to sum the digits and take care of the hold.
	if(a.sign == b.sign) return strip_big_int(add_same_sign(&a, &b));
	if(a.sign != b.sign) { // if the signs are different,
		// we can return 0 if |a| == |b|
		if(cmp == 0) return ZERO;
		// or we need to calculate the difference between the two.
		if(cmp == -1) return strip_big_int(add_diff_sign(&b, &a));
		if(cmp == 1) return strip_big_int(add_diff_sign(&a, &b));
	}
	return NaN;
}

big_int big_int_diff(big_int a, big_int b) {
	// a - b = a + (-b) or a - (-b) = a + b
	b.sign = b.sign == '-' ? '+' : '-';
	big_int r = big_int_sum(a, b);
	return r;
}

static void free_chiffre(digit *c) {
	digit *actual = c;
	while(actual != NULL) {
		digit *tmp = actual;
		actual = actual->next;
		free(tmp);
	}
}

void free_big_int(big_int *a) {
	free_chiffre(a->head);
	a->head = NULL;
	a->tail = NULL;
	a->sign = '*';
	a->len = 0;
}

big_int copy_big_int(big_int *a) {
	if(a == NULL) return NaN;
	big_int result = { .sign=a->sign, .len=0, .head=NULL, .tail=NULL };
	for(digit *actual = a->head; actual != NULL; actual = actual->next)
		push_back(&result, actual->c);
	return result;
}

static void left_shift(big_int *u, long long unsigned n) {
	if(!isZERO(*u))
		while(n-- > 0)
			push_back(u, '0');
}

static void pop_back(big_int *u) {
	if(u->tail != NULL) {
		if(u->len == 1) {
			u->head->c = '0';
			return;
		} else {
			u->tail = u->tail->prev;
			free_chiffre(u->tail->next);
			u->tail->next = NULL;
			--u->len;
		}
	}
}

static void right_shift(big_int *u, long long unsigned n) {
	if(!isZERO(*u))
		while(n-- > 0 && u->len > 0)
			pop_back(u);
}

static big_int big_int_fois_chiffre(big_int *a, digit *c) {
	if(a == NULL || isNaN((*a))) return NaN;
	if(c->c == '0' || isZERO((*a))) return ZERO;
	if(c->c == '1') return copy_big_int(a);

	const int coef = ctoi(c->c);
	big_int result = { .sign=a->sign, .len=0, .head=NULL, .tail=NULL };

	digit *c_a = a->tail;
	int res, hold = 0;
	while(c_a != NULL) {
		res = ctoi(c_a->c) * coef + hold;
		hold = res / 10;
		push_front(&result, res % 10);
		c_a = c_a->prev;
	}

	if(hold != 0) push_front(&result, hold);
	return result;
}

big_int big_int_product(big_int a, big_int b) {
	if(isNaN(a) || isNaN(b)) return NaN;
	if(isZERO(a) || isZERO(b)) return ZERO;

	big_int result = ZERO, inter, tmp;
	result.sign = (a.sign != b.sign && (a.sign == '-' || b.sign == '-')) ? '-' : '+';

	digit *c_b = b.tail;
	long long unsigned rand = 0;
	while(c_b != NULL) {
		if(c_b->c != '0') {
			inter = big_int_fois_chiffre(&a, c_b);
			inter.sign = result.sign;
			left_shift(&inter, rand);

			tmp = big_int_sum(result, inter);
			free_big_int(&inter);
			free_big_int(&result);
			result = tmp;
		}

		++rand;
		c_b = c_b->prev;
	}
	return result;
}

big_int big_int_pow(big_int u, big_int n) {
	if(isNaN(u) || isNaN(n) || n.sign == '-') return NaN;
	if(isZERO(n)) return string_to_big_int("1");
	if(isZERO(u)) return ZERO;
	n = copy_big_int(&n);

	big_int result = copy_big_int(&u), pas = string_to_big_int("-1"), tmp;
	while(n.len > 1 || n.head->c > '1') {
		tmp = big_int_product(result, u); // result = result * u
		free_big_int(&result);
		result = tmp;

		tmp = big_int_sum(n, pas); // n = n - 1
		free_big_int(&n);
		n = tmp;
	}

	free_big_int(&n);
	free_big_int(&pas);
	return result;
}

static big_int big_int_division(big_int a, big_int b, bool modulo) {
	if(isNaN(a) || isNaN(b) || isZERO(b)) return NaN;
	if(isZERO(a)) return ZERO;
	if(isONE(b)) {
		if(b.sign == '-') a.sign = '-';
		return modulo ? ZERO : copy_big_int(&a);
	}

	bool minus = a.sign != b.sign;
	int cmp = cmp_abs(a, b);
	if(cmp == -1) return modulo ? copy_big_int(&a) : ZERO;
	if(cmp == 0) return modulo ? ZERO : string_to_big_int(minus ? "-1" : "1");

	big_int remain = copy_big_int(&a);
	big_int pas = copy_big_int(&b);
	remain.sign = b.sign = pas.sign = '+';

	big_int count = string_to_big_int("1");
	if(!modulo) left_shift(&count, remain.len - pas.len);
	left_shift(&pas, remain.len - pas.len);

	big_int quotient = string_to_big_int("0"), tmp;
	while(big_int_cmp_big_int(remain, b) >= 0) {
		if(big_int_cmp_big_int(remain, pas) == -1) {
			right_shift(&pas, 1);
			if(!modulo) right_shift(&count, 1);
			continue;
		}

		tmp = big_int_diff(remain, pas);
		free_big_int(&remain);
		remain = tmp;

		if(!modulo) {
			tmp = big_int_sum(quotient, count);
			free_big_int(&quotient);
			quotient = tmp;
		}
	}

	quotient.sign = minus ? '-' : '+';
	remain.sign = isZERO(remain) ? '+' : a.sign;
	big_int result = copy_big_int(modulo ? &remain : &quotient);

	free_big_int(&count);
	free_big_int(&quotient);
	free_big_int(&remain);
	free_big_int(&pas);
	return result;
}

big_int big_int_quotient(big_int a, big_int b) {
	return big_int_division(a, b, false);
}

big_int big_int_modulo(big_int a, big_int b) {
	return big_int_division(a, b, true);
}
