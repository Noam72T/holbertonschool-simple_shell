#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


extern char **environ;

void prompt(void);
ssize_t read_input(char **line, size_t *len);
void execute_command(char *line);
char *trim_whitespace(char *str);
char *resolve_command(char *command);

#endif
