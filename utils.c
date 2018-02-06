#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
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

bool remove_ampersand(const char *cmd[]) {
  const char **c = cmd;

  while (*c) {
    cmd = c++;
  }

  if (*cmd && strncmp("&", *cmd, 2) == 0) {
    *cmd = 0;
    return true;
  }

  return false;
}

int pipe_destination(char cmd[], char **dest_loc) {
  char *curr = cmd;
  // search for file pipe character
  while (*curr) {
    if (strncmp(curr, " > ", 3) == 0) break;
    curr++;
  }

  // return if file pipe character not found
  if (!*curr) {
    *dest_loc = 0;
    return 0;
  }

  // set cmd to end of cmd before file pipe character
  cmd = curr;
  bool amp = false;

  // move cursor to start of pipe destination
  curr += 3;

  // ignore whitespace
  while (*curr++ == ' ')
    ;

  // start at the beginning of the filename
  char *start = --curr;

  // get length of pipe destination filename
  int len = 0;
  while (*curr) {
    if (*curr == ' ' &&
        ((*(curr + 1) != '&') || (*(curr + 1) && *(curr + 2) != 0))) {
      len = -1;
      break;
    }
    if (len > 0 && *(curr - 1) == ' ' && *curr == '&' && *(curr + 1) == 0) {
      amp = true;
      len--;
      break;
    }
    curr++;
    len++;
  }

  // return if there is more than a file name
  if (len == -1) {
    *dest_loc = 0;
    return -1;
  }

  // save filename to location pointer
  *dest_loc = malloc(len + 1);
  char *dest = *dest_loc;
  strncpy(dest, start, len);
  dest[len] = 0;

  if (amp) {
    cmd++;
    *(cmd)++ = '&';
  }
  // strip everything after the command
  *cmd = 0;
  return 1;
}