
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Ejercicio 7. Escribir dos versiones, una con system(3) y otra con execvp(3), de un programa que
ejecute otro programa que se pasará como argumento por línea de comandos. En cada caso, se
debe imprimir la cadena “El comando terminó de ejecutarse” después de la ejecución. ¿En qué
casos se imprime la cadena? ¿Por qué?*/
/* execve.c */

int main(int argc, char *argv[])
{
    char *newargv[] = {"bash", "-c", NULL, NULL};

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file-to-exec>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    newargv[2] = argv[1];

    execvp("/bin/bash", newargv);
    printf("El comando termino de ejecutarse\n");

    perror("execvp"); /* execvp() returns only on error */
    exit(EXIT_FAILURE);
}
