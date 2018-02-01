#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int num_digits(int num) {
  int res = 0;
  while (num) {
    num /= 10;
    res++;
  }

  return res;
}

static builtin_val get_process_id(pid_t process_id) {
  size_t res_size = num_digits(process_id) + 1;
  char* res = (char*)malloc(res_size * sizeof(char));
  snprintf(res, res_size, "%d", process_id);
  return res;
}

builtin_val pid() { return get_process_id(getpid()); }
builtin_val ppid() { return get_process_id(getppid()); }
// builtin_val cd();
// builtin_val pwd();
// builtin_val set(const char* var, const char* value);
// builtin_val get(const char* var);
