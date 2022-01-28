#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <locale.h>
int main()
{
    char outstr[200];
    time_t t;
    struct tm *tmp;

    if ((t = time(NULL)) == -1)
    {
        perror("time");
        exit(EXIT_FAILURE);
    }
    tmp = localtime(&t);
    if (tmp == NULL)
    {
        perror("localtime");
        exit(EXIT_FAILURE);
    }
    setlocale(LC_ALL, "");

    if (strftime(outstr, sizeof(outstr), "%A, %d de %B de %y, %R", tmp) == 0)
    {

        printf("Error max\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", outstr);
    exit(EXIT_SUCCESS);
}
