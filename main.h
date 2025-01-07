#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define PROMPT "$shell "

void display_prompt(void);
ssize_t read_input(char **line, size_t *len);

void execute_command(char *line);

/**
 * main - Entry point of the shell program
 *
 * Return: 0 on success, or an error code on failure.
 */
int main(void)
{
    char *line = NULL;

    size_t len = 0;
    ssize_t nread;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            display_prompt();

        nread = read_input(&line, &len);
        if (nread == -1)
            break;

        if (line[0] == '\0')
            continue;

        execute_command(line);
    }

    free(line);
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "\n", 1);

    return (0);
}

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

/**
 * read_input - Reads input from the user.
 * @line: Pointer to the buffer to store input.
 * @len: Pointer to the size of the buffer.
 *
 * Return: Number of characters read, or -1 on error or EOF.
 */
ssize_t read_input(char **line, size_t *len)
{
    ssize_t nread = getline(line, len, stdin);

    if (nread > 0 && (*line)[nread - 1] == '\n')
        (*line)[nread - 1] = '\0';

    return (nread);
}

/**
 * execute_command - Executes a given command.
 * @line: The command to execute.
 */
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
        if (execl(line, line, NULL) == -1)
        {
            perror(line);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;

        waitpid(pid, &status, 0);
    }
}