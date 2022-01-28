#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Error, faltan argumentos\n");
        exit(1);
    }
    const char *pathname = argv[1];
    int flags;
    mode_t mode;
    int op;

    if ((op = open(pathname, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH)) == -1)
    {

        printf("Error open\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
