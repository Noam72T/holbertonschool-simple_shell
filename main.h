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
ssize_t get_input(char **line, size_t *len)
void execute_command(char *line);


#endif
