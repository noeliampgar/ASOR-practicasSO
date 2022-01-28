#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    if (setuid(0) == -1)
    {

        perror("Error en setuid");
        printf("%i\n", errno);
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
