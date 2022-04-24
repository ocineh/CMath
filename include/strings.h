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

/**
 * Get the index of the first occurrence of the character in the string
 *
 * @param s a string
 * @param c a character
 * @return an index in the string or -1 if not found
 */
size_t index_of(const char *s, char c);

#endif //PROJET_STRINGS_H
