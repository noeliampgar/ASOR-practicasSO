#include <time.h>

#include <stdlib.h>
#include <stdio.h>

int main()
{
    time_t tloc;

    if ((tloc = time(NULL)) == -1)
    {

        perror("Error time");
        exit(EXIT_FAILURE);
    }
    printf("Time %li\n", tloc);

    exit(EXIT_SUCCESS);
}
