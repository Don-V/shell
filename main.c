#include "shell.h"
#include "utils.h"

int main(int argc, char** argv) {
  static char cmd[CMD_LEN] = {0};
  shell_t shell;
  init_shell(&shell);
  // parse command line arguments
  cmd_args args = parse_args(argc, argv);

  // start shell
  while (should_be_open(cmd)) {
    // get input
    print_prompt(args.prompt);
    get_input(cmd);

    // process input
    if (handle_builtin(shell.dest, cmd)) continue;
    const char** input_args = split(cmd, ' ');

    handle_process(&shell, input_args);
    check_for_dead_processes(&shell);
  }

  return 0;
}