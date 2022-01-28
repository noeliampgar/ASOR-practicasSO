
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>

/*Ejercicio 11. Escribir un programa que bloquee las señales SIGINT y SIGTSTP. Después de
bloquearlas el programa debe suspender su ejecución con sleep(3) un número de segundos que se
obtendrán de la variable de entorno SLEEP_SECS. Al despertar, el proceso debe informar de si
recibió la señal SIGINT y/o SIGTSTP. En este último caso, debe desbloquearla con lo que el proceso
se detendrá y podrá ser reanudado en la shell (imprimir una cadena antes de finalizar el programa
para comprobar este comportamiento).*/

int main(int argc, char *argv[])
{

    setenv("SLEEP_SECS", "10", 1);

    char *duerme = getenv("SLEEP_SECS");

    if (duerme == NULL)
    {

        perror("getenv");
        exit(EXIT_FAILURE);
    }

    sigset_t set;

    int cjtovacio = sigemptyset(&set);
    int add1 = sigaddset(&set, SIGINT);
    int add2 = sigaddset(&set, SIGTSTP);
    if (cjtovacio == -1)
    {

        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }

    if (add1 == -1)
    {

        perror("sigaddset");
        exit(EXIT_FAILURE);
    }
    if (add2 == -1)
    {

        perror("sigaddset");
        exit(EXIT_FAILURE);
    }

    /* Prototype for the glibc wrapper function */
    int mascara = sigprocmask(SIG_BLOCK, &set, NULL);
    if (mascara == -1)
    {

        perror("mascara");
        exit(EXIT_FAILURE);
    }
    printf("Segundos duermiendo: %i\n", atoi(duerme));

    sleep(atoi(duerme));
    sigset_t pendientes;

    int nump = sigpending(&pendientes);

    if (nump == -1)
    {

        perror("sigpending");
        exit(EXIT_FAILURE);
    }
    int miembro = sigismember(&pendientes, SIGINT);
    int miembro2 = sigismember(&pendientes, SIGTSTP);

    if (miembro == -1)
    {

        perror("sigismember");
        exit(EXIT_FAILURE);
    }
    if (miembro2 == -1)
    {

        perror("sigismember");
        exit(EXIT_FAILURE);
    }
    if (miembro == 1)
    {
        sigset_t set2;

        sigemptyset(&set2);
        sigaddset(&set2, SIGINT);
        printf("Desbloqueando señales sigint...\n");
        int mascaraeliminar = sigprocmask(SIG_UNBLOCK, &set2, NULL);
        if (mascaraeliminar == -1)
        {

            perror("mascara");
            exit(EXIT_FAILURE);
        }
    }
    if (miembro2 == 1)
    {
        printf("Desbloqueando señales sigtstp...\n");
        int mascaraeliminar = sigprocmask(SIG_UNBLOCK, &pendientes, NULL);
        if (mascaraeliminar == -1)
        {

            perror("mascara");
            exit(EXIT_FAILURE);
        }
    }
    printf("Fin...\n");

    exit(EXIT_SUCCESS);
}
