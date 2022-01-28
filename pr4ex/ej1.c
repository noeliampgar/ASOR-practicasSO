
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
/*Ejercicio 1. Escribir un programa que emule el comportamiento de la shell en la ejecución de una
sentencia en la forma: comando1 argumento1 | comando2 argumento2. El programa creará una
tubería sin nombre y creará un hijo:
    El proceso padre redireccionará la salida estándar al extremo de escritura de la tubería y
    ejecutará comando1 argumento1.
    El proceso hijo redireccionará la entrada estándar al extremo de lectura de la tubería y
    ejecutará comando2 argumento2.
Probar el funcionamiento con una sentencia similar a: ./ejercicio1 echo 12345 wc -c
Nota: Antes de ejecutar el comando correspondiente, deben cerrarse todos los descriptores no
necesarios.*/
int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;

    if (argc < 5)
    {
        fprintf(stderr, "There should be 5 args.\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    { /*
        char *command = malloc(sizeof(argv[3]) + sizeof(argv[4]) + 1);
        strcpy(command, argv[3]);
        strcat(command, " ");
        strcat(command, argv[4]);
        printf("command: %s\n", command);*/

        if (dup2(pipefd[0], 0) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]); /* Close unused write end */
        close(pipefd[1]);

        /*if (system(command) == -1)
        {
            perror("system");
            exit(EXIT_FAILURE);
        }*/
        sleep(1);
        if (execlp(argv[3], argv[3], argv[4], NULL) == -1)
        {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }
    else
    { /* Parent writes argv[1] to pipe */ /*
        char *command = malloc(sizeof(argv[1]) + sizeof(argv[2]) + 1);
        strcpy(command, argv[1]);
        strcat(command, " ");
        strcat(command, argv[2]);
        printf("command: %s\n", command);*/

        if (dup2(pipefd[1], 1) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]); /* Close unused read end */

        close(pipefd[1]);
        /*  if (system(command) == -1)
        {
            perror("system");
            exit(EXIT_FAILURE);
        }*/
        sleep(1);
        if (execlp(argv[1], argv[1], argv[2], NULL) == -1)
        {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
        sleep(2);
    }
    exit(EXIT_SUCCESS);
}
