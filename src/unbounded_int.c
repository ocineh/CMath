#include <unbounded_int.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

void print_unbounded_int(unbounded_int *u) {
	if(u->signe == '*') printf("NaN\n");
	else {
		printf("%c", u->signe);
		chiffre *actual = u->premier;
		while(actual != NULL) {
			printf("%c", actual->c);
			actual = actual->suivant;
		}
		printf("\n");
	}
}

static void add_chiffre(unbounded_int *u, char n) {
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
		
		while(i < len && isdigit(e[i])) add_chiffre(&result, e[i++]);
	}
	return result;
}

unbounded_int ll2unbounded_int(long long int i) {
	if(i == 0) return string2unbounded_int("0");
	long long n = i;
	unsigned count = 0;
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
	char *res = malloc(sizeof(char) * (i.len + 2));
	res[i.len + 1] = '\0';
	res[0] = i.signe;
	
	chiffre *p_chiffre = i.premier;
	for(int j = 1; j <= i.len && p_chiffre != NULL; ++j) {
		res[j] = p_chiffre->c;
		p_chiffre = p_chiffre->suivant;
	}
	return res;
}

static short ctoi(const char c) {
	switch(c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default: return -1;
	}
}

static short itoc(const short i) {
	switch(abs(i)) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		default: return '*';
	}
}

static int cpm(char a, char b) {
	short i = ctoi(a), j = ctoi(b);
	if(i < j) return -1;
	if(i > j) return 1;
	return 0;
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

static int cmp_abs(unbounded_int *a, unbounded_int *b) {
	char tmp_a = a->signe, tmp_b = b->signe;
	a->signe = b->signe = '+';
	int tmp = unbounded_int_cmp_unbounded_int(*a, *b);
	a->signe = tmp_a;
	b->signe = tmp_b;
	return tmp;
}

int unbounded_int_cmp_ll(unbounded_int a, long long int b) {
	unbounded_int tmp = ll2unbounded_int(b);
	return unbounded_int_cmp_unbounded_int(a, tmp);
}

static void add_begin(unbounded_int *u, short n) {
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

static unbounded_int add_same_sign(unbounded_int *a, unbounded_int *b) {
	if(a->signe == b->signe) {
		unbounded_int r = { .signe=a->signe, .len=0, .premier=NULL, .dernier=NULL };
		short hold = 0, tmp;
		
		chiffre *p_a = a->dernier, *p_b = b->dernier;
		while(p_a != NULL && p_b != NULL) {
			tmp = ctoi(p_a->c) + ctoi(p_b->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			add_begin(&r, tmp);
			p_a = p_a->precedent;
			p_b = p_b->precedent;
		}
		
		chiffre *p = p_a == NULL ? p_b : p_a;
		while(p != NULL) {
			tmp = ctoi(p->c) + hold;
			hold = tmp / 10;
			tmp %= 10;
			add_begin(&r, tmp);
			p = p->precedent;
		}
		
		if(hold != 0) add_begin(&r, hold);
		return r;
	}
	return NaN;
}

static unbounded_int add_diff_sign(unbounded_int *a, unbounded_int *b) {
	if(a->signe != b->signe) {
		unbounded_int r = { .signe= (char) (cmp_abs(a, b) == 1 ? a->signe
															   : b->signe), .len=0, .premier=NULL, .dernier=NULL };
		short hold = 0, tmp = 0;
		
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
			add_begin(&r, tmp);
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
			add_begin(&r, tmp);
			p = p->precedent;
		}
		
		if(hold != 0) add_begin(&r, hold);
		return r;
	}
	return NaN;
}

static unbounded_int strip(unbounded_int u) {
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

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b) {
	if(a.signe == '*' || b.signe == '*') return NaN;
	if(isZERO(a)) return b;
	if(isZERO(b)) return a;
	
	int cmp = cmp_abs(&a, &b);
	if(a.signe != b.signe && cmp == 0) return ZERO;
	
	if(a.signe == b.signe) return strip(add_same_sign(&a, &b));
	if(a.signe != b.signe) {
		if(cmp == 0) return ZERO;
		if(cmp == -1) return strip(add_diff_sign(&b, &a));
		if(cmp == 1) return strip(add_diff_sign(&a, &b));
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