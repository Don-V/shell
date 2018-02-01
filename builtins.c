#include "builtins.h"
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

builtin_val cd(const char* path) {
  //   puts(path);
  if (!path) path = getenv("HOME");
  chdir(path);
  return 0;
}

builtin_val pwd() {
  size_t res_size = 1024;
  char* res = (char*)malloc(res_size * sizeof(char));
  // TODO: handle error
  return getcwd(res, res_size);
}

builtin_val set(const char* var, const char* value) {
  if (value)
    setenv(var, value, true);
  else
    unsetenv(var);
  return 0;
}

builtin_val get(const char* var) {
  const char* env = getenv(var);
  if (!env) env = " ";

  size_t res_size = strlen(env) + 1;
  char* res = (char*)malloc(res_size * sizeof(char));
  strncpy(res, env, res_size);
  return res;
}
