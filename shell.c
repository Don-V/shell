#include "shell.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "builtins.h"
#include "utils.h"

void init_shell(shell_t* shell) {
  DEST dest = {stdout, stderr};
  shell->dest = dest;
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

bool handle_builtin(DEST dest, const char* cmd) {
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

  if (strncmp("exit", cmd, 5) == 0) return true;

  return false;
}

void handle_process(DEST dest, const char* cmd[]) {
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
      write_to_out(dest.err, msg);
      exit(-1);
    }
  }

  // check for failed creation of child
  if (ch_pid == -1) {
    write_to_out(dest.err, strerror(errno));
    free((void*)cmd);
    return;
  }

  // parent act
  // 4 for square brackets, space and null terminator
  int status_len = num_digits(ch_pid) + 4 + strlen(cmd[0]);
  char child_status[status_len];
  child_status[status_len - 1] = 0;
  snprintf(child_status, status_len, "[%d] %s", ch_pid, cmd[0]);
  write_to_out(dest.out, child_status);

  // check for background process
  if (!bg_process) {
    // get child status
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
      // TODO: out
      printf("[%d] %s Exit %d\n", ch_pid, cmd[0], WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {  // or just else
                                       // TODO: out
      printf("[%d] %s Killed (%d)\n", ch_pid, cmd[0], WTERMSIG(status));
    }
  }

  free((void*)cmd);
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

void check_for_dead_processes(DEST dest) {
  int status;
  pid_t pid;
  if ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    printf("Background process %d\n", pid);
  }
}