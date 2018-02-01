#ifndef SHELL_H
#define SHELL_H
#include <stdbool.h>
#include <stdio.h>
#include "builtins.h"

// Red color constant
#ifndef KRED
#define KRED "\x1B[31m"
#endif

#define CMD_LEN 200

#define RESET "\x1B[0m"

/**
 * Structure for command line arguments
 */
typedef struct command_line_arguments {
  const char* prompt;
} cmd_args;

/**
 * Destination file of output
 */
typedef struct {
  /**
   * Destination of standard out
   */
  FILE* out;

  /**
   * Destination of stderr
   */
  FILE* err;
} DEST;

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

/**
 * Handles builtin commands and returns true if command was builtin
 * \param dest destination of output of builtin handler
 * \param cmd input command
 * \return true if command was handled
 */
bool handle_builtin(DEST dest, const char* cmd);

/**
 * Writes to the current output destination
 * \param dest destination of output
 * \param out string to be written to output
 */
void write_to_out(FILE* dest, const char* out);

/**
 * Processes builtin output and prints it
 * \param dest destination of output
 * \param res string to be written to output
 */
bool process_builtin_out(DEST dest, builtin_val res);

#endif