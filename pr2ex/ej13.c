
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int oldfd;

    if ((oldfd = open(argv[1], O_RDWR | O_CREAT, 0666)) == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

    int duptwo;

    if ((duptwo = dup2(oldfd, 1)) == -1)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    printf("Estoy en el fichero para salida estandar\n");

        int duptwoerr;

    if ((duptwoerr = dup2(oldfd, 2)) == -1)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Estoy en el fichero para slaida err\n");

    exit(EXIT_SUCCESS);
}
