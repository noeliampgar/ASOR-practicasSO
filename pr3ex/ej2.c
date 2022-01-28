
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

/*Ejercicio 2. Escribir un programa que muestre la política de planificación (como cadena) y la
prioridad del proceso actual, además de mostrar los valores máximo y mínimo de la prioridad para la
política de planificación.*/

int main(int argc, char *argv[])
{

    int policy;
    pid_t pid = getpid();

    if ((policy = sched_getscheduler(pid)) == -1)
    {

        perror("Error sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    printf("Policy: ");

    switch (policy)
    {
    case SCHED_OTHER:
        printf("SCHED_OTHER\n");
        break;

    case SCHED_FIFO:
        printf("SCHED_FIFO\n");
        break;
    case SCHED_RR:
        printf("SCHED_RR\n");
        break;

    default:
        printf("unknown?\n");
        break;
    }

    int prio;

    if ((prio = getpriority(PRIO_PROCESS, pid)) == -1)
    {
        perror("Error getpriority");
        exit(EXIT_FAILURE);
    }

    printf("Priority: %i\n", prio);
    int maxprio;
    int minprio;
    if ((maxprio = sched_get_priority_max(policy)) == -1)
    {
        perror("Error prioritymax");
        exit(EXIT_FAILURE);
    }
    if ((minprio = sched_get_priority_min(policy)) == -1)
    {
        perror("Error prioritymin");
        exit(EXIT_FAILURE);
    }
    printf("Prioritymax and min: %i %i\n", maxprio, minprio);

    exit(EXIT_SUCCESS);
}
