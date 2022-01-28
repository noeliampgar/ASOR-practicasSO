#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/*Ejercicio 5. Crear otra tubería con nombre. Escribir un programa que espere hasta que haya datos
listos para leer en alguna de ellas. El programa debe mostrar la tubería desde la que leyó y los datos
leídos. Consideraciones:
● Para optimizar las operaciones de lectura usar un buffer (ej. de 256 bytes).
● Usar read(2) para leer de la tubería y gestionar adecuadamente la longitud de los datos
leídos.
● Normalmente, la apertura de la tubería para lectura se bloqueará hasta que se abra para
escritura (ej. con echo 1 > tuberia). Para evitarlo, usar la opción O_NONBLOCK en open(2).
● Cuando el escritor termina y cierra la tubería, read(2) devolverá 0, indicando el fin de
fichero, por lo que hay que cerrar la tubería y volver a abrirla. Si no, select(2) considerará
el descriptor siempre listo para lectura y no se bloqueará.*/

int main(void)
{
    fd_set rfds;
    //char *buff[256];
    char buf;
    int op1 = open("./tub", O_NONBLOCK | O_RDONLY);

    if (op1 == -1)
    {
        perror("error open1");
        exit(EXIT_FAILURE);
    }
    int op2 = open("./tub2", O_NONBLOCK | O_RDONLY);

    if (op2 == -1)
    {
        perror("error open2");
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(op1, &rfds);
        FD_SET(op2, &rfds);

        /* Wait up to five seconds. */
        int max = op1;
        if (op2 > op1)
            max = op2;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        retval = select(max + 1, &rfds, NULL, NULL, &tv);

        /* Don't rely on the value of tv now! */

        if (retval == -1)
            perror("select()");
        else if (retval)
        {
            printf("Data is available now.\n");

            if (FD_ISSET(op1, &rfds))
            {
                printf("Tub1 gives the data.\n");
                //int wr = read(op1, buff, 256);

                while (read(op1, &buf, 1) > 0)
                    write(STDOUT_FILENO, &buf, 1);
                close(op1);
                int op1 = open("./tub", O_NONBLOCK | O_RDONLY);

                if (op1 == -1)
                {
                    perror("error open1");
                    exit(EXIT_FAILURE);
                }
            }
            else if (FD_ISSET(op2, &rfds))
            {
                printf("Tub2 gives the data.\n");

                while (read(op2, &buf, 1) > 0)
                    write(STDOUT_FILENO, &buf, 1);
                close(op2);
                int op2 = open("./tub2", O_NONBLOCK | O_RDONLY);

                if (op2 == -1)
                {
                    perror("error open2");
                    exit(EXIT_FAILURE);
                }
            }
    /* FD_ISSET(0, &rfds) will be true. */}
    else
        printf("No data within five seconds.\n");
    }
    exit(EXIT_SUCCESS);
}
