#include "shell.h"
#include "utils.h"

int main(int argc, char** argv) {
  static char cmd[CMD_LEN];

  // create shell
  shell_t shell;
  init_shell(&shell);

  // parse command line arguments
  cmd_args args = parse_args(argc, argv);
  bool change_destination = false;

  // start shell listener
  while (should_be_open(cmd)) {
    // close destination if not stdout
    if (change_destination) close_destination(&shell);

    // get input
    print_prompt(args.prompt);
    get_input(cmd);

    // change output destination if needed
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