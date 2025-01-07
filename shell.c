#include "shell.h"

/**
 * main - Simple shell
 * @ac: Number of arguments
 * @av: Array of arguments
 * @env: Array of environment variables
 * Return: 0, 1, 2 or exit value
 */
int main(int ac, char **av, char **env)
{
    size_t bufsize = 0;
    char *buffer = NULL;
    int nb_cmd = 1, status = 0;

    if (!isatty(STDIN_FILENO))
    {
        non_interactive(av[0], buffer, bufsize, nb_cmd, env, &status);
        return (status);
    }

    while (1)
    {
        interactive(av[0], &buffer, &bufsize, nb_cmd, env, &status);
        nb_cmd++;
    }

    (void)ac;
    return (0);
}

/**
 * interactive - Execute the simple shell in interactive mode
 * @name: Name of the executable
 * @lineptr: Buffer for getline input
 * @bufsize: Size of the buffer
 * @nb_cmd: Number of commands executed
 * @env: Environment variables
 * @status: Execution status
 */
void interactive(char *name, char **lineptr, size_t *bufsize, int nb_cmd, char **env, int *status)
{
    int nb;

    printf("$ ");
    nb = getline(lineptr, bufsize, stdin);

    if (nb == -1) /* getline failed */
    {
        printf("\n");
        free(*lineptr);
        exit(*status);
    }

    if (nb > 0) /* Remove newline character */
        (*lineptr)[nb - 1] = '\0';

    if (**lineptr && !is_empty(*lineptr))
        parse_cmd(*lineptr, name, nb_cmd, env, status);

    free(*lineptr);
    *lineptr = NULL;
}

/**
 * non_interactive - Execute the simple shell in non-interactive mode
 * @name: Name of the executable
 * @buffer: Buffer for getline input
 * @bufsize: Size of the buffer
 * @nb_cmd: Number of commands executed
 * @env: Environment variables
 * @status: Execution status
 */
void non_interactive(char *name, char *buffer, size_t bufsize, int nb_cmd, char **env, int *status)
{
    int nb;
    while ((nb = getline(&buffer, &bufsize, stdin)) >= 0)
    {
        if (nb > 0)
            buffer[nb - 1] = '\0';

        if (*buffer && !is_empty(buffer))
            parse_cmd(buffer, name, nb_cmd, env, status);
    }

    free(buffer);
}

/**
 * is_empty - Checks if a string is empty or contains only spaces
 * @str: The string to check
 * Return: 1 if empty, 0 otherwise
 */
int is_empty(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] != ' ')
            return 0;
    }
    return 1;
}
