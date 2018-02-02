#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int num_digits(int num) {
  int res = 0;
  while (num) {
    num /= 10;
    res++;
  }

  return res;
}

char *trim(char *str) {
  size_t len = 0;
  char *frontp = str;
  char *endp = 0;

  if (str == 0) {
    return 0;
  }
  if (str[0] == '\0') {
    return str;
  }

  len = strlen(str);
  endp = str + len;

  /* Move the front and back pointers to address the first non-whitespace
   * characters from each end.
   */
  while (isspace((unsigned char)*frontp)) {
    ++frontp;
  }
  if (endp != frontp) {
    while (isspace((unsigned char)*(--endp)) && endp != frontp) {
    }
  }

  if (str + len - 1 != endp)
    *(endp + 1) = '\0';
  else if (frontp != str && endp == frontp)
    *str = '\0';

  /* Shift the string so that it starts at str so that if it's dynamically
   * allocated, we can still free it on the returned pointer.  Note the reuse
   * of endp to mean the front of the string buffer now.
   */
  endp = str;
  if (frontp != str) {
    while (*frontp) {
      *endp++ = *frontp++;
    }
    *endp = '\0';
  }

  return str;
}

int count(const char *str, char sub) {
  int len = 0;
  while (*str) {
    if (*(str++) == sub) len++;
  }
  return len;
}

const char **split(char *str, char delim) {
  if (!str) return 0;
  int len = count(str, delim) + 1;
  const char **res = (const char **)malloc(sizeof(char *) * (len + 1));
  res[0] = strtok(str, &delim);
  int i = 1;
  while ((res[i++] = strtok(0, &delim)))
    ;

  return res;
}