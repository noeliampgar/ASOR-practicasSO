#include <sys/utsname.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    long a;

    if ((a = sysconf(_SC_CHILD_MAX)) == -1)
    {
        //printf("Error: %s\n", strerror(errno));
        perror("Error en sysconf");
        exit(EXIT_FAILURE);
    }

    printf("Numero maximo hijos %li\n",a);

    if ((a = sysconf(_SC_OPEN_MAX)) == -1)
    {
        //printf("Error: %s\n", strerror(errno));
        perror("Error en sysconf");
        exit(EXIT_FAILURE);
    }
        printf("Numero maximo ficheros abiertos %li\n", a);

    if ((a = sysconf(_SC_ARG_MAX)) == -1)
    {
        //printf("Error: %s\n", strerror(errno));
        perror("Error en sysconf");
        exit(EXIT_FAILURE);
    }
        printf("Numero maximo args %li\n", a);


    exit(EXIT_SUCCESS);
}
