#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main()
{
    time_t timep;
    struct tm *tim;

    if ((timep = time(NULL)) == -1)
    {

        perror("Error time");
        exit(EXIT_FAILURE);
    }
    if ((tim = localtime(&timep)) == NULL)
    {

        perror("Error localtime");
        exit(EXIT_FAILURE);
    }

    printf("Year %i\n", tim->tm_year + 1900);

    exit(EXIT_SUCCESS);
}