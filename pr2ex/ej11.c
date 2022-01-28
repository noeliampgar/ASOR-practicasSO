
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/sysmacros.h>

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat sb;

    if (lstat(argv[1], &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    if ((sb.st_mode & S_IFMT) == S_IFREG /*S_ISREG(b.st_mode & S_IFMT)*/)
    {
        char *oldpath = argv[1];
        char *newpath = malloc(strlen(oldpath) +  5);
        char *target = argv[1];
        char *linkpath = malloc(strlen(target) +  4);

        strcpy(newpath, oldpath); /* copy name into the new var */
        strcat(newpath, ".hard"); /* add the extension */

        strcpy(linkpath, target); /* copy name into the new var */
        strcat(linkpath, ".sym"); /* add the extension */

        int rigido;

        int simbolico;

        if ((rigido = link(oldpath, newpath)) == -1)
        {
            perror("error link");
            exit(EXIT_FAILURE);
        }

        if ((simbolico = symlink(target, linkpath)) == -1)
        {
            perror("error symlink");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
