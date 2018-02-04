#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

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

/**
 * Checks for a trailing ampersand, and removes it
 * \param cmd the string buffer being check
 * \return true if trailing ampersand was found and removed
 */
bool remove_ampersand(const char *cmd[]);

/**
 * Checks if the command output should be piped
 * Strips the command of the pipe character and pipe destination
 * Writes the destination file name to dest param
 * \param cmd the command being checked
 * \param dest_loc the address of the filename buffer
 * filename will be malloc'd(must be freed)
 * \return the status
 *  if *dest_loc is 0 and return is 0,there is no pipe,
 *  if *dest_loc is 0 and return is -1, there is an error
 *  if *dest_loc is not 0, return is 1, strip was successful
 */
int pipe_destination(char cmd[], char **dest);

#endif