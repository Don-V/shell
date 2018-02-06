#ifndef SHELL_H
#define SHELL_H
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include "builtins.h"
#include "linked_list/list.h"

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
typedef struct output_dest {
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
 * Details of a background process ran by the shell
 */
typedef struct process {
  /**
   * Name of the process
   */
  char* name;

  /**
   * Process ID of the process
   */
  pid_t pid;
} process_t;

typedef struct shell {
  /**
   * Output destination of the shell
   */
  DEST dest;

  /**
   * Background processes currently active
   */
  List jobs;
} shell_t;

/**
 * Sets up the output and error destination of the shell to
 * stdout and stderr respectively
 * \param shell the shell being initialized
 */
void init_shell(shell_t* shell);

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
 * \param shell the shell
 * \param cmd input command
 * \return true if command was handled
 */
bool handle_builtin(const shell_t* shell, const char* cmd);

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

/**
 * Runs a process
 * \param dest the destination of the output of the process
 * \param cmd the command string of the process to be run
 */
void handle_process(const shell_t* shell, const char* cmd[]);

/**
 * Prints the status of a process to a destination
 * \param dest the destination of the output
 * \param pid the process id of the process
 * \param cmd_path the name of the process
 * \param status status of dead process
 */
void print_status(DEST dest, pid_t pid, const char* const cmd_path, int status);

/**
 * Checks for background processes and
 * outputs their exit status if any exists
 * \param shell the shell
 */
void check_for_dead_processes(shell_t* shell);

/**
 * Searches for a background process with the process of of pid
 * \param l the list being searched
 * \param pid the process id of the process
 * \return the name of the process if found else 0
 */
const char* find_process(List* l, pid_t pid);

/**
 * Prints out the current background processes
 * \shell the shell running the processes
 */
void list_background_processes(const shell_t* shell);

/**
 * Format output
 * \param dest destination of output
 * \param format string format to be written to output
 */
void write_format(FILE* dest, const char* format, ...);

/**
 * Sets the destination of the output
 * \param shell the shell
 * \param cmd the command being run
 * \return true if destination was changed from default
 */
bool set_output_destination(shell_t* shell, char* cmd);

/**
 * Closes the output destinations
 * \param shell the shell
 */
void close_destination(shell_t* shell);

/**
 * Adds a process name and id to the list of background processes
 * \param list of processes
 * \param name name of process
 * \param pid process id
 */
void add_background_process(List* l, const char* name, pid_t pid);

/**
 * Handles buitlin for 'jobs' command
 * \param shell the shell
 * \param cmd the command being run
 * \return true if the jobs command was executed else false
 */
bool jobs_builtin(shell_t* shell, const char* cmd);

#endif
