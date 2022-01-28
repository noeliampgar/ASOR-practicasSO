#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    if (setuid(0) == -1)
    {

        perror("Error en setuid");

        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
