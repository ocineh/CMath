#ifndef PROJET_STRINGS_H
#define PROJET_STRINGS_H

#include <stdio.h>
#include <stdbool.h>

/**
 * Returns the length of the string.
 *
 * @param s a string
 * @return the length of the string
 */
extern size_t strlen(const char *s);

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
extern int index_of(const char *s, char c);

/**
 * Get the index of the last occurrence of the character in the string
 *
 * @param s a string
 * @param c a character
 * @return an index in the string or -1 if not found
 */
extern int last_index_of(const char *s, char c);

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
 * Returns if the string is a number
 *
 * @param s a string
 * @return true if the string is a number
 */
extern bool is_number(const char *s);

extern char *__concat__(char *s, ...);
#define concat(...) __concat__(__VA_ARGS__, NULL)

/**
 * Returns if the character is a digit or an arithmetic operator.
 *
 * @param c a character
 * @return if the character is a digit or an arithmetic operator
 */
extern bool is_arithmetic(char c);

/**
 * Return if a string is a valid arithmetic expression.
 *
 * @param s a string
 * @return if the string is a valid arithmetic expression
 */
extern bool is_arithmetic_expression(const char *s);

extern bool is_digit(char c);

extern char *remove_spaces(char *s);

extern bool is_empty(const char *s);

#endif //PROJET_STRINGS_H
