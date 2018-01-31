#ifndef SHELL_H
#define SHELL_H
#include <stdbool.h>

// Red color constant
#ifndef KRED
#define KRED "\x1B[31m"
#endif

#define CMD_LEN 200

#define RESET "\x1B[0m"

typedef struct command_line_arguments {
  const char* prompt;
} cmd_args;

/**
 * Prints the prompt formatted like a shell prompt
 * \param prompt the prompt to be printed
 */
void print_prompt(const char* prompt);

/**
 * Gets input from stdin and stores it in cmd
 * \param cmd the array the input is stored
 */
void get_input(char* cmd);

/**
 * Parses the command line arguments and set defaults
 * \param argc number of command line arguments
 * \param argv command line arguments
 * \return command line options
 */
cmd_args parse_args(int argc, char** argv);

/**
 * Checks whether the prompt should be open based on the input
 * \param cmd the input command
 * \return true if the prompt should be open else false
 */
bool should_be_open(const char* cmd);

#endif