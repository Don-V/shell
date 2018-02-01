#ifndef BUILTIN_H
#define BUILTIN_H

/// Return value of a builtin command handler. Must be freed
typedef const char* builtin_val;

/**
 * Returns the process id of the shell
 * \return shell output of the process id
 */
builtin_val pid();

/**
 * Returns the process id of the parent process
 * \return shell output of the parent's process id
 */
builtin_val ppid();

/**
 * Changes the current working directory
 * \param path path to the new working directory. Defaults to /home directory if
 * none is specified
 * \return NULL
 */
builtin_val cd(const char* path);

/**
 * Returns the absolute path of the working directory
 * \return absolute path of working directory
 */
builtin_val pwd();

/**
 * Sets or clears the environment value
 * \param var environment variable to be set
 * \param value value of environment variable. The environment variable is
 * deleted if this is NULL
 * \return NULL
 */
builtin_val set(const char* var, const char* value);

/**
 * Returns the value of the environment variable
 * \param var environment variable whose value is returned
 * \return value of the environment variable
 */
builtin_val get(const char* var);

#endif