#include "builtins.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils.h"

static builtin_val get_process_id(pid_t process_id) {
  // count number of digits in process id
  size_t res_size = num_digits(process_id) + 1;

  // store process id as char array
  char* out = (char*)malloc(res_size * sizeof(char));
  snprintf(out, res_size, "%d", process_id);

  // set error to false
  builtin_val res = {out, false};
  return res;
}

builtin_val pid() { return get_process_id(getpid()); }

builtin_val ppid() { return get_process_id(getppid()); }

builtin_val cd(const char* path) {
  bool is_error = false;
  const char* out = 0;

  // set default path to home
  if (!path) path = getenv("HOME");

  // change directory
  if (chdir(path)) {
    out = strerror(errno);
    is_error = true;
  }

  builtin_val res = {out, is_error};
  return res;
}

builtin_val pwd() {
  size_t res_size = 1024;
  const char* out = 0;
  bool is_error = false;

  // get current working directory
  if (!(out = getcwd(0, res_size))) {
    const char* err = strerror(errno);
    out = err;
    is_error = true;
  }

  builtin_val res = {out, is_error};
  return res;
}

builtin_val set(const char* var, const char* value) {
  bool is_error = false;
  const char* err = 0;

  // set env var if value is not NULL
  if ((value && setenv(var, value, true)) || (!value && unsetenv(var))) {
    err = strerror(errno);
    is_error = true;
  }

  builtin_val res = {err, is_error};
  return res;
}

builtin_val get(const char* var) {
  const char* env = getenv(var);
  char* out = 0;
  if (env) {
    size_t res_size = strlen(env) + 1;
    out = (char*)malloc(res_size * sizeof(char));
    strncpy(out, env, res_size);
  }

  builtin_val res = {out, false};
  return res;
}
