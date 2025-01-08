#include "main.h"

#define BUFFER_SIZE 1024

void prompt(void);
ssize_t read_input(char **line, size_t *len);
void execute_command(char *line);
char *trim_whitespace(char *str);

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
	return (1);
}

/**
 * prompt - Prompt du shell
 */
void prompt(void)
{
	printf("#cisfun$ ");
}

/**
 * read_input - Li les input
 * @line: Pointeur vers le buffer
 * @len: Pointeur vers la taille buffer 
 * Return: Nombre de caractères lus, ou -1 sur EOF
 */
ssize_t read_input(char **line, size_t *len)
{
	return (getline(line, len, stdin));
}

/**
 * execute_command - Exécute une commande en utilisant execve
 * @line: La commande d'éxécution
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char *argv[2];

	argv[0] = line;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

/**
 * trim_whitespace - Supprime les espaces en début et en fin de chaîne
 * @str: La chaîne à nettoyer
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
