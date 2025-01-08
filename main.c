#include "main.h"

#define BUFFER_SIZE 1024


void prompt(void);
ssize_t read_input(char **line, size_t *len);
void execute_command(char *line);
char *trim_whitespace(char *str);
char *resolve_command(char *command);

/**
 * main - Function main interpréteur de commande
 * Return: 0 si bon, 1 si erreur
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		prompt();
		read = read_input(&line, &len);
		if (read == -1) 
		{
			printf("\n");
			break;
		}


		line = trim_whitespace(line);
		if (strlen(line) == 0) 
			continue;

		execute_command(line);
	}
	free(line);
	return (0);
}

/**
 * prompt - Affiche le prompt du shell
 */
void prompt(void)
{
	printf("#cisfun$ ");
}

/**
 * read_input - Lis l'entrée utilisateur
 * @line: Pointeur vers le buffer de ligne
 * @len: Taille du buffer
 * Return: Nombre de caractères lus ou -1 sur EOF
 */
ssize_t read_input(char **line, size_t *len)
{
	return (getline(line, len, stdin));
}

/**
 * execute_command - Exécute une commande via execve
 * @line: La commande à exécuter
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char *argv[2];
	char *command;

	
	command = resolve_command(line);
	if (command == NULL)
	{
		fprintf(stderr, "%s: command not found\n", line);
		return;
	}

	argv[0] = command;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(command);
		return;
	}
	if (pid == 0) 
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror(argv[0]);
			free(command);
			exit(EXIT_FAILURE);
		}
	}
	else 
	{
		waitpid(pid, &status, 0);
		free(command);
	}
}

/**
 * trim_whitespace - Supprime les espaces en début et en fin de chaîne
 * @str: Chaîne à nettoyer
 * Return: Pointeur vers la chaîne nettoyée
 */
char *trim_whitespace(char *str)
{
	char *end;

	while (*str == ' ') 
		str++;

	if (*str == '\0')
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && *end == ' ') 
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * resolve_command - Trouve le chemin complet d'une commande
 * @command: Commande à résoudre
 * Return: Chemin absolu ou NULL si introuvable
 */
char *resolve_command(char *command)
{
	char *path = getenv("PATH");
	char *token, *full_path;
	size_t len;

	if (path == NULL)
		return (NULL);

	token = strtok(path, ":");
	while (token != NULL)
	{
		len = strlen(token) + strlen(command) + 2;
		full_path = malloc(len);
		if (full_path == NULL)
			return (NULL);

		sprintf(full_path, "%s/%s", token, command);
		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		token = strtok(NULL, ":");
	}
	return (NULL);
}
