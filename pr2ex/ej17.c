#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>

#include <dirent.h>
#include <errno.h>
#include <string.h>

/*Ejercicio 17. Escribir un programa que cumpla las siguientes especificaciones:
● El programa tiene un único argumento que es la ruta a un directorio. El programa debe
comprobar la corrección del argumento.
● El programa recorrerá las entradas del directorio de forma que:
    ○ Si es un fichero normal, escribirá el nombre.
    ○ Si es un directorio, escribirá el nombre seguido del carácter ‘/’.
    ○ Si es un enlace simbólico, escribirá su nombre seguido de ‘->’ y el nombre del
      fichero enlazado. Usar readlink(2) y dimensionar adecuadamente el buffer.
    ○ Si el fichero es ejecutable, escribirá el nombre seguido del carácter ‘*’.
● Al final de la lista el programa escribirá el tamaño total que ocupan los ficheros (no
directorios) en kilobytes.*/

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dirp;
    const char *name = argv[1];
    if ((dirp = opendir(name)) == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }
    struct dirent *entradas;
    int bites = 0;

    while (1 == 1)
    {
        errno = 0;
        if ((entradas = readdir(dirp)) == NULL)
        {
            printf("Tamanyo en kb: %i\n", bites / 1024);

            perror("readdir");
            exit(EXIT_FAILURE);
        }

        char *nombre;
        nombre = entradas->d_name;

        char *ruta;
        ruta = malloc(strlen(name) + strlen(nombre) + 1); /* make space for the new string (should check the return value ...) */
        strcpy(ruta, name);                               /* copy name into the new var */
        strcat(ruta, "/");                                /* add the extension */
        strcat(ruta, nombre);                             /* add the extension */
        printf("%s\n", ruta);

        struct stat sb;

        if (lstat(ruta, &sb) == -1)
        {
            perror("lstat");
            exit(EXIT_FAILURE);
        }
        printf("File type:                ");

        switch (sb.st_mode & S_IFMT)
        {

        case S_IFDIR:

            printf("directory                ");
            printf("%s/\n", nombre);

            break;

        case S_IFLNK:
            printf("symlink                ");
            printf("%s-> ", nombre);

            ssize_t tamanyo;
            char *buf = malloc(4096);

            if ((tamanyo = readlink(ruta, buf, strlen(ruta))) == -1)
            {
                perror("readlink");
                exit(EXIT_FAILURE);
            }
            printf("%s\n", buf);
            free(buf)
            bites += sb.st_size;

            break;
        case S_IFREG:
            if (sb.st_mode & S_IXUSR)
            {

                printf("exec file                ");
                printf("%s*\n", nombre);
            }
            else
            {

                printf("regular file                ");
                printf("%s\n", nombre);
            }
            bites += sb.st_size;

            break;

        default:
            printf("unknown?\n");
            break;
        }

        printf("\n\n");
    }

    int cierre;

    if ((cierre = closedir(dirp)) == -1)
    {
        perror("closedir");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
