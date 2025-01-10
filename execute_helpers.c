#include "main.h"

/**
 * execute_fork - Handles the process forking and executing the command
 * @args: The command arguments
 * @env: The environment variables
 * @name: The name of the program
 */
void execute_fork(char **args, char **env, char *name)
{
    char *path = get_command_path(args[0], env);
    pid_t child_pid;
    int status;

    if (!path)
    {
        fprintf(stderr, "%s :Command not found: %s\n", name, args[0]);
        free_args(args);
        return;
    }

    child_pid = fork();
    if (child_pid == -1)
    {
        perror(name);
        free_args(args);
        free(path);
        return;
    }

    if (child_pid == 0 && execve(path, args, env) == -1)
    {
        perror(name);
        free(path);
        free_args(args);
        exit(EXIT_FAILURE);
    }

    wait(&status);
    free_args(args);
    free(path);
}
