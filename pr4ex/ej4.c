#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>/*Ejercicio 4. Escribir un programa que abra la tubería con el nombre anterior en modo sólo escritura,
y escriba en ella el primer argumento del programa. En otro terminal, leer de la tubería usando un
comando adecuado.*/

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int op = open("./tub", O_WRONLY);

    if (op == -1)
    {
        perror("error open");
        exit(EXIT_FAILURE);
    }

    int wr = write(op, argv[1], strlen(argv[1]));

    if (wr == -1)
    {
        perror("error write");
        exit(EXIT_FAILURE);
    }
    close(op);
    exit(EXIT_SUCCESS);
}