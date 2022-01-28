
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

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /*if (sys >= 0) {
                   fprintf(stderr, "Command NULL and/or shell not available or shell could not be exec in child\n");
                   exit(EXIT_FAILURE);
               }*/
    int sys;
    sys = system(argv[1]);
    if (sys == -1)
    {
        perror("System");
        exit(EXIT_FAILURE);
    }
    printf("El comando termino de ejecutarse\n");

    exit(EXIT_SUCCESS);
}
