#include "main.h"

/**
 * tokenize_input - Splits the input into tokens
 * @input: The input string
 * Return: Array of strings (tokens)
 */
char **tokenize_input(char *input)
{
	char **tokens, *token;
	int token_count = 0;

	tokens = malloc(sizeof(char *) * MAX_TOKENS);
	if (!tokens)
		return (NULL);

	token = strtok(input, " \t\n");
	while (token && token_count < MAX_TOKENS - 1)
	{
		tokens[token_count] = strdup(token);
		if (!tokens[token_count])
		{
			free_args(tokens);
			return (NULL);
		}
		token_count++;
		token = strtok(NULL, " \t\n");
	}
	tokens[token_count] = NULL;

	return (tokens);
}

/**
 * free_args - Frees the argument array
 * @args: The argument array to free
 */
void free_args(char **args)
{
	int i = 0;

	if (!args)
		return;

	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/**
 * print_env - Prints the current environment variables
 * @env: Array of environment variables
 */
void print_env(char **env)
{
	int i = 0;

	if (!env)
		return;

	while (env[i])
	{
		write(STDOUT_FILENO, env[i], strlen(env[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * get_command_path - Gets the full path of a command
 * @command: The command to find
 * @env: The environment variables
 * Return: Full path of the command or NULL if not found
 */
char *get_command_path(char *command, char **env)
{
	char *path, *path_copy, *dir, *full_path;
	struct stat st;
	int i = 0;

	if (strchr(command, '/') && stat(command, &st) == 0)
		return (strdup(command));

	while (env[i] && strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);

	path_copy = strdup(env[i] + 5);
	if (!path_copy)
		return (NULL);

	for (dir = strtok(path_copy, ":"); dir; dir = strtok(NULL, ":"))
	{
		full_path = malloc(strlen(dir) + strlen(command) + 2);
		if (!full_path)
			break;
		sprintf(full_path, "%s/%s", dir, command);
		if (stat(full_path, &st) == 0)
			return (free(path_copy), full_path);
		free(full_path);
	}
	free(path_copy);
	return (NULL);
}
