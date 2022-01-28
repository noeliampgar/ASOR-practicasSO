#include <sys/utsname.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    struct utsname buf=malloc(sizeof(struct utsname));
    if (uname(buf) == -1)
    {
        //printf("Error: %s\n", strerror(errno));
        perror("Error en uname");
        exit(EXIT_FAILURE);
    }
    //printf("Error: %s\n", strerror(errno));
    printf("Sysname: %s\n", buf->sysname);
    printf("Nodename: %s\n", buf->nodename);
    printf("Release: %s\n", buf->release);
    printf("Version: %s\n", buf->version);
    printf("Machine: %s\n", buf->machine);
    printf("domainname: %s\n", buf->__domainname);
    exit(EXIT_SUCCESS);
}
