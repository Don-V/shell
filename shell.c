#include "shell.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "builtins.h"

int main(int argc, char** argv) {
  static char cmd[CMD_LEN];

  // parse command line arguments
  cmd_args args = parse_args(argc, argv);

  // start shell
  while (should_be_open(cmd)) {
    // get input
    print_prompt(args.prompt);
    get_input(cmd);

    // process input

    // test - to be removes
    print_prompt(args.prompt);
    printf("You typed '%s'\n", cmd);
  }

  return 0;
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
}