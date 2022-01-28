#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
//uid_t a = getuid()) ;
printf("id real %i\n", getuid());
printf("id efectivo %i\n", geteuid());
printf("id grupo %i\n", getgid());
printf("gid grupo efectivo %i\n", getegid());

exit(EXIT_SUCCESS);
}