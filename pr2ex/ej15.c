#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
/*Ejercicio 15. Escribir un programa que consulte y muestre en pantalla el estado del cerrojo sobre un
fichero usando lockf(3). El programa mostrará el estado del cerrojo (bloqueado o desbloqueado).
Además:
● Si está desbloqueado, fijará un cerrojo y escribirá la hora actual. Después suspenderá su
ejecución durante 30 segundos (con sleep(3)) y a continuación liberará el cerrojo.
● Si está bloqueado, terminará el programa.*/

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd;

    if ((fd = open(argv[1], O_RDWR | O_CREAT, 0666)) == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    off_t tam;

    if ((tam = lseek(fd, 0, SEEK_END)) == -1)
    {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    int cerr;
    cerr = lockf(fd, F_TEST, tam);
    if (cerr == -1)
    {
        printf("Bloqueado\n");
        close(fd);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Desbloqueado\n");

        int cerr2;
        cerr2 = lockf(fd, F_LOCK, tam);
        if (cerr2 == -1)
        {
            printf("Error cerr\n");
            exit(EXIT_FAILURE);
        }
        time_t timep;

        if ((timep = time(NULL)) == -1)
        {
            perror("time");
            exit(EXIT_FAILURE);
        }

        struct tm *tiempolocal;

        if ((tiempolocal = localtime(&timep)) == NULL)
        {
            perror("localtime");
            exit(EXIT_FAILURE);
        }

        printf("Son las: %i:%i\n", tiempolocal->tm_hour, tiempolocal->tm_min);

        sleep(5);

        int cerr3;
        cerr3 = lockf(fd, F_ULOCK, tam);
        if (cerr3 == -1)
        {
            printf("Error cerr\n");
            exit(EXIT_FAILURE);
        }
        close(fd);
    }

    exit(EXIT_SUCCESS);
}
