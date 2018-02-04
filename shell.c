#include "shell.h"
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "builtins.h"
#include "utils.h"

static bool process_equals(const void* data1, const void* data2) {
  const process_t* process = (const process_t*)data1;
  const pid_t* pid = (const pid_t*)data2;
  return process->pid == *pid;
}

void init_shell(shell_t* shell) {
  DEST dest = {stdout, stderr};
  shell->dest = dest;
  list_init(&(shell->jobs), process_equals);
}

void print_prompt(const char* prompt) { printf("%s> ", prompt); }

cmd_args parse_args(int argc, char** argv) {
  cmd_args args;
  args.prompt = "308sh";

  for (int i = 1; i < argc; i++) {
    // check for -p flag
    if (strncmp("-p", argv[i], 3) == 0) {
      if (i + 1 < argc) {
        args.prompt = argv[i + 1];
      } else {
        fprintf(stderr, "%s %serror:%s %s", argv[0], KRED, RESET,
                "shell: -p must take a string argument\n");
      }
    }
  }

  return args;
}

bool should_be_open(const char* cmd) {
  const char* exit = "exit";
  return strncmp(exit, cmd, strlen(exit)) != 0;
}

void get_input(char* cmd) {
  fgets(cmd, CMD_LEN - 1, stdin);
  int len = strlen(cmd);
  if (len > 0) cmd[len - 1] = 0;
  trim(cmd);
}

bool handle_builtin(const shell_t* shell, const char* cmd) {
  DEST dest = shell->dest;
  if (strncmp("pid", cmd, 4) == 0) {
    builtin_val out_str = pid();
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("ppid", cmd, 5) == 0) {
    builtin_val out_str = ppid();
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("pwd", cmd, 4) == 0) {
    builtin_val out_str = pwd();
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("get ", cmd, 4) == 0) {
    size_t var_length = strlen(cmd) - 3;
    char var[var_length];
    strncpy(var, cmd + 4, var_length);

    builtin_val out_str = get(var);
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("cd", cmd, 3) == 0) {
    builtin_val out_str = cd(0);
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("cd ", cmd, 3) == 0) {
    size_t var_length = strlen(cmd) - 2;
    char path[var_length];
    strncpy(path, cmd + 3, var_length);

    builtin_val out_str = cd(path);
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("set ", cmd, 4) == 0) {
    size_t var_length = strlen(cmd) - 3;
    char var[var_length];
    strncpy(var, cmd + 4, var_length);

    const char* env_name = strtok(var, " ");
    const char* env_value = strtok(0, " ");
    builtin_val out_str = set(env_name, env_value);
    return process_builtin_out(dest, out_str);
  }

  if (strncmp("jobs", cmd, 5) == 0) {
    list_background_processes(shell);
    return true;  // process_builtin_out(dest, out_str);
  }

  if (strncmp("exit", cmd, 5) == 0) return true;

  return false;
}

void handle_process(const shell_t* shell, const char* cmd[]) {
  if (!cmd || !*cmd) return;
  bool bg_process = remove_ampersand(cmd);
  pid_t ch_pid = fork();

  // child act
  if (!ch_pid) {
    if (execvp(cmd[0], (char* const*)cmd)) {
      const char* err = strerror(errno);
      const char* prefix = "Cannot exec";

      // 4 for 3 spaces and null terminator;
      int len = strlen(err) + strlen(cmd[0]) + strlen(prefix) + 4;
      char msg[len];
      snprintf(msg, len, "%s %s: %s", prefix, cmd[0], err);
      free((void*)cmd);
      write_to_out(shell->dest.err, msg);
      exit(-1);
    }
  }

  // check for failed creation of child
  if (ch_pid == -1) {
    write_to_out(shell->dest.err, strerror(errno));
    free((void*)cmd);
    return;
  }

  // parent act
  // 4 for square brackets, space and null terminator
  int status_len = num_digits(ch_pid) + 4 + strlen(cmd[0]);
  char child_status[status_len];
  snprintf(child_status, status_len, "[%d] %s", ch_pid, cmd[0]);
  write_to_out(shell->dest.out, child_status);

  if (bg_process) {
    // add background process to list of background processes
    process_t* process = (process_t*)malloc(sizeof(process_t));
    size_t name_len = strlen(cmd[0]) + 1;
    process->name = (char*)malloc(name_len);
    strncpy(process->name, cmd[0], name_len);
    process->pid = ch_pid;
    enqueue((List*)&(shell->jobs), process);
  } else {
    // output child status
    int status;
    waitpid(ch_pid, &status, 0);
    print_status(shell->dest, ch_pid, cmd[0], status);
  }

  free((void*)cmd);
}

void print_status(DEST dest, pid_t pid, const char* const cmd_path,
                  int status) {
  if (WIFEXITED(status)) {
    // TODO: out
    write_format(dest.out, "[%d] %s Exit %d\n", pid, cmd_path,
                 WEXITSTATUS(status));
  } else if (WIFSIGNALED(status)) {  // or just else
                                     // TODO: out
    write_format((shell->dest).out, "[%d] %s Killed (%d)\n", pid, cmd_path,
                 WTERMSIG(status));
  }
}

bool process_builtin_out(DEST dest, builtin_val res) {
  if (res.error) {
    write_to_out(dest.err, res.value);
    return true;
  }
  write_to_out(dest.out, res.value);
  free((void*)res.value);
  return true;
}

void write_to_out(FILE* dest, const char* out) {
  if (out) fprintf(dest, "%s\n", out);
}

void check_for_dead_processes(shell_t* shell) {
  int status;
  pid_t pid = waitpid(-1, &status, WNOHANG);
  if (pid > 0) {
    const char* name = find_process(shell, pid);
    print_status(shell->dest, pid, name, status);
    free((void*)name);
  }
}

void write_format(FILE* dest, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(dest, format, args);
  va_end(args);
}

const char* find_process(shell_t* shell, pid_t pid) {
  process_t* found = list_remove(&(shell->jobs), &pid);
  if (found) {
    const char* name = found->name;
    free(found);
    return name;
  }
  return 0;
}

void list_background_processes(const shell_t* shell) {
  const process_t* job = (const process_t*)iterate((List*)&(shell->jobs), true);
  if (!job) {
    write_format((shell->dest).out, "No running processes\n");
    return;
  }
  write_format((shell->dest).out, "%40s | %10s\n", "process", "pid");
  write_format((shell->dest).out, "%40s | %10d\n", job->name, job->pid);
  while ((job = (const process_t*)iterate((List*)&(shell->jobs), false))) {
    write_format((shell->dest).out, "%40s | %10d\n", job->name, job->pid);
  }
}