#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "main.h"

void prompt(void)
{
    printf("#cisfun$ ");
    fflush(stdout);
}

ssize_t get_input(char **line)
{
    size_t len = 0;
    return getline(line, &len, stdin);
}

void execute_command(char *line)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        if (execve(line, (char *[]){line, NULL}, environ) == -1)
        {
            perror("./shell");
            exit(1);
        }
    }
    else
        wait(NULL);
}

int main(void)
{
    char *line = NULL;
    ssize_t read;

    while (1)
    {
        prompt();
        read = get_input(&line);
        if (read == -1)
        {
            if (feof(stdin))
                break;
            perror("getline");
            continue;
        }
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0)
            continue;
        execute_command(line);
    }
    free(line);
    return 0;
}
