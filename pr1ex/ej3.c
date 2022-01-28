#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    int i;
    for (i = 0; i < 255; ++i)
    {

        printf("Error: %s\n", strerror(i));
    }

    exit(EXIT_SUCCESS);
}
