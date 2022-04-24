#ifndef PROJET_STRINGS_H
#define PROJET_STRINGS_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "unbounded_int.h"

/**
 * Remove spaces at start and end of the string
 *
 * @param c a string
 */
extern char *strip(char *c);

/**
 * Split the elements of the string with the separator
 *
 * @param line a string
 * @param separator a separator
 * @return an array of strings
 */
extern size_t split(char *line, char separator, char ***tokens);

#endif //PROJET_STRINGS_H
