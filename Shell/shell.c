#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define PROMPT "uwu$ "
#define BUFFER_SIZE 1024

extern char **environ;


void prompt_and_read(char **line, size_t *len);
void execute_command(char *line);

/**
 * main - Shell Function
 *
 * Return: Always 0.
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        prompt_and_read(&line, &len);
        execute_command(line);
    }

    free(line);
    return (0);
}

/**
 * prompt_and_read - Affichage de l'invite et de l'entrée de lecture
 * @line: Pointer de la ligne
 * @len: Pointer de la longueur
 */
void prompt_and_read(char **line, size_t *len)
{
    ssize_t nread;

    /* Affichage de l'invite */
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

    /* Read input */
    nread = getline(line, len, stdin);

    /* Gérer EOF (Ctrl+D) */
    if (nread == -1)
    {
        if (*line)
            free(*line);
        write(STDOUT_FILENO, "\n", 1);
        exit(EXIT_SUCCESS);
    }

    /* Suppression de la nouvelle ligne de fin */
    (*line)[nread - 1] = '\0';
}

/**
 * execute_command - Exécute la commande donnée
 * @line: Commande a éxécuté
 */
void execute_command(char *line)
{
    pid_t pid;
    int status;
    char *args[] = {line, NULL};

    /* Duplication du process Enfant */
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        /* Process Enfant */
        if (execve(line, args, environ) == -1)
        {
            perror(line);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Process Parent */
        wait(&status);
    }
}
