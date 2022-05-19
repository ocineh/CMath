#ifndef PROJET_STRINGS_H
#define PROJET_STRINGS_H

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief      Concatenates a variable number of strings.
 * @param      ... A variable number of strings.
 */
#define concat(...) __concat__(__VA_ARGS__, NULL)

/**
 * @brief      Copy a string.
 * @param      s  The string
 */
#define copy(s) concat(s)

/**
 * Remove spaces at start and end of the string
 *
 * @param c a string
 */
extern char *strip(const char *c);

/**
 * Get the substring from the string
 *
 * @param s a string
 * @param begin the beginning of the substring
 * @param end the end of the substring
 * @return the substring
 */
extern char *substring(const char *s, size_t begin, size_t end);

/**
 * Concatenate a variable number of strings
 *
 * @param s a string
 * @param ... the strings to concatenate
 * @return the concatenation of the strings
 */
extern char *__concat__(const char *s, ...);

/**
 * Return if a string is a valid arithmetic expression.
 *
 * @param s a string
 * @return if the string is a valid arithmetic expression
 */
extern bool is_arithmetic_expression(const char *s);

/**
 * Remove all the spaces in the string
 *
 * @param s a string
 * @return the string without the spaces
 */
extern char *remove_spaces(const char *s);

extern bool is_empty(char *s);

#endif //PROJET_STRINGS_H
