#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_input(char *line, size_t len);
void execute_command(char *line);

int main(void)
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        printf("$ ");
        fflush(stdout);
        if (getline(&line, &len, stdin) == -1)
        {
            if (feof(stdin))
            {
                printf("\n");
                break;
            }
            perror("getline");
            continue;
        }
        handle_input(line, len);
    }

    free(line);
    return (0);
}

void handle_input(char *line, size_t len)
{
    if (line[len - 1] == '\n')
        line[len - 1] = '\0';
    execute_command(line);
}

void execute_command(char *line)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        /* Create an array of pointers to char with the command */
        char *argv[2];
        argv[0] = line; /* The first argument is the command itself */
        argv[1] = NULL; /* The last element must be NULL */

        if (execvp(argv[0], argv) == -1)
        {
            perror(line);
            exit(1);
        }
    }
    else
    {
        wait(NULL); /* Wait for the child process to finish */
    }
}
