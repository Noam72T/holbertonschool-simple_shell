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

    path = get_command_path(args[0], env);
    if (!path)
    {
        fprintf(stderr, "Command not found: %s\n", args[0]);
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
        if (execve(path, args, env) == -1)
        {
            perror("execve");
            free(path);
            free_args(args);
            exit(EXIT_FAILURE);
        }
    }
    else
        wait(&status);

    free_args(args);
    free(path);
}