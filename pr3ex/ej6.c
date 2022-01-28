
#include <signal.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

/*Ejercicio 5. Escribir un programa que muestre los identificadores del proceso: identificador de
proceso, de proceso padre, de grupo de procesos y de sesión. Mostrar además el número máximo
de ficheros que puede abrir el proceso y el directorio de trabajo actual.*/

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { /* Code executed by child */
        pid_t sesion = setsid();
        if (sesion == -1)
        {
            perror("setsid");
            exit(1);
        }

        pid_t pid = getpid();

        printf("Proceso actual: %i\n", pid);
        printf("Proceso padre: %i\n", getppid());
        pid_t pgid;

        if ((pgid = getpgid(pid)) == -1)
        {
            perror("Error getpgid");
            exit(EXIT_FAILURE);
        }
        printf("Grupo proceso: %i\n", pgid);
        pid_t sid;

        if ((sid = getsid(pid)) == -1)
        {
            perror("Error getsid");
            exit(EXIT_FAILURE);
        }
        printf("Sesion proceso: %i\n", sid);

        struct rlimit rlim;
        int err;
        err = getrlimit(RLIMIT_NOFILE, &rlim);
        if ((err) == -1)
        {
            perror("Error getrlimit");
            exit(EXIT_FAILURE);
        }
        printf("Limite fds: %li\n", rlim.rlim_max);

        char *dir;
        char buf;
        dir = getcwd(&buf, 200);

        if ((dir) == NULL)
        {
            perror("Error getcwd");
            exit(EXIT_FAILURE);
        }
        printf("Directorio antiguo: %s\n", dir);
        int nuevodir;
        nuevodir = chdir("/tmp");
        if ((nuevodir) == -1)
        {
            perror("Error chdir");
            exit(EXIT_FAILURE);
        }
        printf("Directorio nuevo: %s\n", "/tmp");
    }
    else
    { /* Code executed by parent */
        printf("Parent PID is %ld\n", (long)getpid());
        sleep(5);
    }
}
