
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>
#include <signal.h>
#include <string.h>

/*Ejercicio 13. Escribir un programa que realice el borrado programado del propio ejecutable. El
programa tendrá como argumento el número de segundos que esperará antes de borrar el fichero.
El borrado del fichero se podrá detener si se recibe la señal SIGUSR1.
Nota: Usar sigsuspend(2) para suspender el proceso y la llamada al sistema apropiada para borrar
el fichero.
*/

volatile int vivo = 0;

static void
handler(int sig)
{
    vivo = 1;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <num of secs to delete exe>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sigaction act;


    act.sa_handler = handler;

    int accion = sigaction(10, &act,
                           NULL);
    sleep(atoi(argv[1]));

    if (vivo == 0)
    {
        char *comando = malloc(sizeof(argv[0] + 3));
        strcpy(comando, "rm ");
        strcat(comando, argv[0]);
        printf("El comando es: %s\n", comando);

        int sis = system(comando);
        if (sis == -1)
        {
            perror("sys");
            exit(EXIT_FAILURE);
        }
        printf("Se ha borrado exe.\n");
    }

    else

        printf("No se ha borrado exe.\n");

return(0);}
