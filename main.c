#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define BUFFER_SIZE 1024

void prompt(void);
ssize_t read_input(char **line, size_t *len);
void execute_command(char *line);
char *trim_whitespace(char *str);

/**
 * main - Fonction principale de l'interpréteur de commande
 * Return: 0 si tout se passe bien, 1 en cas d'erreur
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
        if (read == -1) /* EOF detected */
        {
            printf("\n");
            break;
        }

        /* Remove leading/trailing spaces */
        line = trim_whitespace(line);
        if (strlen(line) == 0) /* Ignore empty input */
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
 * read_input - Lit l'entrée de l'utilisateur
 * @line: Pointeur vers le buffer
 * @len: Pointeur vers la taille du buffer
 * Return: Nombre de caractères lus, ou -1 en cas d'EOF
 */
ssize_t read_input(char **line, size_t *len)
{
    return (getline(line, len, stdin));
}

/**
 * execute_command - Exécute une commande en utilisant execve
 * @line: La commande à exécuter
 */
void execute_command(char *line)
{
    pid_t pid;
    int status;
    char *argv[2];

    argv[0] = line;
    argv[1] = NULL;

    /* Vérifier l'existence de la commande avant de tenter de l'exécuter */
    if (access(argv[0], F_OK) == -1)
    {
        perror("./shell");
        return; /* Command not found */
    }

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
            /* Command not found */
            perror("./shell");
            _exit(EXIT_FAILURE); /* Exit child process */
        }
    }
    else
    {
        waitpid(pid, &status, 0); /* Wait for child process to finish */
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

    /* Supprimer les espaces en début */
    while (*str == ' ')
        str++;

    /* Si la chaîne est vide */
    if (*str == '\0')
        return (str);

    /* Supprimer les espaces en fin */
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ')
        end--;

    /* Ajouter le terminateur */
    *(end + 1) = '\0';

    return (str);
}