#include "main.h"

#define BUFFER_SIZE 1024

extern char **environ;

void prompt(void);
ssize_t read_input(char **line, size_t *len);
void execute_command(char *line);

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
		if (strlen(line) == 0)
			continue;
		execute_command(line);
	}
	free(line);
	return (0);
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

	line[strcspn(line, "\n")] = '\0';
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