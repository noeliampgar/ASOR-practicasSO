
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

/*Ejercicio 12. Escribir un programa que instale un manejador para las señales SIGINT y SIGTSTP. El
manejador debe contar las veces que ha recibido cada señal. El programa principal permanecerá en
un bucle que se detendrá cuando se hayan recibido 10 señales. El número de señales de cada tipo se
mostrará al finalizar el programa.
*/
volatile int cont1 = 0;
volatile int cont2 = 0;

static void
handler(int sig)
{
    if (sig == SIGINT)
        cont1 = cont1 + 1;
    else if (sig == SIGTSTP)
        cont2 = cont2 + 1;
}

int main(int argc, char *argv[])
{
    /*   struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
*/

    struct sigaction act1;
    struct sigaction act2;
    act1.sa_handler = handler;
    act2.sa_handler = handler;

    sigaction(SIGINT, &act1, NULL);

    sigaction(SIGTSTP, &act2, NULL);
    while (cont1 + cont2 < 10)
        ;

    printf("sigint: %i + sigtstp: %i\n", cont1, cont2);

    exit(EXIT_SUCCESS);
}
