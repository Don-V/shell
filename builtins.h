#ifndef BUILTIN_H
#define BUILTIN_H

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

builtin_val cd();
builtin_val pwd();
builtin_val set(const char* var, const char* value);
builtin_val get(const char* var);

#endif