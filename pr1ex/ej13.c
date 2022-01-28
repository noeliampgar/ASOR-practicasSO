#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int main()
{
    struct timeval tv;

    time_t tloc;

    if ((gettimeofday(&tv, NULL)) == -1)
    {

        perror("Error gettimeofday");
        exit(EXIT_FAILURE);
    }

    size_t i;
    for (i = 0; i < 100000; ++i)
        ;

    struct timeval tv2;

    int timeday2;

    time_t tloc2;

    if ((gettimeofday(&tv2, NULL)) == -1)
    {

        perror("Error gettimeofday2");
        exit(EXIT_FAILURE);
    }
    printf("Time in micros %li\n", tv2.tv_usec - tv.tv_usec);

    exit(EXIT_SUCCESS);
}
