#include "shell.h"
#include "utils.h"

int main(int argc, char** argv) {
  static char cmd[CMD_LEN] = {0};
  shell_t shell;
  init_shell(&shell);
  // parse command line arguments
  cmd_args args = parse_args(argc, argv);
  bool change_destination = false;

  // start shell
  while (should_be_open(cmd)) {
    if (change_destination) close_destination(&shell);
    // get input
    print_prompt(args.prompt);
    get_input(cmd);

    change_destination = set_output_destination(&shell, cmd);

    // handle builtin
    if (handle_builtin(&shell, cmd)) {
      check_for_dead_processes(&shell);
      continue;
    }

    if (jobs_builtin(&shell, cmd)) continue;
    const char** input_args = split(cmd, ' ');

    // handle new process
    handle_process(&shell, input_args);
    check_for_dead_processes(&shell);
  }

  return 0;
}