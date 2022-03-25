#include "unbounded_int.h"

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
