
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Error argumentos\n");
        exit(EXIT_FAILURE);
    }
    const char *pathname = argv[1];
    mode_t modo;
    mode_t mask;

    modo = umask(0027);

    int op;

    if ((op = open(pathname, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IXOTH)) == -1)
    {

        printf("Error open\n");
        exit(EXIT_FAILURE);
    }
close(op);
    exit(EXIT_SUCCESS);
}
