#ifndef PROJET_LIBRARY_H
#define PROJET_LIBRARY_H

#include <stddef.h>

void hello(void);

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

#endif //PROJET_LIBRARY_H
