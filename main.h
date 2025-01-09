#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_TOKENS 64

void execute_command(char *command, char **env);
char **tokenize_input(char *input);
void free_args(char **args);
char *get_command_path(char *command, char **env);
void print_env(char **env);

#endif