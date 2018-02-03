#include "shell.h"
#include "utils.h"

int main(int argc, char** argv) {
  static char cmd[CMD_LEN] = {0};
  DEST dest = {stdout, stderr};
  // parse command line arguments
  cmd_args args = parse_args(argc, argv);

  // start shell
  while (should_be_open(cmd)) {
    // get input
    print_prompt(args.prompt);
    get_input(cmd);

    // process input
    if (handle_builtin(dest, cmd)) continue;
    const char** input_args = split(cmd, ' ');

    handle_process(dest, input_args);
    check_for_dead_processes(dest);
  }

  return 0;
}