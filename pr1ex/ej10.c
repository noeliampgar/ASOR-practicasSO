#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>

int main()
{

    printf("id real %i\n", getuid());
    printf("id efectivo %i\n", geteuid());
    printf("id grupo %i\n", getgid());
    printf("gid grupo efectivo %i\n", getegid());

    uid_t uid;

    uid = getuid();
    struct passwd *pw;
    if ((pw = getpwuid(uid)) == -1)
    {

        perror("Error getpwuid");
    }
    printf("Nombre de usuario %s\n", pw->pw_name);
    printf("Home %s\n", pw->pw_dir);
    printf("Descripcion de usuario %s\n", pw->pw_gecos);
    exit(EXIT_SUCCESS);
}
