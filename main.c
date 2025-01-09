#include "main.h"

/**
 * main - Entry point for the simple shell program
 * @ac: Argument count
 * @av: Argument vector
 * @env: Environment variables
 *
 * Return: 0 on success
 */
int main(int ac, char **av, char **env)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t read_size;

    (void)ac;
    (void)av;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "uwu:) ", 6);

        read_size = getline(&input, &input_size, stdin);
        if (read_size == -1)
        {
            printf("\n");
            exit(0);
        }

        if (input[read_size - 1] == '\n')
            input[read_size - 1] = '\0';

        if (strcmp("exit", input) == 0)
        {
            free(input);
            exit(0);
        }

        execute_command(input, env);
    }

    free(input);
    return (0);
}