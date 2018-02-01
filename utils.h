#ifndef UTILS_H
#define UTILS_H

/**
 * Returns the number of digits in a number
 * \param num number to be checked
 * \return the number of digits
 */
int num_digits(int num);

/**
 * Trims leading and trailing white space from a char buffer
 * \param str char buffer to be trimmed
 * \return pointer to the first char of the buffer or 0 for NULL buffers
 */
char *trim(char *str);

/**
 * Counts the number of times a string occurs in another
 * \param str string to be searched
 * \param sub string being search for
 */
int count(const char *str, char sub);

/**
 * Splits a string into a list of strings based on a delimeter
 * Must be freed
 * \param str the string to the split
 * \param delim the delimeter to be used
 * \return a pointer to the list.
 */
const char **split(char *str, char delim);

#endif