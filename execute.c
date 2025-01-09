#include "main.h"

/**
 * execute_command - Executes the given command
 * @command: The command to execute
 * @env: The environment variables
 */
void execute_command(char *command, char **env)
{
    char **args, *path;
    pid_t child_pid;
    int status;

    args = tokenize_input(command);
    if (!args || !args[0])
    {
        free_args(args);
        return;
    }

    /* Check for the built-in exit command */
    if (strcmp(args[0], "exit") == 0)
    {
        free_args(args);
        exit(0); // Exit the shell
    }

    path = get_command_path(args[0], env);
    if (!path)
    {
        fprintf(stderr, "Error: Command not found: %s\n", args[0]);
        free_args(args);
        return;
    }

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        free_args(args);
        free(path);
        return;
    }
    if (child_pid == 0)
    {
        /* In child process */
        if (execve(path, args, env) == -1)
        {
            perror("execve");
            free(path);
            free_args(args);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* In parent process */
        wait(&status);
    }

    free_args(args);
    free(path);
}
