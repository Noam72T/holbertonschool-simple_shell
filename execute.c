#include "main.h"


/**
 * execute_command - Executes the given command
 * @command: The command to execute
 * @env: The environment variables
 * @name: The name of the program
 */
void execute_command(char *command, char **env, char *name)
{
char **args;
args = tokenize_input(command);
if (!args || !args[0])
{
free_args(args);
return;
}
/* Check if the command is "exit" */
if (strcmp(args[0], "exit") == 0)
{
free_args(args);
exit(0);
}
if (strcmp(args[0], "env") == 0)
{
print_env(env);
free_args(args);
return;
}
execute_fork(args, env, name);
}
